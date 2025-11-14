#include "psis.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>

PS::Result PS::psis(const std::vector<double>& weights) {
    std::vector<double> sorted_w = weights;
    std::sort(sorted_w.rbegin(), sorted_w.rend());

    size_t n = weights.size();
    size_t tail_start = n / 5;
    if (tail_start == 0) tail_start = 1;

    std::vector<double> tail_w(sorted_w.begin() + tail_start, sorted_w.end());
    std::vector<double> tail_index(tail_w.size());
    for (size_t i = 0; i < tail_w.size(); ++i) {
        tail_index[i] = static_cast<double>(n - tail_start + i + 1) / n;
    }

    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
    for (size_t i = 0; i < tail_w.size(); ++i) {
        double x = std::log(tail_index[i]);
        double y = std::log(tail_w[i]);
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_x2 += x * x;
    }
    size_t m = tail_w.size();
    double slope = (m * sum_xy - sum_x * sum_y) / (m * sum_x2 - sum_x * sum_x);
    double k_raw = -slope;
    double k_hat = std::max(0.0, k_raw);

    std::vector<double> smoothed_weights = weights;
    if (k_hat > 0.7) {
        double threshold_w = sorted_w[tail_start];
        for (auto& w : smoothed_weights) {
            if (w > threshold_w) w = threshold_w;
        }
    }

    double sum_w = std::accumulate(smoothed_weights.begin(), smoothed_weights.end(), 0.0);
    double sum_w2 = 0.0;
    for (double w : smoothed_weights) {
        sum_w2 += w * w;
    }
    double ess = sum_w * sum_w / sum_w2;

    Result res;
    res.smoothed_weights = smoothed_weights;
    res.k_hat = k_hat;
    res.ess = ess;
    return res;
}
