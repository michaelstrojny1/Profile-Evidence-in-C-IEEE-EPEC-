#define _USE_MATH_DEFINES
#include <cmath>
#include "optimizer.hpp"
#include <iostream>

double obj(const double* x, const H* theta, const LogLik* loglik, const LogPrior* logprior, int n_nu, int n_beta, bool profile_likelihood) {
    Params nu;
    nu.h = Eigen::VectorXd::Map(x, 4);
    nu.d = Eigen::VectorXd::Map(x + 4, 5);
    Discrep beta;
    if (n_beta > 0) {
        beta.coeffs = Eigen::VectorXd::Map(x + 9, n_beta);
    } else {
        beta.coeffs = Eigen::VectorXd::Zero(0);
    }
    double loglik_val = loglik->loglik(*theta, nu, beta);
    double logpost = loglik_val;
    if (!profile_likelihood) {
        logpost += logprior->logprior_nu(nu);
        if (n_beta > 0) logpost += logprior->logprior_beta(beta);
    }
    return -logpost;
}

Eigen::VectorXd grad(const double* x, const H* theta, const LogLik* loglik, const LogPrior* logprior, int n_nu, int n_beta, bool profile_likelihood, double eps = 1e-7) {
    int n = n_nu + n_beta;
    Eigen::VectorXd g(n);
    double f0 = obj(x, theta, loglik, logprior, n_nu, n_beta, profile_likelihood);
    std::vector<double> x_pert(x, x + n);
    for (int i = 0; i < n; ++i) {
        x_pert[i] += eps;
        double f1 = obj(x_pert.data(), theta, loglik, logprior, n_nu, n_beta, profile_likelihood);
        g(i) = (f1 - f0) / eps;
        x_pert[i] -= eps;
    }
    return g;
}

ParamsDiscrep Opt::optimize_nu_beta(const H& theta, bool profile_likelihood) const {
    int n_nu = 9;
    int n_beta = profile_likelihood ? 0 : 6;
    int n = n_nu + n_beta;
    std::vector<double> x(n, 0.0);
    for (int i = 0; i < 4; ++i) x[i] = std::log(5.0);
    for (int i = 0; i < 5; ++i) x[i+4] = std::log(0.1);
    double alpha = 0.01;
    int max_iter = 500;
    for (int iter = 0; iter < max_iter; ++iter) {
        Eigen::VectorXd grad_val = grad(x.data(), &theta, &loglik_, &logprior_, n_nu, n_beta, profile_likelihood);
        for (int i = 0; i < n; ++i) {
            x[i] -= alpha * grad_val(i);
        }
    }
    ParamsDiscrep nubeta;
    for (int i = 0; i < 4; ++i) nubeta.p.h(i) = std::exp(x[i]);
    for (int i = 0; i < 5; ++i) nubeta.p.d(i) = std::exp(x[i+4]);
    for (int i = 0; i < 6; ++i) nubeta.d.coeffs(i) = (n_beta > 0) ? x[i+9] : 0.0;
    return nubeta;
}

Opt::Opt(const LogLik& loglik, const LogPrior& logprior)
    : loglik_(loglik), logprior_(logprior) {}

Params Opt::optimize_nu(const H& theta) const {
    return optimize_nu_beta(theta, true).p;
}
