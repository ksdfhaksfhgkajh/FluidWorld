#include "fluidsim/core/Simulation.hpp"

#include <stdexcept>
#include <utility>

namespace fluidsim {

Simulation::Simulation(SimulationConfig config)
    : config_(std::move(config)),
      boundary_(config_.domainMin, config_.domainMax),
      solver_(config_.smoothingRadius) {
    config_.validate();
}

std::size_t Simulation::addFluidBlock(const Vec3& minimum, const Vec3& maximum,
                                      float spacing, const Vec3& initialVelocity) {
    if (spacing <= 0.0F || minimum.x > maximum.x || minimum.y > maximum.y ||
        minimum.z > maximum.z) {
        throw std::invalid_argument("FluidWorld: invalid fluid block");
    }

    const float particleMass = config_.restDensity * spacing * spacing * spacing;
    const std::size_t previousCount = particles_.size();
    const float epsilon = spacing * 0.25F;

    for (float z = minimum.z; z <= maximum.z + epsilon; z += spacing) {
        for (float y = minimum.y; y <= maximum.y + epsilon; y += spacing) {
            for (float x = minimum.x; x <= maximum.x + epsilon; x += spacing) {
                particles_.addParticle({x, y, z}, initialVelocity, particleMass);
            }
        }
    }
    return particles_.size() - previousCount;
}

void Simulation::step() {
    solver_.step(particles_, config_, boundary_);
}

void Simulation::step(std::size_t substeps) {
    for (std::size_t i = 0; i < substeps; ++i) {
        step();
    }
}

} // namespace fluidsim
