#ifndef VECTOR2_H
#define VECTOR2_H

#pragma once

#define TOLERANCE 0.000005
#include <cmath>
#include <string>

namespace emc {
    struct Vector2 {
        float x, y;

        Vector2() : x(0.0f), y(0.0f) {}
        Vector2(const float x, const float y) : x(x), y(y) {}

        [[nodiscard]] std::string ToString() const {
            std::string str;
            for (int i = 0; i < 3; ++i) {
                str += std::to_string((*this)[i]) + ", ";
            }
            return str;
        }

        [[nodiscard]] float Dot(const Vector2& other) const {
            return x * other.x + y * other.y;
        }

        [[nodiscard]] float Magnitude() const { return std::sqrt(x * x + y * y); }

		explicit operator float* () { return &x; }
        explicit operator const float* () const { return &x; }

        void Normalise() {
            const float mag = this->Magnitude();
            if (mag == 0.0f) { return; }
            x /= mag;
            y /= mag;
        }

        [[nodiscard]] Vector2 Normalised() const {
            const float mag = this->Magnitude();
            if (mag == 0.0f) { return {0.0f, 0.0f}; }
            return { x / mag, y / mag};
        }

    	Vector2 operator+(const Vector2& other) const { return { x + other.x, y + other.y }; }

        Vector2 operator-(const Vector2& other) const { return { x - other.x, y - other.y }; }

        Vector2 operator*(const float scale) const { return { x * scale, y * scale }; }

        Vector2 operator/(const float scale) const { return { x / scale, y / scale }; }

        bool operator==(const Vector2& other) const {
	        return (abs(x - other.x) < TOLERANCE && abs(y - other.y) < TOLERANCE);
        }

        bool operator!=(const Vector2& other) const {
            return !(*this == other);
        }

        float& operator[](const size_t value) {
            return (&x)[value];
        }

        const float& operator[](const size_t value) const {
            return (&x)[value];
        }
	};

    Vector2 operator*(float scale, const Vector2& other);

    inline Vector2 operator*(const float scale, const Vector2& other) {
        return { scale * other.x, scale * other.y};
    }
}

#endif //VECTOR2_H
