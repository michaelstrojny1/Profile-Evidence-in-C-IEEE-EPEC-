#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "../../include/ieee14_data.hpp"
#include "loglik.hpp"
#include "../../include/types.hpp"

int main() {
    IEEE14Data data;
    Vector6d y_obs;
    y_obs << 0.0, 0.0, 232.4, 1.06, 0.0, 60.0;
    double sigma_y = 0.1;

    LogLikelihood loglik(y_obs, sigma_y, data);

    Theta theta{4.0};
    Nu nu;
    nu.other_H = Eigen::VectorXd::Constant(4, 5.0);
    nu.D = Eigen::VectorXd::Constant(5, 0.1);
    Beta beta = {Eigen::VectorXd::Zero(6)};

    double ll = loglik.loglik(theta, nu, beta);
    std::cout << "Log-likelihood: " << ll << std::endl;

    // For perfect match, should be high (less negative)
    if (ll > -10.0) {
        std::cout << "Loglik test passed: Reasonable value." << std::endl;
    } else {
        std::cout << "Loglik test failed: Too low." << std::endl;
    }

    return 0;
}
