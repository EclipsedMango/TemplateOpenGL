#ifndef VECTOR4_H
#define VECTOR4_H

#pragma once

#define TOLERANCE 0.000005

#include <cmath>

namespace emc {
    struct Vector4 {
        float x, y, z, w;

    	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
        Vector4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}

        [[nodiscard]] std::string ToString() const {
            std::string str;
            for (int i = 0; i < 4; ++i) {
                str += std::to_string((*this)[i]) + ", ";
            }
            return str;
        }

        [[nodiscard]] float Dot(const Vector4& other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        [[nodiscard]] float Magnitude() const { return std::sqrt(x * x + y * y + z * z + w * w); }

        explicit operator float* () { return &x; }
        explicit operator const float* () const { return &x; }

        void Normalise() {
            const float mag = this->Magnitude();
            if (mag == 0.0f) { return; }
            x /= mag;
            y /= mag;
            z /= mag;
            w /= mag;
        }

        [[nodiscard]] Vector4 Normalised() const {
            const float mag = this->Magnitude();
			if (mag == 0.0f) { return {0.0f, 0.0f, 0.0f, w}; }
            return { x / mag, y / mag, z / mag, w / mag};
        }

        [[nodiscard]] Vector4 Cross(const Vector4& other) const {
            return { (y * other.z - z * other.y), (z * other.x - x * other.z), (x * other.y - other.x * y), 0.0f };
        }

        Vector4 operator+(const Vector4& other) const {
            return { x + other.x, y + other.y, z + other.z, w + other.w };
        }

        Vector4 operator-(const Vector4& other) const {
            return { x - other.x, y - other.y, z - other.z, w - other.w };
        }

        Vector4 operator*(const float scale) const {
            return { x * scale, y * scale, z * scale, w * scale };
        }

        Vector4 operator/(const float scale) const {
            return { x / scale, y / scale, z / scale, w / scale };
        }

        bool operator==(const Vector4& other) const {
            return (abs(x - other.x) < TOLERANCE &&
                    abs(y - other.y) < TOLERANCE &&
					abs(z - other.z) < TOLERANCE &&
                    abs(w - other.w) < TOLERANCE
                );
    	}

        bool operator!=(const Vector4& other) const {
            return !(*this == other);
        }

        float& operator[](const size_t value) {
            return (&x)[value];
    	}

        const float& operator[](const size_t value) const {
            return (&x)[value];
        }
    };

    Vector4 operator*(float scale, const Vector4& other);

    inline Vector4 operator*(const float scale, const Vector4& other) {
        return { scale * other.x, scale * other.y, scale * other.z, scale * other.w};
    }
}

#endif