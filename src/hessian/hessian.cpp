#define _USE_MATH_DEFINES
#include <cmath>
#include "hessian.hpp"
#include <vector>

static double obj(const double* x, const H* theta, const LogLik* loglik, const LogPrior* logprior, int n_nu, int n_beta) {
    Params nu;
    nu.h = Eigen::VectorXd::Map(x, 4);
    nu.d = Eigen::VectorXd::Map(x + 4, 5);
    Discrep beta;
    if (n_beta > 0) {
        beta.coeffs = Eigen::VectorXd::Map(x + 9, n_beta);
    } else {
        beta.coeffs = Eigen::VectorXd::Zero(0);
    }
    double logpost = loglik->loglik(*theta, nu, beta) + logprior->logprior_nu(nu);
    if (n_beta > 0) logpost += logprior->logprior_beta(beta);
    return -logpost;
}

Eigen::MatrixXd Hess::compute_hessian(const H& theta, const Params& nu_hat, const Discrep& beta_hat) {
    int n_nu = 9;
    int n_beta = beta_hat.coeffs.size();
    int n = n_nu + n_beta;
    Eigen::MatrixXd H = Eigen::MatrixXd::Zero(n, n);
    double eps = 1e-7;
    std::vector<double> x(n);
    for (int i = 0; i < 4; ++i) x[i] = nu_hat.h(i);
    for (int i = 0; i < 5; ++i) x[i+4] = nu_hat.d(i);
    for (int i = 0; i < n_beta; ++i) x[i+9] = beta_hat.coeffs(i);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::vector<double> x_pp = x, x_pm = x, x_mp = x, x_mm = x;
            x_pp[i] += eps; x_pp[j] += eps;
            x_pm[i] += eps; x_pm[j] -= eps;
            x_mp[i] -= eps; x_mp[j] += eps;
            x_mm[i] -= eps; x_mm[j] -= eps;

            double f_pp = obj(x_pp.data(), &theta, &loglik_, &logprior_, n_nu, n_beta);
            double f_pm = obj(x_pm.data(), &theta, &loglik_, &logprior_, n_nu, n_beta);
            double f_mp = obj(x_mp.data(), &theta, &loglik_, &logprior_, n_nu, n_beta);
            double f_mm = obj(x_mm.data(), &theta, &loglik_, &logprior_, n_nu, n_beta);

            H(i,j) = (f_pp - f_pm - f_mp + f_mm) / (4 * eps * eps);
        }
    }
    return H;
}

Hess::Hess(const LogLik& loglik, const LogPrior& logprior)
    : loglik_(loglik), logprior_(logprior) {}
