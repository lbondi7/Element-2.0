//
// Created by lbondi7 on 08/11/2020.
//

#include <element/Maths/Vec2.h>


namespace Element {
    namespace Maths {
        Vec2::Vec2() : x(0.0f), y(0.0f) {}
        Vec2::Vec2(float x, float y) : x(x), y(y) {}
        Vec2::Vec2(float value): x(value), y(value) {}

        Vec2 Vec2::operator()(float value) const {
            return Vec2(value);
        }

        Vec2 Vec2::operator()(float x, float y, float z) const {
            return Vec2(x, y);
        }

        Vec2 &Vec2::operator=(const Vec2 &other) {
            this->x = other.x;
            this->y = other.y;
            return *this;
        }

        bool Vec2::operator==(const Vec2 &other) const {
            return this->x == other.x &&
                   this->y == other.y;
        }

        Vec2 &Vec2::operator+=(const Vec2 &other) {
            this->x += other.x;
            this->y += other.y;

            return *this;
        }

        Vec2 &Vec2::operator-=(const Vec2 &other) {
            this->x -= other.x;
            this->y -= other.y;
            return *this;
        }

        float Vec2::operator*=(const Vec2 &other) {
            return this->x * other.x +
                   this->y * other.y;
        }

        float Vec2::operator/=(const Vec2 &other) {
            return this->x / other.x +
                   this->y / other.y;
        }

        Vec2 Vec2::operator+(const Vec2 &other) {
            return Vec2(this->x + other.x, this->y + other.y);
        }

        Vec2 Vec2::operator-(const Vec2 &other) {
            return Vec2(this->x - other.x, this->y - other.y);
        }

        Vec2 Vec2::operator*(const Vec2 &other) {
            return Vec2(this->x * other.x, this->y * other.y);
        }

        Vec2 Vec2::operator/(const Vec2 &other) {
            return Vec2(this->x / other.x, this->y / other.y);
        }

        Vec2 &Vec2::operator+=(float other) {
            this->x += other;
            this->y += other;
            return *this;
        }

        Vec2 &Vec2::operator-=(float other) {
            this->x -= other;
            this->y -= other;
            return *this;
        }

        Vec2 &Vec2::operator*=(float other) {
            this->x *= other;
            this->y *= other;
            return *this;
        }

        Vec2 &Vec2::operator/=(float other) {
            this->x /= other;
            this->y /= other;
            return *this;
        }

        std::ostream &operator<<(std::ostream &os, const Vec2 &Vec2) {
            os << "x: " << Vec2.x << " r: " << Vec2.r << " s: " << Vec2.s << " y: " << Vec2.y << " g: " << Vec2.g << " t: "
               << Vec2.t;
            return os;
        }

        bool Vec2::operator<(const Vec2 &rhs) const {
            if (x < rhs.x)
                return true;
            if (rhs.x < x)
                return false;
            if (r < rhs.r)
                return true;
            if (rhs.r < r)
                return false;
            if (s < rhs.s)
                return true;
            if (rhs.s < s)
                return false;
            if (y < rhs.y)
                return true;
            if (rhs.y < y)
                return false;
            if (g < rhs.g)
                return true;
            if (rhs.g < g)
                return false;
            if (t < rhs.t)
                return true;
            if (rhs.t < t)
                return false;
        }

        bool Vec2::operator>(const Vec2 &rhs) const {
            return rhs < *this;
        }

        bool Vec2::operator<=(const Vec2 &rhs) const {
            return !(rhs < *this);
        }

        bool Vec2::operator>=(const Vec2 &rhs) const {
            return !(*this < rhs);
        }

        float Vec2::dot(const Vec2 &other) const {
            return (this->x * other.x) +
                   (this->y * other.y);
        }

        float Vec2::distance(const Vec2 &other) const {
            return std::sqrtf(std::pow(this->x - other.x, 2) +
                              std::pow(this->y - other.y, 2));
        }

        float Vec2::length() const {
            return std::sqrtf(std::pow(this->x, 2) +
                              std::pow(this->y, 2));
        }

        float Vec2::dot(const Vec2 &vec1, const Vec2 &vec2) {
            return (vec1.x * vec2.x) +
                   (vec1.y * vec2.y);
        }

        float Vec2::distance(const Vec2 &vec1, const Vec2 &vec2) {
            return std::sqrtf(std::pow(vec1.x - vec2.x, 2) +
                              std::pow(vec1.y - vec2.y, 2));
        }

    }
}