#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include "../../include/ieee14_data.hpp"
#include "orchestration.hpp"
#include "../../include/types.hpp"

int main() {
    IEEE14Data data;
    Vector6d y_obs;
    y_obs << 0.0, 0.0, 232.4, 1.06, 0.0, 60.0;
    double sigma_y = 0.1;

    Orchestration orch(y_obs, sigma_y, data);

    std::vector<double> theta_grid = {4.0};
    std::vector<double> log_Z_laplace, log_Z_dmis, prof_lik;

    std::cout << "Testing orchestration for theta=4.0..." << std::endl;
    orch.run(theta_grid, log_Z_laplace, log_Z_dmis, prof_lik, false);  // not quiet

    std::cout << "Laplace Z: " << log_Z_laplace[0] << std::endl;
    std::cout << "DMIS Z: " << log_Z_dmis[0] << std::endl;
    std::cout << "Profile Lik: " << prof_lik[0] << std::endl;

    if (!log_Z_laplace.empty() && !log_Z_dmis.empty() && !prof_lik.empty()) {
        std::cout << "Orchestration test passed: Computations completed." << std::endl;
    } else {
        std::cout << "Orchestration test failed: Missing outputs." << std::endl;
    }

    return 0;
}
