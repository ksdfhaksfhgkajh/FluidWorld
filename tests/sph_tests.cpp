#include "fluidsim/core/Simulation.hpp"
#include "fluidsim/kernel/SPHKernels.hpp"
#include "fluidsim/neighbor/UniformGrid.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace {

int failures = 0;

void check(bool condition, const std::string& message) {
    if (!condition) {
        ++failures;
        std::cerr << "[FAIL] " << message << '\n';
    }
}

bool nearlyEqual(float lhs, float rhs, float epsilon = 1.0e-5F) {
    return std::abs(lhs - rhs) <= epsilon;
}

void testKernels() {
    constexpr float h = 1.0F;
    check(fluidsim::kernel::poly6(0.0F, h) > 0.0F,
          "poly6 is positive at the origin");
    check(nearlyEqual(fluidsim::kernel::poly6(h * h, h), 0.0F),
          "poly6 has compact support");

    const fluidsim::Vec3 displacement{0.25F, -0.1F, 0.05F};
    const fluidsim::Vec3 forward = fluidsim::kernel::spikyGradient(displacement, h);
    const fluidsim::Vec3 backward = fluidsim::kernel::spikyGradient(-displacement, h);
    check(nearlyEqual(forward.x, -backward.x) &&
              nearlyEqual(forward.y, -backward.y) &&
              nearlyEqual(forward.z, -backward.z),
          "spiky gradient is antisymmetric");
}

void testUniformGrid() {
    const std::vector<fluidsim::Vec3> positions{
        {0.0F, 0.0F, 0.0F}, {0.8F, 0.0F, 0.0F}, {3.0F, 0.0F, 0.0F}};
    fluidsim::UniformGrid grid(1.0F);
    grid.build(positions);

    std::vector<std::size_t> candidates;
    grid.forEachCandidate(positions[0], [&](std::size_t index) {
        candidates.push_back(index);
    });
    check(std::find(candidates.begin(), candidates.end(), 0U) != candidates.end(),
          "grid query includes the source cell");
    check(std::find(candidates.begin(), candidates.end(), 1U) != candidates.end(),
          "grid query includes adjacent cells");
    check(std::find(candidates.begin(), candidates.end(), 2U) == candidates.end(),
          "grid query excludes distant cells");
}

void testSimulationStaysFiniteAndBounded() {
    fluidsim::SimulationConfig config;
    config.domainMin = {-0.4F, 0.0F, -0.4F};
    config.domainMax = {0.4F, 0.8F, 0.4F};
    fluidsim::Simulation simulation(config);
    simulation.addFluidBlock({-0.2F, 0.1F, -0.2F},
                             {0.2F, 0.5F, 0.2F}, 0.04F);
    simulation.step(100);

    check(simulation.particles().hasConsistentSize(),
          "particle SoA buffers keep matching sizes");
    check(simulation.statistics().averageDensity > 0.0F,
          "solver reports a positive average density");

    for (const auto& position : simulation.particles().positions) {
        check(fluidsim::isFinite(position), "particle position stays finite");
        check(position.x >= config.domainMin.x + config.particleRadius - 1.0e-5F &&
                  position.x <= config.domainMax.x - config.particleRadius + 1.0e-5F &&
                  position.y >= config.domainMin.y + config.particleRadius - 1.0e-5F &&
                  position.y <= config.domainMax.y - config.particleRadius + 1.0e-5F &&
                  position.z >= config.domainMin.z + config.particleRadius - 1.0e-5F &&
                  position.z <= config.domainMax.z - config.particleRadius + 1.0e-5F,
              "particle remains inside the domain");
    }
}

} // namespace

int main() {
    testKernels();
    testUniformGrid();
    testSimulationStaysFiniteAndBounded();

    if (failures != 0) {
        std::cerr << failures << " test assertion(s) failed\n";
        return EXIT_FAILURE;
    }
    std::cout << "All FluidWorld tests passed\n";
    return EXIT_SUCCESS;
}
