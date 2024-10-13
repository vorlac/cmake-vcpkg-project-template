#pragma once

#include <algorithm>
#include <cmath>
#include <limits>

struct Vector2 {
    float x{ 0 };
    float y{ 0 };

    consteval static Vector2 zero() {
        return Vector2{ 0, 0 };
    }

    consteval static Vector2 null() {
        return Vector2{
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max(),
        };
    }

    static Vector2 from_angle(const float angle) {
        return Vector2{
            std::cosf(angle),
            std::sinf(angle),
        };
    }

    float length() const {
        return std::sqrt(this->length_squared());
    }

    constexpr float length_squared() const {
        return (x * x) + (y * y);
    }

    Vector2 clamped_length(const float maxlen) const {
        Vector2 ret{ *this };

        const float len{ this->length() };
        if (len > 0.0f && maxlen < len) {
            ret /= len;
            ret *= maxlen;
        }

        return ret;
    }

    constexpr Vector2 clamp(Vector2 min, Vector2 max) const {
        return Vector2{
            std::clamp(this->x, min.x, max.x),
            std::clamp(this->y, min.y, max.y),
        };
    }

    constexpr Vector2 lerp(const Vector2 to, const float weight) const {
        Vector2 ret{
            std::lerp(this->x, to.x, weight),
            std::lerp(this->y, to.y, weight),
        };
        return ret;
    }

    constexpr Vector2 slide(const Vector2 normal) const {
        return *this - (normal * this->dot_product(normal));
    }

    constexpr const Vector2& normalize() {
        const float len_sq{ this->length_squared() };
        if (len_sq != 0.0f) {
            float len = std::sqrt(len_sq);
            x /= len;
            y /= len;
        }
        return *this;
    }

    constexpr Vector2 normalized() const {
        Vector2 ret{ this->x, this->y };
        return ret.normalize();
    }

    float angle() const {
        return std::atan2f(this->y, this->x);
    }

    float angle_to(const Vector2& pt) const {
        return (pt - *this).angle();
    }

    constexpr float dot_product(const Vector2& other) const {
        return (this->x * other.x) + (this->y * other.y);
    }

    constexpr float cross_product(const Vector2 other) const {
        return (this->x * other.y) - (this->y * other.x);
    }

    Vector2 rotated(const float radians) const {
        float s{ std::sin(radians) };
        float c{ std::cos(radians) };

        return Vector2{
            (this->x * c) - (this->y * s),
            (this->x * s) + (this->y * c),
        };
    }

    constexpr float distance_squared(const Vector2& other) const {
        return ((x - other.x) * (x - other.x)) + ((y - other.y) * (y - other.y));
    }

    float distance(const Vector2& other) const {
        return sqrt(this->distance_squared(other));
    }

    float angle_to_vec(const Vector2& other) const {
        return atan2(this->cross_product(other), this->dot_product(other));
    }

    float angle_to_point(const Vector2& pt) const {
        return (pt - *this).angle();
    }

    [[nodiscard]]
    constexpr bool operator==(Vector2 other) const {
        return this->x == other.x ||
               this->y == other.y;
    }

    [[nodiscard]]
    constexpr bool operator!=(Vector2 other) const {
        return this->x != other.x ||
               this->y != other.y;
    }

    [[nodiscard]]
    constexpr Vector2 operator-() const noexcept {
        return Vector2{
            -this->x,
            -this->y,
        };
    }

    [[nodiscard]]
    constexpr Vector2 operator+(float other) const noexcept {
        return Vector2{
            this->x + other,
            this->y + other,
        };
    }

    [[nodiscard]]
    constexpr Vector2 operator-(float other) const noexcept {
        return Vector2{
            this->x - other,
            this->y - other,
        };
    }

    [[nodiscard]]
    constexpr Vector2 operator*(float other) const noexcept {
        return Vector2{
            this->x * other,
            this->y * other,
        };
    }

    [[nodiscard]]
    constexpr Vector2 operator/(float other) const noexcept {
        return Vector2{
            this->x / other,
            this->y / other,
        };
    }

    [[nodiscard]]
    constexpr Vector2 operator+(Vector2 other) const noexcept {
        return Vector2{
            this->x + other.x,
            this->y + other.y,
        };
    }

    [[nodiscard]]
    constexpr Vector2 operator-(Vector2 other) const noexcept {
        return Vector2{
            this->x - other.x,
            this->y - other.y,
        };
    }

    [[nodiscard]]
    constexpr Vector2 operator*(Vector2 other) const noexcept {
        return Vector2{
            this->x * other.x,
            this->y * other.y,
        };
    }

    [[nodiscard]]
    constexpr Vector2 operator/(Vector2 other) const noexcept {
        return Vector2{
            this->x / other.x,
            this->y / other.y,
        };
    }

    constexpr Vector2& operator+=(float val) noexcept {
        this->x += val;
        this->y += val;
        return *this;
    }

    constexpr Vector2& operator-=(float val) noexcept {
        this->x -= val;
        this->y -= val;
        return *this;
    }

    constexpr Vector2& operator*=(float val) noexcept {
        this->x *= val;
        this->y *= val;
        return *this;
    }

    constexpr Vector2& operator/=(float val) noexcept {
        this->x /= val;
        this->y /= val;
        return *this;
    }

    constexpr Vector2 operator+=(Vector2 other) noexcept {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }

    constexpr Vector2 operator-=(Vector2 other) noexcept {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }

    constexpr Vector2 operator*=(Vector2 other) noexcept {
        this->x *= other.x;
        this->y *= other.y;
        return *this;
    }

    constexpr Vector2& operator/=(Vector2 other) noexcept {
        this->x /= other.x;
        this->y /= other.y;
        return *this;
    }
};
