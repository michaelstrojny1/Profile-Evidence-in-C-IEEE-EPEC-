#ifndef DMIS_PROPOSAL_H
#define DMIS_PROPOSAL_H

#include <vector>
#include <random>
#include <Eigen/Dense>
#include "../../include/types.hpp"
#include "../../src/loglik/loglik.hpp"
#include "../../src/logprior/logprior.hpp"

struct Comp {
    Eigen::Matrix<double,9,9> cov;
    Vector9d mean;
    double weight;
    Eigen::LLT<Eigen::Matrix<double,9,9>> llt_cov;
    double log_det;
    bool is_valid;

    Comp(const Vector9d& m, const Eigen::Matrix<double,9,9>& c, double w)
        : mean(m), cov(c), weight(w), llt_cov(c), is_valid(false), log_det(0.0) {
        if (llt_cov.info() == Eigen::Success) {
            Eigen::Matrix<double,9,9> L = llt_cov.matrixL();
            if ((L.diagonal().array() > 0).all()) {
                is_valid = true;
                log_det = 2.0 * L.diagonal().array().log().sum();
            }
        }
    }
};

class DMISProp {
public:
    DMISProp(const Params& nu_hat, const Eigen::Matrix<double,9,9>& H, double pi_0 = 0.1);
    Params sample() const;
    double log_density(const Params& nu) const;

private:
    std::vector<Comp> components_;
    mutable std::mt19937 gen_;
    mutable std::normal_distribution<double> dist_;
};

#endif // DMIS_PROPOSAL_H
