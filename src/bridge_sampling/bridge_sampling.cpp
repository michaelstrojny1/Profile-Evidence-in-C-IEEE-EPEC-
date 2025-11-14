#define _USE_MATH_DEFINES
#include "bridge_sampling.hpp"
#include <cmath>
#include <iostream>

BS::BS(const std::vector<Eigen::VectorXd>& posterior_samples,
                               const std::vector<Eigen::VectorXd>& proposal_samples,
                               const LogLik& loglik, const LogPrior& logprior,
                               const DMISProp& proposal)
    : posterior_samples_(posterior_samples), proposal_samples_(proposal_samples),
      loglik_(loglik), logprior_(logprior), proposal_(proposal) {}

double BS::log_tilde_pi(const Eigen::VectorXd& nu_beta, const H& theta) {
    Params nu;
    nu.h = Eigen::Map<const Vector4d>(nu_beta.data());
    nu.d = Eigen::Map<const Vector5d>(nu_beta.data() + 4);
    Discrep beta;
    beta.coeffs = Eigen::Map<const Vector6d>(nu_beta.data() + 9);
    return loglik_.loglik(theta, nu, beta) + logprior_.logprior_nu(nu) + logprior_.logprior_beta(beta);
}

double BS::estimate_evidence(const H& theta) {
    double lambda = 0.5;
    double num = 0.0, den = 0.0;

    for (const auto& sample : proposal_samples_) {
        Params nu;
        nu.h = Eigen::Map<const Vector4d>(sample.data());
        nu.d = Eigen::Map<const Vector5d>(sample.data() + 4);
        Discrep beta;
        beta.coeffs = Eigen::Map<const Vector6d>(sample.data() + 9);
        double log_L = loglik_.loglik(theta, nu, beta);
        double log_pi_nu = logprior_.logprior_nu(nu);
        double log_pi_beta = logprior_.logprior_beta(beta);
        double log_L_pi = log_L + log_pi_nu + log_pi_beta;
        double log_q_nu = proposal_.log_density(nu);
        double log_q_beta = 0.0;
        double sigma_q2 = 0.01;
        for (int i = 0; i < 6; ++i) {
            double val = beta.coeffs(i);
            log_q_beta += -0.5 * val * val / sigma_q2 - 0.5 * std::log(2 * M_PI * sigma_q2);
        }
        double log_q = log_q_nu + log_q_beta;
        num += std::exp(lambda * (log_L_pi - log_q));
    }
    num /= proposal_samples_.size();

    for (const auto& sample : posterior_samples_) {
        Params nu;
        nu.h = Eigen::Map<const Vector4d>(sample.data());
        nu.d = Eigen::Map<const Vector5d>(sample.data() + 4);
        Discrep beta;
        beta.coeffs = Eigen::Map<const Vector6d>(sample.data() + 9);
        double log_L = loglik_.loglik(theta, nu, beta);
        double log_pi_nu = logprior_.logprior_nu(nu);
        double log_pi_beta = logprior_.logprior_beta(beta);
        double log_L_pi = log_L + log_pi_nu + log_pi_beta;
        double log_q_nu = proposal_.log_density(nu);
        double log_q_beta = 0.0;
        double sigma_q2 = 0.01;
        for (int i = 0; i < 6; ++i) {
            double val = beta.coeffs(i);
            log_q_beta += -0.5 * val * val / sigma_q2 - 0.5 * std::log(2 * M_PI * sigma_q2);
        }
        double log_q = log_q_nu + log_q_beta;
        den += std::exp(lambda * (log_q - log_L_pi));
    }
    den /= posterior_samples_.size();

    return std::log(num / den);
}
