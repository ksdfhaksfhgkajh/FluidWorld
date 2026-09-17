#pragma once

#include "fluidsim/core/Vec3.hpp"

#include <cmath>
#include <cstddef>
#include <unordered_map>
#include <vector>

namespace fluidsim {

struct CellKey {
    int x;
    int y;
    int z;

    bool operator==(const CellKey& rhs) const noexcept {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }
};

struct CellKeyHash {
    std::size_t operator()(const CellKey& key) const noexcept;
};

class UniformGrid {
public:
    explicit UniformGrid(float cellSize);

    void setCellSize(float cellSize);
    void build(const std::vector<Vec3>& positions);

    [[nodiscard]] float cellSize() const noexcept { return cellSize_; }
    [[nodiscard]] std::size_t cellCount() const noexcept { return cells_.size(); }

    template <typename Callback>
    void forEachCandidate(const Vec3& position, Callback&& callback) const {
        const CellKey center = cellFor(position);
        for (int dz = -1; dz <= 1; ++dz) {
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    const CellKey key{center.x + dx, center.y + dy, center.z + dz};
                    const auto cell = cells_.find(key);
                    if (cell == cells_.end()) {
                        continue;
                    }
                    for (const std::size_t particleIndex : cell->second) {
                        callback(particleIndex);
                    }
                }
            }
        }
    }

private:
    [[nodiscard]] CellKey cellFor(const Vec3& position) const noexcept {
        return {
            static_cast<int>(std::floor(position.x * inverseCellSize_)),
            static_cast<int>(std::floor(position.y * inverseCellSize_)),
            static_cast<int>(std::floor(position.z * inverseCellSize_)),
        };
    }

    float cellSize_;
    float inverseCellSize_;
    std::unordered_map<CellKey, std::vector<std::size_t>, CellKeyHash> cells_;
};

} // namespace fluidsim
