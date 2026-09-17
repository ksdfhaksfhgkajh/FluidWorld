#pragma once

#include "fluidsim/neighbor/UniformGrid.hpp"

#include <cstddef>

namespace fluidsim {

class AxisAlignedBox;
class ParticleSet;
struct SimulationConfig;

struct WCSPHStatistics {
    float averageDensity{0.0F};
    float maximumDensity{0.0F};
    std::size_t maximumNeighborCount{0};
};

class WCSPHSolver {
public:
    explicit WCSPHSolver(float smoothingRadius);

    void step(ParticleSet& particles, const SimulationConfig& config,
              const AxisAlignedBox& boundary);

    [[nodiscard]] const WCSPHStatistics& statistics() const noexcept { return statistics_; }

private:
    void computeDensitiesAndPressures(ParticleSet& particles,
                                      const SimulationConfig& config);
    void computeAccelerations(ParticleSet& particles,
                              const SimulationConfig& config);
    void integrate(ParticleSet& particles, const SimulationConfig& config,
                   const AxisAlignedBox& boundary) const;

    UniformGrid grid_;
    WCSPHStatistics statistics_;
};

} // namespace fluidsim
