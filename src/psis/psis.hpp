#ifndef PSIS_H
#define PSIS_H

#include <vector>

class PS {
public:
    struct Result {
        std::vector<double> smoothed_weights;
        double k_hat;
        double ess;
    };

    static Result psis(const std::vector<double>& weights);
};

#endif // PSIS_H
