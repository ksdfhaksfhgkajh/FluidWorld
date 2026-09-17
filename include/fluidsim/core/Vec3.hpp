#pragma once

#include <cmath>
#include <ostream>

namespace fluidsim {

struct Vec3 {
    float x{0.0F};
    float y{0.0F};
    float z{0.0F};

    constexpr Vec3() = default;
    constexpr Vec3(float xValue, float yValue, float zValue)
        : x(xValue), y(yValue), z(zValue) {}

    constexpr Vec3 operator+() const { return *this; }
    constexpr Vec3 operator-() const { return {-x, -y, -z}; }

    constexpr Vec3& operator+=(const Vec3& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    constexpr Vec3& operator-=(const Vec3& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    constexpr Vec3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    constexpr Vec3& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
};

constexpr Vec3 operator+(Vec3 lhs, const Vec3& rhs) { return lhs += rhs; }
constexpr Vec3 operator-(Vec3 lhs, const Vec3& rhs) { return lhs -= rhs; }
constexpr Vec3 operator*(Vec3 value, float scalar) { return value *= scalar; }
constexpr Vec3 operator*(float scalar, Vec3 value) { return value *= scalar; }
constexpr Vec3 operator/(Vec3 value, float scalar) { return value /= scalar; }

constexpr float dot(const Vec3& lhs, const Vec3& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

constexpr float lengthSquared(const Vec3& value) { return dot(value, value); }
inline float length(const Vec3& value) { return std::sqrt(lengthSquared(value)); }

inline Vec3 normalized(const Vec3& value) {
    const float magnitude = length(value);
    return magnitude > 0.0F ? value / magnitude : Vec3{};
}

inline bool isFinite(const Vec3& value) {
    return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z);
}

inline std::ostream& operator<<(std::ostream& output, const Vec3& value) {
    return output << '(' << value.x << ", " << value.y << ", " << value.z << ')';
}

} // namespace fluidsim
