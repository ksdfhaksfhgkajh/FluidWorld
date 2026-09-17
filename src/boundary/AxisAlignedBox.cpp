#include "fluidsim/boundary/AxisAlignedBox.hpp"

#include <stdexcept>

namespace fluidsim {
namespace {

void constrainAxis(float& position, float& normalVelocity,
                   float& tangentVelocityA, float& tangentVelocityB,
                   float minimum, float maximum,
                   float restitution, float friction) {
    bool collided = false;
    if (position < minimum) {
        position = minimum;
        if (normalVelocity < 0.0F) {
            normalVelocity = -normalVelocity * restitution;
        }
        collided = true;
    } else if (position > maximum) {
        position = maximum;
        if (normalVelocity > 0.0F) {
            normalVelocity = -normalVelocity * restitution;
        }
        collided = true;
    }

    if (collided) {
        const float tangentScale = 1.0F - friction;
        tangentVelocityA *= tangentScale;
        tangentVelocityB *= tangentScale;
    }
}

} // namespace

AxisAlignedBox::AxisAlignedBox(Vec3 minimum, Vec3 maximum)
    : minimum_(minimum), maximum_(maximum) {
    if (minimum.x >= maximum.x || minimum.y >= maximum.y || minimum.z >= maximum.z) {
        throw std::invalid_argument("FluidWorld: invalid axis-aligned boundary");
    }
}

void AxisAlignedBox::constrain(Vec3& position, Vec3& velocity, float radius,
                               float restitution, float friction) const {
    constrainAxis(position.x, velocity.x, velocity.y, velocity.z,
                  minimum_.x + radius, maximum_.x - radius, restitution, friction);
    constrainAxis(position.y, velocity.y, velocity.x, velocity.z,
                  minimum_.y + radius, maximum_.y - radius, restitution, friction);
    constrainAxis(position.z, velocity.z, velocity.x, velocity.y,
                  minimum_.z + radius, maximum_.z - radius, restitution, friction);
}

} // namespace fluidsim
