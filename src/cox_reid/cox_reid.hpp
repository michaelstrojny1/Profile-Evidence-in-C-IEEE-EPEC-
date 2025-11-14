#ifndef COX_REID_H
#define COX_REID_H

#include <Eigen/Dense>
#include "../../include/types.hpp"

class CR {
public:
    CR(const Eigen::MatrixXd& fisher_info, const Eigen::VectorXd& reference_nu);

    Eigen::VectorXd orthogonalize(const Eigen::VectorXd& nu) const;

private:
    Eigen::MatrixXd A_;  // Orthogonalization matrix
    Eigen::VectorXd nu_ref_;
};

#endif // COX_REID_H
