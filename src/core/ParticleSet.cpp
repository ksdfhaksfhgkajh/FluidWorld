#include "fluidsim/core/ParticleSet.hpp"

#include <stdexcept>

namespace fluidsim {

void ParticleSet::reserve(std::size_t capacity) {
    positions.reserve(capacity);
    velocities.reserve(capacity);
    accelerations.reserve(capacity);
    masses.reserve(capacity);
    densities.reserve(capacity);
    pressures.reserve(capacity);
}

void ParticleSet::clear() {
    positions.clear();
    velocities.clear();
    accelerations.clear();
    masses.clear();
    densities.clear();
    pressures.clear();
}

std::size_t ParticleSet::addParticle(const Vec3& position, const Vec3& velocity, float mass) {
    if (mass <= 0.0F) {
        throw std::invalid_argument("FluidWorld: particle mass must be positive");
    }
    const std::size_t index = size();
    positions.push_back(position);
    velocities.push_back(velocity);
    accelerations.emplace_back();
    masses.push_back(mass);
    densities.push_back(0.0F);
    pressures.push_back(0.0F);
    return index;
}

bool ParticleSet::hasConsistentSize() const noexcept {
    const std::size_t count = positions.size();
    return velocities.size() == count && accelerations.size() == count &&
           masses.size() == count && densities.size() == count && pressures.size() == count;
}

} // namespace fluidsim
