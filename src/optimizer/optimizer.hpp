#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "../../include/types.hpp"
#include "../../src/loglik/loglik.hpp"
#include "../../src/logprior/logprior.hpp"

class Opt {
public:
    Opt(const LogLik& loglik, const LogPrior& logprior);

    Params optimize_nu(const H& theta) const;
    ParamsDiscrep optimize_nu_beta(const H& theta, bool profile_likelihood = false) const;

private:
    const LogLik& loglik_;
    const LogPrior& logprior_;
};

#endif // OPTIMIZER_H
