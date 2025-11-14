#include "ais.hpp"
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

AI::AI(const LogLik& loglik, const LogPrior& logprior)
    : loglik_(loglik), logprior_(logprior) {}

double AI::estimate_evidence(const H& theta, int n_runs, int n_temperatures) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dist_other_H(5.0, 10.0);
    std::normal_distribution<> dist_D(0.1, 1.0);
    std::normal_distribution<> dist_beta(0.0, 2.0);

    double total_log_z = 0.0;
    for (int run = 0; run < n_runs; ++run) {
        std::vector<double> loglik_samples;
        for (int s = 0; s < n_temperatures; ++s) {
            Params nu;
            nu.h = Eigen::VectorXd(4);
            for (int i = 0; i < 4; ++i) nu.h(i) = dist_other_H(gen);
            nu.d = Eigen::VectorXd(5);
            for (int i = 0; i < 5; ++i) nu.d(i) = dist_D(gen);

            Discrep beta;
            beta.coeffs = Eigen::VectorXd(6);
            for (int i = 0; i < 6; ++i) beta.coeffs(i) = dist_beta(gen);

            double loglik_val = loglik_.loglik(theta, nu, beta);
            loglik_samples.push_back(loglik_val);
        }

        double max_log = *std::max_element(loglik_samples.begin(), loglik_samples.end());
        double sum_exp = 0.0;
        for (double l : loglik_samples) {
            sum_exp += std::exp(l - max_log);
        }
        double log_z = max_log + std::log(sum_exp / n_temperatures);
        total_log_z += log_z;
    }
    return total_log_z / n_runs;
}
