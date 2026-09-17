#include "fluidsim/core/Simulation.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>

int main() {
    fluidsim::SimulationConfig config;
    config.domainMin = {-0.6F, 0.0F, -0.4F};
    config.domainMax = {0.6F, 1.2F, 0.4F};

    fluidsim::Simulation simulation(config);
    const std::size_t count = simulation.addFluidBlock(
        {-0.45F, 0.08F, -0.25F}, {-0.05F, 0.80F, 0.25F}, 0.04F);

    std::cout << "FluidWorld simple WCSPH example\n"
              << "particles: " << count << "\n\n"
              << "step, average_density, maximum_density, maximum_neighbors, min_y, max_y\n";

    constexpr std::size_t totalSteps = 300;
    for (std::size_t step = 0; step <= totalSteps; ++step) {
        if (step % 30 == 0) {
            const auto& particles = simulation.particles();
            float minimumY = particles.positions.front().y;
            float maximumY = minimumY;
            for (const auto& position : particles.positions) {
                minimumY = std::min(minimumY, position.y);
                maximumY = std::max(maximumY, position.y);
            }
            const auto& stats = simulation.statistics();
            std::cout << step << ", " << std::fixed << std::setprecision(2)
                      << stats.averageDensity << ", " << stats.maximumDensity << ", "
                      << stats.maximumNeighborCount << ", " << minimumY << ", "
                      << maximumY << '\n';
        }
        if (step < totalSteps) {
            simulation.step();
        }
    }

    return 0;
}
