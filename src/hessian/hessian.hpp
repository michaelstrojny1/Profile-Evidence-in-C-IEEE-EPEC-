#ifndef HESSIAN_H
#define HESSIAN_H

#include "../../include/types.hpp"
#include <Eigen/Dense>
#include "../../src/loglik/loglik.hpp"
#include "../../src/logprior/logprior.hpp"

class Hess {
public:
    Hess(const LogLik& loglik, const LogPrior& logprior);

    Eigen::MatrixXd compute_hessian(const H& theta, const Params& nu_hat, const Discrep& beta_hat);

private:
    const LogLik& loglik_;
    const LogPrior& logprior_;
};

#endif // HESSIAN_H
