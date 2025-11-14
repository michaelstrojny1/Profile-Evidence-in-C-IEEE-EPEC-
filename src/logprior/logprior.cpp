#define _USE_MATH_DEFINES
#include <cmath>
#include "logprior.hpp"

double LogPrior::logprior_theta(const H& theta) const {
    if (theta.val >= 3.0 && theta.val <= 7.0) {
        return -std::log(4.0);
    } else {
        return -std::numeric_limits<double>::infinity();
    }
}

double LogPrior::logprior_nu(const Params& nu) const {
    double logp = 0.0;
    for (int i = 0; i < nu.h.size(); ++i) {
        double mean = 5.0, sigma = 10.0;
        double diff = nu.h(i) - mean;
        logp += -0.5 * diff * diff / (sigma * sigma) - 0.5 * std::log(2 * M_PI * sigma * sigma);
    }
    for (int i = 0; i < nu.d.size(); ++i) {
        double mean = 0.1, sigma = 1.0;
        double diff = nu.d(i) - mean;
        logp += -0.5 * diff * diff / (sigma * sigma) - 0.5 * std::log(2 * M_PI * sigma * sigma);
    }
    return logp;
}

double LogPrior::logprior_beta(const Discrep& beta) const {
    double logp = 0.0;
    for (int i = 0; i < beta.coeffs.size(); ++i) {
        double val = beta.coeffs(i);
        logp += -0.5 * val * val / 4.0 - 0.5 * std::log(2 * M_PI * 4.0);
    }
    return logp;
}
