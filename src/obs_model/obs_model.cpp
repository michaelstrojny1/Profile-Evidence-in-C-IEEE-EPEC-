#define _USE_MATH_DEFINES
#include <cmath>
#include "obs_model.hpp"
#include "../model_dyn/model_dyn.hpp"

ObsMod::ObsMod(const Data& data) : data_(data) {}

Eigen::Matrix<double,6,1> ObsMod::observe(const H& theta, const Params& nu, const Discrep& beta) const {
    DynMod dm(data_, theta, nu);
    Eigen::Matrix<double,10,1> init_s = Eigen::Matrix<double,10,1>::Zero();
    for (int i = 0; i < 5; ++i) init_s(2*i + 1) = 0.0;
    init_s(0) = 0.01;

    double t_end = 10.0;
    double dt = 0.001;

    Eigen::Matrix<double,10,1> s = dm.integrate(init_s, 0.0, t_end, dt);

    Eigen::Matrix<double,6,1> obs;
    obs(0) = s(0);
    obs(1) = s(1);
    double Pe1 = dm.gen_params[0].Pm - dm.gen_params[0].D * s(1) - 0.1 * sin(s(0));
    obs(2) = Pe1;
    obs(3) = 1.06;
    obs(4) = 0.0;
    obs(5) = 60.0 + s(1) / (2 * M_PI);

    obs += beta.coeffs;

    return obs;
}
