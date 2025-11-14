#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "../../include/ieee14_data.hpp"
#include "optimizer.hpp"
#include "../../include/types.hpp"

int main() {
    IEEE14Data data;
    Vector6d y_obs;
    y_obs << 0.0, 0.0, 232.4, 1.06, 0.0, 60.0;
    double sigma_y = 0.1;
    LogLikelihood loglik(y_obs, sigma_y, data);
    LogPrior logprior;

    Optimizer opt(loglik, logprior);

    Theta theta{4.0};

    // Test evidence optimization (nu + beta)
    std::cout << "Testing evidence optimization..." << std::endl;
    NuBeta nubeta_ev = opt.optimize_nu_beta(theta, false);
    double obj_ev = -loglik.loglik(theta, nubeta_ev.nu, nubeta_ev.beta) - logprior.logprior_nu(nubeta_ev.nu) - logprior.logprior_beta(nubeta_ev.beta);
    std::cout << "Final objective (evidence): " << obj_ev << std::endl;

    // Test profile optimization (nu only, beta=0)
    std::cout << "Testing profile optimization..." << std::endl;
    NuBeta nubeta_prof = opt.optimize_nu_beta(theta, true);
    double obj_prof = -loglik.loglik(theta, nubeta_prof.nu, nubeta_prof.beta);
    std::cout << "Final objective (profile): " << obj_prof << std::endl;

    // Check if objectives are reasonable (negative)
    if (obj_ev < 0 && obj_prof < 0) {
        std::cout << "Optimizer test passed: Converged to reasonable objectives." << std::endl;
    } else {
        std::cout << "Optimizer test failed: Objectives not negative." << std::endl;
    }

    return 0;
}
