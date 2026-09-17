#pragma once

#include "fluidsim/boundary/AxisAlignedBox.hpp"
#include "fluidsim/core/ParticleSet.hpp"
#include "fluidsim/core/SimulationConfig.hpp"
#include "fluidsim/solver/WCSPHSolver.hpp"

#include <cstddef>

namespace fluidsim {

class Simulation {
public:
    explicit Simulation(SimulationConfig config = {});

    std::size_t addFluidBlock(const Vec3& minimum, const Vec3& maximum,
                              float spacing, const Vec3& initialVelocity = {});
    void step();
    void step(std::size_t substeps);

    [[nodiscard]] ParticleSet& particles() noexcept { return particles_; }
    [[nodiscard]] const ParticleSet& particles() const noexcept { return particles_; }
    [[nodiscard]] const SimulationConfig& config() const noexcept { return config_; }
    [[nodiscard]] const WCSPHStatistics& statistics() const noexcept {
        return solver_.statistics();
    }

private:
    SimulationConfig config_;
    ParticleSet particles_;
    AxisAlignedBox boundary_;
    WCSPHSolver solver_;
};

} // namespace fluidsim
