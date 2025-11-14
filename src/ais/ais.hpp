#ifndef AIS_H
#define AIS_H

#include <vector>
#include "../../include/types.hpp"
#include "../loglik/loglik.hpp"
#include "../logprior/logprior.hpp"

class AI {
public:
    AI(const LogLik& loglik, const LogPrior& logprior);

    double estimate_evidence(const H& theta, int n_runs, int n_temperatures);

private:
    const LogLik& loglik_;
    const LogPrior& logprior_;
};

#endif // AIS_H
