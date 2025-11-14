#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <omp.h>
#include <random>
#include <fstream>
#include "include/types.hpp"
#include "include/ieee14_data.hpp"
#include "src/orchestration/orchestration.hpp"
#include "src/obs_model/obs_model.hpp"

#include "src/audit/audit.hpp"

int main() {
    AL::log("Starting Bayesian evidence estimation");

    std::cout << "Bayesian Evidence vs Profile Likelihood" << std::endl;

    std::vector<double> theta_grid;
    for (double h = 3.0; h <= 6.01; h += 0.1) {
        theta_grid.push_back(h);
    }

    Data data;
    ObsMod obs_model(data);
    H theta_true{5.5};
    Params nu_mean;
    nu_mean.h = Vector4d::Constant(5.0);
    nu_mean.d = Vector5d::Constant(0.1);
    Discrep beta_zero;
    beta_zero.coeffs = Vector6d::Zero();
    Vector6d true_y_obs = obs_model.observe(theta_true, nu_mean, beta_zero);
    double sigma_y = 0.1;

    const int n_runs = 10;
    size_t n_theta = theta_grid.size();
    std::vector<std::vector<double>> all_laplace(n_runs, std::vector<double>(n_theta));
    std::vector<std::vector<double>> all_dmis(n_runs, std::vector<double>(n_theta));
    std::vector<std::vector<double>> all_profile_lik(n_runs, std::vector<double>(n_theta));

#pragma omp parallel for
    for (int run = 0; run < n_runs; ++run) {
        std::mt19937 gen(run);
        std::normal_distribution<> noise_dist(0.0, sigma_y);
        Vector6d y_obs = true_y_obs;
        for (int i = 0; i < 6; ++i) {
            y_obs(i) += noise_dist(gen);
        }

        Orch orch(y_obs, sigma_y, data);
        std::vector<double> log_Z_laplace, log_Z_dmis, prof_lik;
        orch.run(theta_grid, log_Z_laplace, log_Z_dmis, prof_lik, true);
        all_laplace[run] = log_Z_laplace;
        all_dmis[run] = log_Z_dmis;
        all_profile_lik[run] = prof_lik;
    }

    std::vector<double> avg_laplace(n_theta, 0.0);
    std::vector<double> avg_dmis(n_theta, 0.0);
    std::vector<double> avg_profile_lik(n_theta, 0.0);
    std::vector<int> valid_runs(n_theta, 0);
    for (size_t i = 0; i < n_theta; ++i) {
        for (int run = 0; run < n_runs; ++run) {
            if (!std::isnan(all_laplace[run][i])) {
                avg_laplace[i] += all_laplace[run][i];
                valid_runs[i]++;
            }
            if (!std::isnan(all_dmis[run][i])) {
                avg_dmis[i] += all_dmis[run][i];
                valid_runs[i] = std::max(valid_runs[i], 1);
            }
            if (!std::isnan(all_profile_lik[run][i])) {
                avg_profile_lik[i] += all_profile_lik[run][i];
                valid_runs[i]++;
            }
        }
        if (valid_runs[i] > 0) {
            avg_laplace[i] /= valid_runs[i];
            avg_dmis[i] /= valid_runs[i];
            avg_profile_lik[i] /= valid_runs[i];
        } else {
            avg_laplace[i] = -std::numeric_limits<double>::infinity();
            avg_dmis[i] = -std::numeric_limits<double>::infinity();
            avg_profile_lik[i] = -std::numeric_limits<double>::infinity();
        }
    }

    std::vector<double> log_evidence_post(n_theta);
    double max_log_evidence = -std::numeric_limits<double>::infinity();
    for (size_t i = 0; i < n_theta; ++i) {
        H theta{theta_grid[i]};
        double log_prior = LogPrior().logprior_theta(theta);
        log_evidence_post[i] = avg_dmis[i] + log_prior;
        if (log_evidence_post[i] > max_log_evidence) max_log_evidence = log_evidence_post[i];
    }

    std::vector<double> log_profile_post(n_theta);
    double max_log_profile = -std::numeric_limits<double>::infinity();
    for (size_t i = 0; i < n_theta; ++i) {
        H theta{theta_grid[i]};
        double log_prior = LogPrior().logprior_theta(theta);
        log_profile_post[i] = avg_profile_lik[i] + log_prior;
        if (log_profile_post[i] > max_log_profile) max_log_profile = log_profile_post[i];
    }

    double max_log_profile_post = *std::max_element(log_profile_post.begin(), log_profile_post.end());
    std::vector<double> evidence_post(n_theta);
    std::vector<double> profile_post(n_theta);
    double sum_evidence = 0.0, sum_profile = 0.0;
    for (size_t i = 0; i < n_theta; ++i) {
        evidence_post[i] = std::exp(avg_dmis[i] - max_log_evidence);
        profile_post[i] = std::exp(log_profile_post[i] - max_log_profile_post);
        sum_evidence += evidence_post[i];
        sum_profile += profile_post[i];
    }
    for (auto& p : evidence_post) p /= sum_evidence;
    for (auto& p : profile_post) p /= sum_profile;

    std::ofstream outfile("bayesian_evidence_results.csv");
    outfile << "Theta,Avg_Laplace_Z,Avg_DMIS_Z,Avg_Profile_Lik,Evidence_Posterior,Profile_Posterior" << std::endl;
    for (size_t i = 0; i < n_theta; ++i) {
        outfile << theta_grid[i] << "," 
                << avg_laplace[i] << "," 
                << avg_dmis[i] << "," 
                << avg_profile_lik[i] << "," 
                << evidence_post[i] << "," 
                << profile_post[i] << std::endl;
    }
    outfile.close();

    std::cout << "Results saved to bayesian_evidence_results.csv" << std::endl;
    std::cout << "Evidence Posterior Peak: theta=" 
              << theta_grid[std::distance(evidence_post.begin(), std::max_element(evidence_post.begin(), evidence_post.end()))] 
              << std::endl;
    std::cout << "Profile Posterior Peak: theta=" 
              << theta_grid[std::distance(profile_post.begin(), std::max_element(profile_post.begin(), profile_post.end()))] 
              << std::endl;

    AL::log("Completed Bayesian evidence estimation");

    return 0;
}
