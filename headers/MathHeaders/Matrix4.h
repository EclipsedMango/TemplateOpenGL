#ifndef MATRIX4_H
#define MATRIX4_H

#pragma once

#define TOLERANCE 0.000005

#include <cmath>
#include <string>
#include "Vector3.h"
#include "Vector4.h"

namespace emc {
    struct Matrix4 {
    	float m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16;
		float* mm[4] = {&m1, &m5, &m9, &m13};

        Matrix4() { m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m9 = m10 = m11 = m12 = m13 = m14 = m15 = m16 = 0; }
    	Matrix4(const float val) { m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m9 = m10 = m11 = m12 = m13 = m14 = m15 = m16 = val; }

        Matrix4(const float m1, const float m2, const float m3, const float m4, const float m5, 
				const float m6, const float m7, const float m8, const float m9, const float m10, 
				const float m11, const float m12, const float m13, const float m14, const float m15, const float m16) :
				m1(m1), m2(m2), m3(m3), m4(m4), m5(m5), m6(m6), m7(m7), m8(m8), m9(m9), m10(m10), m11(m11),
    			m12(m12), m13(m13), m14(m14), m15(m15), m16(m16) {}

        explicit Matrix4(const float* values) :
    		m1(values[0]), m2(values[1]), m3(values[2]), m4(values[3]), m5(values[4]),
    		m6(values[5]), m7(values[6]), m8(values[7]), m9(values[8]), m10(values[9]),
    		m11(values[10]), m12(values[11]), m13(values[12]), m14(values[13]), m15(values[14]), m16(values[15]) { }

		[[nodiscard]] std::string ToString() const {
            std::string str;
            for (int i = 0; i < 16; ++i) {
                str += std::to_string((*this)[i]) + ", ";
            }
            return str;
        }

		explicit operator float*() { return &m1; }

        static Matrix4 MakeIdentity() { return {
        		1.0f, 0.0f, 0.0f, 0.0f,
        		0.0f, 1.0f, 0.0f, 0.0f,
        		0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
        }

        [[nodiscard]] Matrix4 Transposed() const { return {
        		m1, m5, m9, m13,
        		m2, m6, m10, m14,
        		m3, m7, m11, m15,
				m4, m8, m12, m16
			};
        }

		static Matrix4 MakeScale(const Vector3& vec) {
            return {
            	vec.x, 0.0f, 0.0f, 0.0f,
                0.0f, vec.y, 0.0f, 0.0f,
                0.0f, 0.0f, vec.z, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }

        static Matrix4 MakeScale(const float x, const float y, const float z) {
            return {
                x, 0.0f, 0.0f, 0.0f,
                0.0f, y, 0.0f, 0.0f,
                0.0f, 0.0f, z, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }

		 static Matrix4 MakeTranslation(const Vector3& vec) {
            return {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                vec.x, vec.y, vec.z, 1.0f
            };
        }

        static Matrix4 MakeTranslation(const float x, const float y, const float z) {
            return {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                x, y, z, 1.0f
            };
        }

		static Matrix4 MakeRotateX(const float theta) {
            return {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, std::cos(theta), -std::sin(theta), 0.0f,
                0.0f, std::sin(theta), std::cos(theta), 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }

        static Matrix4 MakeRotateY(const float theta) {
            return {
                std::cos(theta), 0.0f, std::sin(theta), 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                -std::sin(theta), 0.0f, std::cos(theta), 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }

        static Matrix4 MakeRotateZ(const float theta) {
            return {
                std::cos(theta), std::sin(theta), 0.0f, 0.0f,
                -std::sin(theta), std::cos(theta), 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }

		static Matrix4 MakeEuler(const Vector3& vec) {
            return { MakeRotateZ(vec.z) * MakeRotateY(vec.y) * MakeRotateX(vec.x) };
        }

        static Matrix4 MakeEuler(const float x, const float y, const float z) {
            return { MakeRotateZ(z) * MakeRotateY(y) * MakeRotateX(x) };
        }


        Vector4 operator*(const Vector4& vec) const {
            return {
            	(m1 * vec.x + m5 * vec.y + m9 * vec.z + m13 * vec.w),
            	(m2 * vec.x + m6 * vec.y + m10 * vec.z + m14 * vec.w),
            	(m3 * vec.x + m7 * vec.y + m11 * vec.z + m15 * vec.w),
                (m4 * vec.x + m8 * vec.y + m12 * vec.z + m16 * vec.w)
            };
        }

        Matrix4 operator*(const Matrix4& other) const {
            return {
				(other.m1 * m1 + other.m2 * m5 + other.m3 * m9 + other.m4 * m13),
				(other.m1 * m2 + other.m2 * m6 + other.m3 * m10 + other.m4 * m14),
				(other.m1 * m3 + other.m2 * m7 + other.m3 * m11 + other.m4 * m15),
				(other.m1 * m4 + other.m2 * m8 + other.m3 * m12 + other.m4 * m16),

				(other.m5 * m1 + other.m6 * m5 + other.m7 * m9 + other.m8 * m13),
				(other.m5 * m2 + other.m6 * m6 + other.m7 * m10 + other.m8 * m14),
				(other.m5 * m3 + other.m6 * m7 + other.m7 * m11 + other.m8 * m15),
				(other.m5 * m4 + other.m6 * m8 + other.m7 * m12 + other.m8 * m16),

				(other.m9 * m1 + other.m10 * m5 + other.m11 * m9 + other.m12 * m13),
				(other.m9 * m2 + other.m10 * m6 + other.m11 * m10 + other.m12 * m14),
				(other.m9 * m3 + other.m10 * m7 + other.m11 * m11 + other.m12 * m15),
				(other.m9 * m4 + other.m10 * m8 + other.m11 * m12 + other.m12 * m16),

				(other.m13 * m1 + other.m14 * m5 + other.m15 * m9 + other.m16 * m13),
				(other.m13 * m2 + other.m14 * m6 + other.m15 * m10 + other.m16 * m14),
				(other.m13 * m3 + other.m14 * m7 + other.m15 * m11 + other.m16 * m15),
				(other.m13 * m4 + other.m14 * m8 + other.m15 * m12 + other.m16 * m16),
            };
        }

		bool operator==(const Matrix4& other) const {
            return (
                abs(m1 - other.m1) < TOLERANCE &&
                abs(m2 - other.m2) < TOLERANCE &&
                abs(m3 - other.m3) < TOLERANCE &&
                abs(m4 - other.m4) < TOLERANCE &&
                abs(m5 - other.m5) < TOLERANCE &&
                abs(m6 - other.m6) < TOLERANCE &&
                abs(m7 - other.m7) < TOLERANCE &&
                abs(m8 - other.m8) < TOLERANCE &&
                abs(m9 - other.m9) < TOLERANCE &&
                abs(m10 - other.m10) < TOLERANCE &&
                abs(m11 - other.m11) < TOLERANCE &&
                abs(m12 - other.m12) < TOLERANCE &&
                abs(m13 - other.m13) < TOLERANCE &&
                abs(m14 - other.m14) < TOLERANCE &&
                abs(m15 - other.m15) < TOLERANCE &&
                abs(m16 - other.m16) < TOLERANCE
            );
        }

        float& operator[](const size_t value) {
            return (&m1)[value];
    	}

		float operator[](const size_t value) const {
            return (&m1)[value];
        }

	};
}

#endif