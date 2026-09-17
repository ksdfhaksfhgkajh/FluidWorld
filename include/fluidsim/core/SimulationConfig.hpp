#pragma once

#include "fluidsim/core/Vec3.hpp"

#include <stdexcept>

namespace fluidsim {

struct SimulationConfig {
    float restDensity{1000.0F};
    float particleRadius{0.02F};
    float smoothingRadius{0.08F};
    float gasStiffness{1500.0F};
    float viscosity{0.08F};
    float timeStep{0.001F};
    float restitution{0.25F};
    float boundaryFriction{0.02F};
    float maxVelocity{25.0F};
    Vec3 gravity{0.0F, -9.81F, 0.0F};
    Vec3 domainMin{-1.0F, 0.0F, -1.0F};
    Vec3 domainMax{1.0F, 2.0F, 1.0F};

    void validate() const {
        if (restDensity <= 0.0F || particleRadius <= 0.0F ||
            smoothingRadius <= particleRadius || gasStiffness < 0.0F ||
            viscosity < 0.0F || timeStep <= 0.0F || maxVelocity <= 0.0F) {
            throw std::invalid_argument("FluidWorld: invalid positive simulation parameter");
        }
        if (restitution < 0.0F || restitution > 1.0F ||
            boundaryFriction < 0.0F || boundaryFriction > 1.0F) {
            throw std::invalid_argument("FluidWorld: restitution and friction must be in [0, 1]");
        }
        if (domainMin.x >= domainMax.x || domainMin.y >= domainMax.y ||
            domainMin.z >= domainMax.z) {
            throw std::invalid_argument("FluidWorld: invalid simulation domain");
        }
    }
};

} // namespace fluidsim
