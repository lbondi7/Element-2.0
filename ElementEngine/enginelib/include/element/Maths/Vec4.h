//
// Created by lbondi7 on 13/11/2020.
//

#pragma once


#include <ostream>

namespace Element {
    namespace Maths {

        class Vec4 {
        public:

            explicit Vec4();

            explicit Vec4(float x, float y, float z, float w);

            explicit Vec4(float value);

            explicit Vec4(float *values);

            explicit Vec4(const Vec4& other);

            ~Vec4() = default;

            Vec4 operator()(float value = 0.0f) const;

            Vec4 operator()(float x, float y, float z, float w) const;

            bool operator==(const Vec4 &other) const;

            Vec4 &operator=(const Vec4 &other);

            Vec4 &operator+=(const Vec4 &other);

            Vec4 &operator-=(const Vec4 &other);

            float operator*=(const Vec4 &other);

            float operator/=(const Vec4 &other);

            Vec4 operator+(const Vec4 &other) const;

            Vec4 operator-(const Vec4 &other) const;

            Vec4 operator*(float scalar) const;

            Vec4 operator/(float scalar) const;

            Vec4 &operator+=(float other);

            Vec4 &operator-=(float other);

            Vec4 &operator*=(float other);

            Vec4 &operator/=(float other);

            friend std::ostream &operator<<(std::ostream &os, const Vec4 &Vec4);

            bool operator<(const Vec4 &rhs) const;

            bool operator>(const Vec4 &rhs) const;

            bool operator<=(const Vec4 &rhs) const;

            bool operator>=(const Vec4 &rhs) const;

            [[nodiscard]] float dot(const Vec4 &other) const;

            [[nodiscard]] float length() const;

            static float dot(const Vec4 &vec1, const Vec4 &vec2);

            union {
                float x, r, s;
            };
            union {
                float y, g, t;
            };
            union {
                float z, b, p;
            };
            union {
                float w, a, o;
            };
        };
    }
}

 //ELEMENTENGINE_VEC4_H
