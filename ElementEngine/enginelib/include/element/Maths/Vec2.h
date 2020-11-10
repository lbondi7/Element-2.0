//
// Created by lbondi7 on 08/11/2020.
//

#pragma once

#include <ostream>

namespace Element {

    namespace Maths {
        class Vec2 {
        public:

            explicit Vec2();

            explicit Vec2(float x, float y);

            explicit Vec2(float value);

            ~Vec2() = default;

            Vec2 operator()(float value = 0.0f) const;

            Vec2 operator()(float x, float y, float z) const;

            bool operator==(const Vec2 &other) const;

            Vec2 &operator=(const Vec2 &other);

            Vec2 &operator+=(const Vec2 &other);

            Vec2 &operator-=(const Vec2 &other);

            float operator*=(const Vec2 &other);

            float operator/=(const Vec2 &other);

            Vec2 operator+(const Vec2 &other);

            Vec2 operator-(const Vec2 &other);

            Vec2 operator*(const Vec2 &other);

            Vec2 operator/(const Vec2 &other);

            Vec2 &operator+=(float other);

            Vec2 &operator-=(float other);

            Vec2 &operator*=(float other);

            Vec2 &operator/=(float other);

            friend std::ostream &operator<<(std::ostream &os, const Vec2 &Vec2);

            bool operator<(const Vec2 &rhs) const;

            bool operator>(const Vec2 &rhs) const;

            bool operator<=(const Vec2 &rhs) const;

            bool operator>=(const Vec2 &rhs) const;

            union {
                float x, r, s;
            };
            union {
                float y, g, t;
            };
        };
    }
}

using namespace Element::Maths;
 //ELEMENTENGINE_VEC2_H
