#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "../../include/ieee14_data.hpp"
#include "obs_model.hpp"
#include "../../include/types.hpp"

int main() {
    IEEE14Data data;

    // Test theta, nu, beta
    Theta theta{4.0};
    Nu nu;
    nu.other_H = Eigen::VectorXd::Constant(4, 5.0);
    nu.D = Eigen::VectorXd::Constant(5, 0.1);
    Beta beta;
    beta.coeffs = Eigen::VectorXd::Zero(6);

    ObservationModel obs_model(data);
    Eigen::Matrix<double,6,1> y = obs_model.observe(theta, nu, beta);

    std::cout << "Observed y: " << y.transpose() << std::endl;
    std::cout << "Expected: delta1~0, omega1~0, Pe1=232.4, V_real1=1.06, V_imag1=0, f~60" << std::endl;

    // Check if reasonable
    if (std::abs(y(0)) < 0.1 && std::abs(y(1)) < 0.1 && std::abs(y(2) - 232.4) < 1.0 &&
        std::abs(y(3) - 1.06) < 0.1 && std::abs(y(4)) < 0.1 && std::abs(y(5) - 60.0) < 1.0) {
        std::cout << "Obs_model test passed: Outputs reasonable." << std::endl;
    } else {
        std::cout << "Obs_model test failed: Outputs not as expected." << std::endl;
    }

    return 0;
}
