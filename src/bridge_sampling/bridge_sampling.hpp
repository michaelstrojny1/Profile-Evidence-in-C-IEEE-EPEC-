#ifndef BRIDGE_SAMPLING_H
#define BRIDGE_SAMPLING_H

#include <vector>
#include <Eigen/Dense>
#include "../../include/types.hpp"
#include "../loglik/loglik.hpp"
#include "../logprior/logprior.hpp"
#include "../dmis_proposal/dmis_proposal.hpp"

class BS {
public:
    BS(const std::vector<Eigen::VectorXd>& posterior_samples,
                   const std::vector<Eigen::VectorXd>& proposal_samples,
                   const LogLik& loglik, const LogPrior& logprior,
                   const DMISProp& proposal);

    double estimate_evidence(const H& theta);

private:
    const std::vector<Eigen::VectorXd>& posterior_samples_;
    const std::vector<Eigen::VectorXd>& proposal_samples_;
    const LogLik& loglik_;
    const LogPrior& logprior_;
    const DMISProp& proposal_;

    double log_tilde_pi(const Eigen::VectorXd& nu_beta, const H& theta);
};

#endif // BRIDGE_SAMPLING_H
