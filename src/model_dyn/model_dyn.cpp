#define _USE_MATH_DEFINES
#include <cmath>
#include "model_dyn.hpp"

DynMod::DynMod(const Data& data, const H& theta, const Params& nu) : data_(data) {
    gen_params.resize(data.generators.size());
    for (size_t i = 0; i < data.generators.size(); ++i) {
        gen_params[i].Pm = data.generators[i].Pg;
        gen_params[i].H = (i == 0) ? theta.val : nu.h(i-1);
        gen_params[i].D = nu.d(i);
        gen_params[i].bus = data.generators[i].bus;
    }
}

DynMod::StateDot DynMod::derivative(const State& x, double t) const {
    int n = gen_params.size();
    Vector10d dx = Vector10d::Zero();

    for (int i = 0; i < n; ++i) {
        double delta = x(2 * i);
        double omega = x(2 * i + 1);
        double H = gen_params[i].H;
        double D = gen_params[i].D;
        double Pm = gen_params[i].Pm;

        double Pe = gen_params[i].Pm - gen_params[i].D * omega - 0.1 * sin(delta);

        dx(2 * i) = omega;
        dx(2 * i + 1) = (omega_s / (2 * H)) * (Pm - Pe - D * omega);
    }

    return dx;
}

DynMod::State DynMod::integrate(const State& init, double t0, double t1, double dt) const {
    State x = init;
    double t = t0;
    while (t < t1) {
        Vector10d k1 = derivative(x, t);
        Vector10d k2 = derivative(x + 0.5 * dt * k1, t + 0.5 * dt);
        Vector10d k3 = derivative(x + 0.5 * dt * k2, t + 0.5 * dt);
        Vector10d k4 = derivative(x + dt * k3, t + dt);

        x += (dt / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);
        t += dt;
    }
    return x;
}

DynMod::State DynMod::rk4_step(const State& x, double dt) const {
    return integrate(x, 0, dt, dt);
}
