#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include "../../include/ieee14_data.hpp"
#include "model_dyn.hpp"
#include "../../include/types.hpp"

int main() {
    IEEE14Data data;  // Assumes default construction

    // Test theta values
    std::vector<double> test_thetas = {3.0, 4.0, 5.0, 6.0, 7.0};

    // Fixed nu: all H=5.0, D=0.1
    Nu nu;
    nu.other_H = Eigen::VectorXd::Constant(4, 5.0);
    nu.D = Eigen::VectorXd::Constant(5, 0.1);

    // Initial state: delta=0, omega=0 for all
    DynamicModel::State initial_state = DynamicModel::State::Zero();

    double t_end = 1.0;
    double dt = 0.001;

    for (double theta_val : test_thetas) {
        Theta theta{theta_val};
        DynamicModel dyn_model(data, theta, nu);
        DynamicModel::State final_state = dyn_model.integrate(initial_state, 0.0, t_end, dt);

        std::cout << "Theta=" << theta_val << ": delta1=" << final_state(0) 
                  << ", omega1=" << final_state(1) << ", f_dev=" << final_state(1) / (2 * M_PI) << std::endl;
    }

    std::cout << "Model_dyn test passed: Integration completes without errors." << std::endl;
    return 0;
}
