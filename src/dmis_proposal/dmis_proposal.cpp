#define _USE_MATH_DEFINES
#include <cmath>
#include "dmis_proposal.hpp"
#include <random>

DMISProp::DMISProp(const Params& nu_hat, const Eigen::Matrix<double,9,9>& H, double pi_0)
    : gen_(std::random_device{}()), dist_(0.0, 1.0) {
    int n = 9;
    Vector9d mu_hat;
    for (int i = 0; i < 4; ++i) mu_hat(i) = nu_hat.h(i);
    for (int i = 0; i < 5; ++i) mu_hat(i+4) = nu_hat.d(i);

    Eigen::Matrix<double,9,9> Sigma_def = Eigen::Matrix<double,9,9>::Identity() * 10.0;
    components_.emplace_back(mu_hat, Sigma_def, pi_0);

    Eigen::Matrix<double,9,9> neg_H = -H;
    Eigen::Matrix<double,9,9> Sigma_mode = neg_H.inverse();
    std::vector<double> scales = {0.1, 0.5, 1.0, 2.0};
    double remaining_pi = 1.0 - pi_0;
    double pi_per = remaining_pi / scales.size();
    for (double c : scales) {
        Eigen::Matrix<double,9,9> Sigma_scaled = c * c * Sigma_mode;
        components_.emplace_back(mu_hat, Sigma_scaled, pi_per);
    }
}

Params DMISProp::sample() const {
    double r = dist_(gen_);
    double cum = 0.0;
    for (const auto& comp : components_) {
        cum += comp.weight;
        if (r < cum) {
            if (!comp.is_valid) {
                Params nu;
                nu.h = Vector4d::Zero();
                nu.d = Vector5d::Zero();
                return nu;
            }
            Vector9d z = Vector9d::Zero();
            for (int i = 0; i < 9; ++i) {
                z(i) = dist_(gen_);
            }
            Vector9d nu_vec = comp.mean + comp.llt_cov.matrixL() * z;
            Params nu;
            nu.h = nu_vec.head(4);
            nu.d = nu_vec.tail(5);
            return nu;
        }
    }
    Params nu;
    nu.h = Vector4d::Zero();
    nu.d = Vector5d::Zero();
    return nu;
}

double DMISProp::log_density(const Params& nu) const {
    Vector9d nu_vec;
    for (int i = 0; i < 4; ++i) nu_vec(i) = nu.h(i);
    for (int i = 0; i < 5; ++i) nu_vec(i+4) = nu.d(i);

    double log_dens = -std::numeric_limits<double>::infinity();
    for (const auto& comp : components_) {
        double comp_log_dens;
        if (!comp.is_valid) {
            comp_log_dens = -std::numeric_limits<double>::infinity();
        } else {
            Vector9d diff = nu_vec - comp.mean;
            Vector9d z = comp.llt_cov.matrixL().solve(diff);
            double quad = z.squaredNorm();
            double log_norm = 0.5 * 9 * std::log(2 * M_PI) + 0.5 * comp.log_det;
            comp_log_dens = -0.5 * quad - log_norm + std::log(comp.weight);
        }
        log_dens = std::log(std::exp(log_dens) + std::exp(comp_log_dens));
    }
    return log_dens;
}
