#define _USE_MATH_DEFINES
#include <cmath>
#include "loglik.hpp"

LogLik::LogLik(const Eigen::Matrix<double,6,1>& y_obs, double sigma_y, const Data& data)
    : y_obs_(y_obs), sigma_y_(sigma_y), obs_model_(data) {}

double LogLik::loglik(const H& theta, const Params& nu, const Discrep& beta) const {
    Eigen::Matrix<double,6,1> y = obs_model_.observe(theta, nu, beta);
    Eigen::Matrix<double,6,1> d = y_obs_ - y;
    double q = d.squaredNorm();
    constexpr int n = 6;
    return -0.5 * q / (sigma_y_ * sigma_y_) - 0.5 * n * std::log(2 * M_PI * sigma_y_ * sigma_y_);
}
