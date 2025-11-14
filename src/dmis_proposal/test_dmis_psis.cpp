#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include "../../include/ieee14_data.hpp"
#include "../loglik/loglik.hpp"
#include "dmis_proposal.hpp"
#include "../importance_weights/importance_weights.hpp"
#include "../psis/psis.hpp"
#include "../../include/types.hpp"

#include "../logprior/logprior.hpp"

int main() {
    IEEE14Data data;
    Vector6d y_obs;
    y_obs << 0.0, 0.0, 232.4, 1.06, 0.0, 60.0;
    double sigma_y = 0.1;

    LogLikelihood loglik(y_obs, sigma_y, data);
    LogPrior logprior;

    Theta theta{4.0};
    Nu nu;
    nu.other_H = Eigen::VectorXd::Constant(4, 5.0);
    nu.D = Eigen::VectorXd::Constant(5, 0.1);
    Eigen::Matrix<double,9,9> H_block = Eigen::MatrixXd::Identity(9,9) * 1.0;  // Dummy H

    DMISProposal proposal(nu, H_block);
    ImportanceWeights iw(theta, proposal, loglik, logprior);
    std::vector<double> weights = iw.compute_weights(1000);  // Smaller N for test

    PSIS::Result psis_result = PSIS::psis(weights);

    std::cout << "Weights size: " << weights.size() << std::endl;
    std::cout << "PSIS k_hat: " << psis_result.k_hat << std::endl;
    std::cout << "PSIS ESS: " << psis_result.ess << std::endl;
    std::cout << "Smoothed weights size: " << psis_result.smoothed_weights.size() << std::endl;

    if (psis_result.k_hat < 1.0 && psis_result.ess > 1) {
        std::cout << "DMIS PSIS test passed: Reasonable diagnostics." << std::endl;
    } else {
        std::cout << "DMIS PSIS test failed: Bad diagnostics." << std::endl;
    }

    return 0;
}
