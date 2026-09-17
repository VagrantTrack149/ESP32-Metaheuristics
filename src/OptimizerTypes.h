#pragma once

#include <vector>
#include <functional>

namespace bio {

using ObjectiveFunction = std::function<float(const std::vector<float>&)>;

struct Bounds {
    std::vector<float> lower;
    std::vector<float> upper;

    Bounds() = default;
    Bounds(const std::vector<float>& lb, const std::vector<float>& ub)
        : lower(lb), upper(ub) {}

    static Bounds uniforme(int dim, float minVal, float maxVal) {
        Bounds bounds;
        bounds.lower.assign(dim, minVal);
        bounds.upper.assign(dim, maxVal);
        return bounds;
    }

    size_t dim() const { return lower.size(); }
};

struct Agente {
    std::vector<float> posicion;
    float fitness = 0.0f;
};

}
