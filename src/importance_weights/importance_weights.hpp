#ifndef IMPORTANCE_WEIGHTS_H
#define IMPORTANCE_WEIGHTS_H

#include <vector>
#include <random>
#include <Eigen/Dense>
#include "../../include/types.hpp"
#include "../../src/dmis_proposal/dmis_proposal.hpp"
#include "../../src/loglik/loglik.hpp"
#include "../../src/logprior/logprior.hpp"

class IW {
public:
    IW(const H& theta, const DMISProp& proposal, const LogLik& loglik, const LogPrior& logprior);
    std::vector<double> compute_weights(size_t N);

private:
    const H& theta_;
    const DMISProp& proposal_;
    const LogLik& loglik_;
    const LogPrior& logprior_;
};

#endif // IMPORTANCE_WEIGHTS_H
