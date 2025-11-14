#define _USE_MATH_DEFINES
#include <cmath>
#include "orchestration.hpp"
#include <iostream>
#include <numeric>
#include "../../include/types.hpp"

Orch::Orch(const Vector6d& y_obs, double sigma_y, const Data& data)
    : loglik_(y_obs, sigma_y, data), logprior_(), optimizer_(loglik_, logprior_), hessian_(loglik_, logprior_) {}

void Orch::run(std::vector<double>& theta_grid, std::vector<double>& log_Z_laplace_grid, std::vector<double>& log_Z_dmis_grid, std::vector<double>& profile_loglik_grid, bool quiet) {
    for (double theta_val : theta_grid) {
        H theta{theta_val};
        ParamsDiscrep pd = optimizer_.optimize_nu_beta(theta, false);
        Eigen::MatrixXd H_mat = hessian_.compute_hessian(theta, pd.p, pd.d);

        double log_lik = loglik_.loglik(theta, pd.p, pd.d);
        double log_prior_nu = logprior_.logprior_nu(pd.p);
        double log_prior_beta = logprior_.logprior_beta(pd.d);
        double log_profile = log_lik + log_prior_nu + log_prior_beta;

        int d = H_mat.rows();
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigensolver(H_mat);
        double log_det_H = 0.0;
        if (eigensolver.info() == Eigen::Success) {
            Eigen::VectorXd eigenvalues = eigensolver.eigenvalues();
            eigenvalues = eigenvalues.array().max(1e-6);
            log_det_H = eigenvalues.array().log().sum();
        } else {
            H_mat += 1e-6 * Eigen::MatrixXd::Identity(H_mat.rows(), H_mat.cols());
            eigensolver.compute(H_mat);
            if (eigensolver.info() == Eigen::Success) {
                Eigen::VectorXd eigenvalues = eigensolver.eigenvalues();
                log_det_H = eigenvalues.array().log().sum();
            } else {
                log_det_H = 0.0;
            }
        }
        double log_Z_laplace = log_profile + 0.5 * d * std::log(2 * M_PI) - 0.5 * log_det_H;

        Eigen::Matrix<double,9,9> H_block = H_mat.block<9,9>(0,0);
        DMISProp proposal(pd.p, H_block);
        IW iw(theta, proposal, loglik_, logprior_);
        std::vector<double> weights = iw.compute_weights(50000);

        PS::Result psis_result = PS::psis(weights);

        bool accepted = (psis_result.k_hat < 0.7) && (psis_result.ess > 10);
        if (!accepted && !quiet) {
            std::cout << "Warning: Evidence estimate not accepted for theta=" << theta_val 
                      << " (k=" << psis_result.k_hat << ", ESS=" << psis_result.ess << ")" << std::endl;
        }

        double log_Z_dmis = std::log(std::accumulate(psis_result.smoothed_weights.begin(), psis_result.smoothed_weights.end(), 0.0));
        log_Z_laplace_grid.push_back(log_Z_laplace);
        log_Z_dmis_grid.push_back(log_Z_dmis);

        ParamsDiscrep pd_profile = optimizer_.optimize_nu_beta(theta, true);
        double profile_loglik = loglik_.loglik(theta, pd_profile.p, pd_profile.d);
        profile_loglik_grid.push_back(profile_loglik);

        if (!quiet) {
            std::cout << theta_val << "," 
                      << log_profile << "," 
                      << log_Z_laplace << "," 
                      << log_Z_dmis << "," 
                      << (log_Z_laplace - log_Z_dmis) << ","
                      << psis_result.k_hat << "," 
                      << psis_result.ess << "," 
                      << (accepted ? "1" : "0") << ","
                      << profile_loglik << std::endl;
        }
    }
}
