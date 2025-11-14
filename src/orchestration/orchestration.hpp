#ifndef ORCHESTRATION_H
#define ORCHESTRATION_H

#include <vector>
#include "../../include/types.hpp"
#include "../../include/ieee14_data.hpp"
#include "../../src/loglik/loglik.hpp"
#include "../../src/logprior/logprior.hpp"
#include "../../src/optimizer/optimizer.hpp"
#include "../../src/hessian/hessian.hpp"
#include "../../src/dmis_proposal/dmis_proposal.hpp"
#include "../../src/importance_weights/importance_weights.hpp"
#include "../../src/psis/psis.hpp"
#include <Eigen/Dense>

using Matrix99 = Eigen::Matrix<double,9,9>;
using Vector6d = Eigen::Matrix<double,6,1>;

class Orch {
public:
    Orch(const Vector6d& y_obs, double sigma_y, const Data& data);

    void run(std::vector<double>& theta_grid, std::vector<double>& log_Z_laplace_grid, std::vector<double>& log_Z_dmis_grid, std::vector<double>& profile_loglik_grid, bool quiet = false);

private:
    LogLik loglik_;
    LogPrior logprior_;
    Opt optimizer_;
    Hess hessian_;
};

#endif // ORCHESTRATION_H
