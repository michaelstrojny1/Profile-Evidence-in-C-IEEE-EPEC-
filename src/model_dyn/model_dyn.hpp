#ifndef MODEL_DYN_H
#define MODEL_DYN_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <Eigen/Dense>
#include "../../include/ieee14_data.hpp"
#include "../../include/types.hpp"

struct GenState {
    double delta;
    double omega;
};

class DynMod {
public:
    DynMod(const Data& data, const H& theta, const Params& nu);

    using State = Vector10d;
    using StateDot = Vector10d;

    StateDot derivative(const State& state, double t) const;

    State integrate(const State& initial, double t0, double t1, double dt) const;

    State rk4_step(const State& state, double dt) const;

    struct GenParams {
        double H;
        double D;
        double Pm;
        int bus;
    };

    std::vector<GenParams> gen_params;

private:
    const Data& data_;
    double omega_s = 2 * M_PI * 60;
};

#endif // MODEL_DYN_H
