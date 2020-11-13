//
// Created by lbondi7 on 08/11/2020.
//

#pragma once

#include <ostream>

namespace Element {

    namespace Maths {
        class Vec3 {
        public:

            explicit Vec3();
            explicit Vec3(float x, float y, float z);
            explicit Vec3(float value);
            explicit Vec3::Vec3(float* values);

            ~Vec3() = default;

            Vec3 operator()(float value = 0.0f) const;

            Vec3 operator()(float x, float y, float z) const;

            bool operator==(const Vec3 &other) const;

            Vec3 &operator=(const Vec3 &other);

            Vec3 &operator+=(const Vec3 &other);

            Vec3 &operator-=(const Vec3 &other);

            float operator*=(const Vec3 &other);

            float operator/=(const Vec3 &other);

            Vec3 operator+(const Vec3 &other) const;

            Vec3 operator-(const Vec3 &other) const;

            Vec3 operator*(float scalar) const;

            Vec3 operator/(float scalar) const;

            Vec3 &operator+=(float other);

            Vec3 &operator-=(float other);

            Vec3 &operator*=(float other);

            Vec3 &operator/=(float other);

            friend std::ostream &operator<<(std::ostream &os, const Vec3 &vec3);

            bool operator<(const Vec3 &rhs) const;

            bool operator>(const Vec3 &rhs) const;

            bool operator<=(const Vec3 &rhs) const;

            bool operator>=(const Vec3 &rhs) const;

            [[nodiscard]] float dot(const Vec3& other) const;

            [[nodiscard]] Vec3 cross(const Vec3 &other) const;

            [[nodiscard]] float distance(const Vec3 &other) const;

            [[nodiscard]] float length() const;

            static float dot(const Vec3& vec1, const Vec3& vec2);

            static Vec3 cross(const Vec3& vec1, const Vec3& vec2);

            static float distance(const Vec3& vec1, const Vec3& vec2);

            union {
                float x, r, s;
            };
            union {
                float y, g, t;
            };
            union {
                float z, b, p;
            };
        };
    }
}

using namespace Element::Maths;
//ELEMENTENGINE_VEC3_H
