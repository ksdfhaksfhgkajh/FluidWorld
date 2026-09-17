#include "fluidsim/solver/WCSPHSolver.hpp"

#include "fluidsim/boundary/AxisAlignedBox.hpp"
#include "fluidsim/core/ParticleSet.hpp"
#include "fluidsim/core/SimulationConfig.hpp"
#include "fluidsim/kernel/SPHKernels.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace fluidsim {

WCSPHSolver::WCSPHSolver(float smoothingRadius) : grid_(smoothingRadius) {}

void WCSPHSolver::step(ParticleSet& particles, const SimulationConfig& config,
                       const AxisAlignedBox& boundary) {
    if (!particles.hasConsistentSize()) {
        throw std::logic_error("FluidWorld: particle buffers have inconsistent sizes");
    }
    if (particles.empty()) {
        statistics_ = {};
        return;
    }

    if (std::abs(grid_.cellSize() - config.smoothingRadius) > 1.0e-6F) {
        grid_.setCellSize(config.smoothingRadius);
    }
    grid_.build(particles.positions);
    computeDensitiesAndPressures(particles, config);
    computeAccelerations(particles, config);
    integrate(particles, config, boundary);
}

void WCSPHSolver::computeDensitiesAndPressures(ParticleSet& particles,
                                                const SimulationConfig& config) {
    const float h = config.smoothingRadius;
    const float hSquared = h * h;
    float densitySum = 0.0F;
    float maximumDensity = 0.0F;
    std::size_t maximumNeighborCount = 0;

    for (std::size_t i = 0; i < particles.size(); ++i) {
        float density = 0.0F;
        std::size_t neighborCount = 0;
        grid_.forEachCandidate(particles.positions[i], [&](std::size_t j) {
            const Vec3 displacement = particles.positions[i] - particles.positions[j];
            const float distanceSquared = lengthSquared(displacement);
            if (distanceSquared < hSquared) {
                density += particles.masses[j] * kernel::poly6(distanceSquared, h);
                ++neighborCount;
            }
        });

        density = std::max(density, config.restDensity * 0.01F);
        particles.densities[i] = density;
        // Clamp negative pressure in the first implementation to avoid tensile clumping.
        particles.pressures[i] = config.gasStiffness *
                                  std::max(density - config.restDensity, 0.0F);
        densitySum += density;
        maximumDensity = std::max(maximumDensity, density);
        maximumNeighborCount = std::max(maximumNeighborCount, neighborCount);
    }

    statistics_.averageDensity = densitySum / static_cast<float>(particles.size());
    statistics_.maximumDensity = maximumDensity;
    statistics_.maximumNeighborCount = maximumNeighborCount;
}

void WCSPHSolver::computeAccelerations(ParticleSet& particles,
                                        const SimulationConfig& config) {
    const float h = config.smoothingRadius;
    const float hSquared = h * h;

    for (std::size_t i = 0; i < particles.size(); ++i) {
        Vec3 pressureAcceleration{};
        Vec3 viscosityAcceleration{};

        grid_.forEachCandidate(particles.positions[i], [&](std::size_t j) {
            if (i == j) {
                return;
            }
            const Vec3 displacement = particles.positions[i] - particles.positions[j];
            const float distanceSquared = lengthSquared(displacement);
            if (distanceSquared >= hSquared || distanceSquared <= 1.0e-12F) {
                return;
            }

            const float densityI = particles.densities[i];
            const float densityJ = particles.densities[j];
            const float symmetricPressure = particles.pressures[i] / (densityI * densityI) +
                                            particles.pressures[j] / (densityJ * densityJ);
            pressureAcceleration -= particles.masses[j] * symmetricPressure *
                                    kernel::spikyGradient(displacement, h);

            const float distance = std::sqrt(distanceSquared);
            viscosityAcceleration += config.viscosity * particles.masses[j] *
                                     (particles.velocities[j] - particles.velocities[i]) /
                                     densityJ * kernel::viscosityLaplacian(distance, h);
        });

        particles.accelerations[i] = config.gravity + pressureAcceleration +
                                     viscosityAcceleration;
    }
}

void WCSPHSolver::integrate(ParticleSet& particles, const SimulationConfig& config,
                            const AxisAlignedBox& boundary) const {
    for (std::size_t i = 0; i < particles.size(); ++i) {
        particles.velocities[i] += particles.accelerations[i] * config.timeStep;
        const float speedSquared = lengthSquared(particles.velocities[i]);
        const float maximumSpeedSquared = config.maxVelocity * config.maxVelocity;
        if (speedSquared > maximumSpeedSquared) {
            particles.velocities[i] *= config.maxVelocity / std::sqrt(speedSquared);
        }
        particles.positions[i] += particles.velocities[i] * config.timeStep;
        boundary.constrain(particles.positions[i], particles.velocities[i],
                           config.particleRadius, config.restitution,
                           config.boundaryFriction);
    }
}

} // namespace fluidsim
