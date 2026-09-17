#pragma once

#include "fluidsim/core/Vec3.hpp"

#include <cmath>

namespace fluidsim::kernel {

constexpr float pi = 3.14159265358979323846F;

// Standard kernels with compact support h. Input direction is r_i - r_j.
inline float poly6(float distanceSquared, float h) {
    const float hSquared = h * h;
    if (distanceSquared < 0.0F || distanceSquared >= hSquared) {
        return 0.0F;
    }
    const float difference = hSquared - distanceSquared;
    const float h9 = hSquared * hSquared * hSquared * hSquared * h;
    return (315.0F / (64.0F * pi * h9)) * difference * difference * difference;
}

inline Vec3 spikyGradient(const Vec3& displacement, float h) {
    const float distance = length(displacement);
    if (distance <= 1.0e-6F || distance >= h) {
        return {};
    }
    const float h2 = h * h;
    const float h6 = h2 * h2 * h2;
    const float difference = h - distance;
    const float scale = (-45.0F / (pi * h6)) * difference * difference / distance;
    return displacement * scale;
}

inline float viscosityLaplacian(float distance, float h) {
    if (distance < 0.0F || distance >= h) {
        return 0.0F;
    }
    const float h2 = h * h;
    const float h6 = h2 * h2 * h2;
    return (45.0F / (pi * h6)) * (h - distance);
}

} // namespace fluidsim::kernel
