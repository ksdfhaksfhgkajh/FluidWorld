#include "fluidsim/neighbor/UniformGrid.hpp"

#include <functional>
#include <stdexcept>

namespace fluidsim {

std::size_t CellKeyHash::operator()(const CellKey& key) const noexcept {
    const std::size_t hx = std::hash<int>{}(key.x);
    const std::size_t hy = std::hash<int>{}(key.y);
    const std::size_t hz = std::hash<int>{}(key.z);
    return hx ^ (hy + 0x9e3779b9U + (hx << 6U) + (hx >> 2U)) ^
           (hz + 0x9e3779b9U + (hy << 6U) + (hy >> 2U));
}

UniformGrid::UniformGrid(float cellSize) : cellSize_(cellSize), inverseCellSize_(1.0F) {
    setCellSize(cellSize);
}

void UniformGrid::setCellSize(float cellSize) {
    if (cellSize <= 0.0F) {
        throw std::invalid_argument("FluidWorld: grid cell size must be positive");
    }
    cellSize_ = cellSize;
    inverseCellSize_ = 1.0F / cellSize;
    cells_.clear();
}

void UniformGrid::build(const std::vector<Vec3>& positions) {
    cells_.clear();
    cells_.reserve(positions.size());
    for (std::size_t index = 0; index < positions.size(); ++index) {
        cells_[cellFor(positions[index])].push_back(index);
    }
}

} // namespace fluidsim
