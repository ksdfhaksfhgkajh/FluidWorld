#pragma once

#include "fluidsim/core/Vec3.hpp"

namespace fluidsim {

class AxisAlignedBox {
public:
    AxisAlignedBox(Vec3 minimum, Vec3 maximum);

    void constrain(Vec3& position, Vec3& velocity, float radius,
                   float restitution, float friction) const;

    [[nodiscard]] const Vec3& minimum() const noexcept { return minimum_; }
    [[nodiscard]] const Vec3& maximum() const noexcept { return maximum_; }

private:
    Vec3 minimum_;
    Vec3 maximum_;
};

} // namespace fluidsim
