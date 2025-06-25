#ifndef MATRIX3_H
#define MATRIX3_H

#pragma once

#define TOLERANCE 0.000005

#include <string>
#include <cmath>
#include "Vector3.h"

namespace emc {
    struct Matrix3 {
        float m1, m2, m3, m4, m5, m6, m7, m8, m9;
        float* mm[3] = {&m1, &m4, &m7};

        Matrix3() { m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m9 = 0; }

        Matrix3(const float m1, const float m2, const float m3, const float m4, const float m5, 
			const float m6, const float m7, const float m8, const float m9) :
				m1(m1), m2(m2), m3(m3), m4(m4), m5(m5), m6(m6), m7(m7), m8(m8), m9(m9) {}

        explicit Matrix3(const float* values) :
    		m1(values[0]), m2(values[1]), m3(values[2]), m4(values[3]), m5(values[4]),
    		m6(values[5]), m7(values[6]), m8(values[7]), m9(values[8]) {}

		[[nodiscard]] std::string ToString() const {
            std::string str;
            for (int i = 0; i < 9; ++i) {
                str += std::to_string((*this)[i]) + ", ";
            }
            return str;
        }

		explicit operator float* () { return &m1; }
        explicit operator const float* () const { return &m1; }

        static Matrix3 MakeIdentity() { return {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f}; }
        [[nodiscard]] Matrix3 Transposed() const { return { m1, m4, m7, m2, m5, m8, m3, m6, m9 }; }

        static Matrix3 MakeScale(const Vector3& vec) {
            return {
            	vec.x, 0.0f, 0.0f,
                0.0f, vec.y, 0.0f,
                0.0f, 0.0f, vec.z
            };
        }

        static Matrix3 MakeScale(const float x, const float y, const float z) {
            return {
                x, 0.0f, 0.0f,
                0.0f, y, 0.0f,
                0.0f, 0.0f, z
            };
        }

        static Matrix3 MakeScale(const float x, const float y) {
            return {
                x, 0.0f, 0.0f,
                0.0f, y, 0.0f,
                0.0f, 0.0f, 1.0f
            };
        }

        static Matrix3 MakeTranslation(const Vector3& vec) {
            return {
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                vec.x, vec.y, vec.z
            };
        }

        static Matrix3 MakeTranslation(const float x, const float y, const float z) {
            return {
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                x, y, z
            };
        }

        static Matrix3 MakeRotateX(const float theta) {
            return {
                1.0f, 0.0f, 0.0f,
                0.0f, std::cos(theta), -std::sin(theta),
                0.0f, std::sin(theta), std::cos(theta)
            };
        }

        static Matrix3 MakeRotateY(const float theta) {
            return {
                std::cos(theta), 0.0f, std::sin(theta),
                0.0f, 1.0f, 0.0f,
                -std::sin(theta), 0.0f, std::cos(theta)
            };
        }

        static Matrix3 MakeRotateZ(const float theta) {
            return {
                std::cos(theta), std::sin(theta), 0.0f,
                -std::sin(theta), std::cos(theta), 0.0f,
                0.0f, 0.0f, 1.0f
            };
        }

        static Matrix3 MakeEuler(const Vector3& vec) {
            return { MakeRotateZ(vec.z) * MakeRotateY(vec.y) * MakeRotateX(vec.x) };
        }

        static Matrix3 MakeEuler(const float x, const float y, const float z) {
            return { MakeRotateZ(z) * MakeRotateY(y) * MakeRotateX(x) };
        }

        Vector3 operator*(const Vector3& vec) const {
            return {
            	(m1 * vec.x + m4 * vec.y + m7 * vec.z),
            	(m2 * vec.x + m5 * vec.y + m8 * vec.z),
            	(m3 * vec.x + m6 * vec.y + m9 * vec.z)
            };
        }

        Matrix3 operator*(const Matrix3& other) const {
            return {
                (other.m1 * m1 + other.m2 * m4 + other.m3 * m7),
            	(other.m1 * m2 + other.m2 * m5 + other.m3 * m8),
            	(other.m1 * m3 + other.m2 * m6 + other.m3 * m9),

                (other.m4 * m1 + other.m5 * m4 + other.m6 * m7),
            	(other.m4 * m2 + other.m5 * m5 + other.m6 * m8),
            	(other.m4 * m3 + other.m5 * m6 + other.m6 * m9),

            	(other.m7 * m1 + other.m8 * m4 + other.m9 * m7),
            	(other.m7 * m2 + other.m8 * m5 + other.m9 * m8),
            	(other.m7 * m3 + other.m8 * m6 + other.m9 * m9),
            };
        }

        bool operator==(const Matrix3& other) const {
            return (
                abs(m1 - other.m1) < TOLERANCE &&
                abs(m2 - other.m2) < TOLERANCE &&
                abs(m3 - other.m3) < TOLERANCE &&
                abs(m4 - other.m4) < TOLERANCE &&
                abs(m5 - other.m5) < TOLERANCE &&
                abs(m6 - other.m6) < TOLERANCE &&
                abs(m7 - other.m7) < TOLERANCE &&
                abs(m8 - other.m8) < TOLERANCE &&
                abs(m9 - other.m9) < TOLERANCE
            );
        }

        float& operator[](const size_t value) {
            return (&m1)[value];
    	}

		const float& operator[](const size_t value) const {
            return (&m1)[value];
        }
    };
}

#endif