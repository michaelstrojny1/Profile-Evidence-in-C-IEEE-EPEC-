#ifndef OBS_MODEL_H
#define OBS_MODEL_H

#include "../../include/types.hpp"
#include <Eigen/Dense>
#include "../../include/ieee14_data.hpp"

class ObsMod {
public:
    ObsMod(const Data& data);
    Eigen::Matrix<double,6,1> observe(const H& theta, const Params& nu, const Discrep& beta) const;

private:
    const Data& data_;
};

#endif // OBS_MODEL_H
