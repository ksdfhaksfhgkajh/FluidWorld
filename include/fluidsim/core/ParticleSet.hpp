#pragma once

#include "fluidsim/core/Vec3.hpp"

#include <cstddef>
#include <vector>

namespace fluidsim {

// Structure-of-arrays layout. It maps directly to independent CUDA buffers later.
class ParticleSet {
public:
    void reserve(std::size_t capacity);
    void clear();
    std::size_t addParticle(const Vec3& position, const Vec3& velocity, float mass);

    [[nodiscard]] std::size_t size() const noexcept { return positions.size(); }
    [[nodiscard]] bool empty() const noexcept { return positions.empty(); }
    [[nodiscard]] bool hasConsistentSize() const noexcept;

    std::vector<Vec3> positions;
    std::vector<Vec3> velocities;
    std::vector<Vec3> accelerations;
    std::vector<float> masses;
    std::vector<float> densities;
    std::vector<float> pressures;
};

} // namespace fluidsim
