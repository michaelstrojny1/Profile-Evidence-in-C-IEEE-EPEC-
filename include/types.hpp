#ifndef TYPES_H
#define TYPES_H

#include <Eigen/Dense>

using Eigen::Vector4d;
using Vector5d = Eigen::Matrix<double,5,1>;
using Vector6d = Eigen::Matrix<double,6,1>;
using Vector9d = Eigen::Matrix<double,9,1>;
using Vector10d = Eigen::Matrix<double,10,1>;

struct H {
    double val;
};

struct Params {
    Vector4d h;
    Vector5d d;
};

struct Discrep {
    Vector6d coeffs;
};

struct ParamsDiscrep {
    Params p;
    Discrep d;
};

#endif // TYPES_H
