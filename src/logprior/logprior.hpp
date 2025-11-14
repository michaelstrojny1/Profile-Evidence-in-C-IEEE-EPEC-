#ifndef LOGPRIOR_H
#define LOGPRIOR_H

#include "../../include/types.hpp"

class LogPrior {
public:
    double logprior_theta(const H& theta) const;
    double logprior_nu(const Params& nu) const;
    double logprior_beta(const Discrep& beta) const;
};

#endif // LOGPRIOR_H
