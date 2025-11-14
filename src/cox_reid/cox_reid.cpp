#include "cox_reid.hpp"
#include <Eigen/Dense>

CR::CR(const Eigen::MatrixXd& fisher_info, const Eigen::VectorXd& reference_nu)
    : nu_ref_(reference_nu) {
    Eigen::LLT<Eigen::MatrixXd> llt(fisher_info);
    if (llt.info() == Eigen::Success) {
        Eigen::MatrixXd L = llt.matrixL();
        A_ = L.inverse();
    } else {
        A_ = Eigen::MatrixXd::Identity(fisher_info.rows(), fisher_info.cols());
    }
}

Eigen::VectorXd CR::orthogonalize(const Eigen::VectorXd& nu) const {
    return A_ * (nu - nu_ref_);
}
