#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "../../include/types.hpp"
#include "../logprior/logprior.hpp"

int main() {
    LogPrior logprior;

    Theta theta{4.0};
    double lp_theta = logprior.logprior_theta(theta);
    std::cout << "Log prior theta: " << lp_theta << std::endl;

    Nu nu;
    nu.other_H = Eigen::VectorXd::Constant(4, 5.0);
    nu.D = Eigen::VectorXd::Constant(5, 0.1);
    double lp_nu = logprior.logprior_nu(nu);
    std::cout << "Log prior nu: " << lp_nu << std::endl;

    Beta beta = {Eigen::VectorXd::Zero(6)};
    double lp_beta = logprior.logprior_beta(beta);
    std::cout << "Log prior beta: " << lp_beta << std::endl;

    if (lp_theta < 0 && lp_nu < 0 && lp_beta <= 0) {  // beta flat, 0
        std::cout << "Logprior test passed: Priors reasonable." << std::endl;
    } else {
        std::cout << "Logprior test failed." << std::endl;
    }

    return 0;
}
