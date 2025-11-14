#define _USE_MATH_DEFINES
#include <random>
#include "importance_weights.hpp"
#include "../../include/types.hpp"

IW::IW(const H& theta, const DMISProp& proposal, const LogLik& loglik, const LogPrior& logprior)
    : theta_(theta), proposal_(proposal), loglik_(loglik), logprior_(logprior) {}

std::vector<double> IW::compute_weights(size_t N) {
    std::vector<double> weights(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0.0, 1.0);
    for (size_t s = 0; s < N; ++s) {
        Params nu = proposal_.sample();
        Discrep dsc;
        dsc.coeffs = Eigen::VectorXd(6);
        for (int i = 0; i < 6; ++i) {
            dsc.coeffs(i) = 0.1 * d(gen);
        }

        double log_p = loglik_.loglik(theta_, nu, dsc) + logprior_.logprior_nu(nu) + logprior_.logprior_beta(dsc);

        double log_q_nu = proposal_.log_density(nu);
        double log_q_beta = 0.0;
        double sigma_q2 = 0.01;
        for (int i = 0; i < 6; ++i) {
            double val = dsc.coeffs(i);
            log_q_beta += -0.5 * val * val / sigma_q2 - 0.5 * std::log(2 * M_PI * sigma_q2);
        }
        double log_q = log_q_nu + log_q_beta;
        double weight = std::exp(log_p - log_q);
        weights[s] = std::max(weight, 1e-300);
    }
    return weights;
}
