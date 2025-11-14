#ifndef LOGLIK_H
#define LOGLIK_H

#include <Eigen/Dense>
#include "../../include/types.hpp"
#include "../../src/obs_model/obs_model.hpp"
#include "../../include/ieee14_data.hpp"

using Vector6d = Eigen::Matrix<double,6,1>;

class LogLik {
public:
    LogLik(const Eigen::Matrix<double,6,1>& y_obs, double sigma_y, const Data& data);
    double loglik(const H& theta, const Params& nu, const Discrep& beta) const;

private:
    Eigen::Matrix<double,6,1> y_obs_;
    double sigma_y_;
    ObsMod obs_model_;
};

#endif // LOGLIK_H
