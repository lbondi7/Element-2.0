//
// Created by lbondi7 on 13/11/2020.
//

#include "element/Maths/Vec4.h"

namespace Element {
    namespace Maths {
        Vec4::Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
        Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        Vec4::Vec4(float value): x(value), y(value), z(value), w(value)  {}

        Vec4::Vec4(float* values): x(values[0]), y(values[1]), z(values[2]), w(values[3]){};

        Vec4 Vec4::operator()(float value) const {
            return Vec4(value);
        }

        Vec4 Vec4::operator()(float x, float y, float z, float w) const {
            return Vec4(x, y, z, w);
        }

        bool Vec4::operator==(const Vec4 &other) const {
            return this->x == other.x &&
                   this->y == other.y &&
                   this->z == other.z;
        }


        Vec4 &Vec4::operator=(const Vec4 &other) {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
            this->w = other.w;
            return *this;
        }

        Vec4 &Vec4::operator+=(const Vec4 &other) {
            this->x += other.x;
            this->y += other.y;
            this->z += other.z;
            this->w += other.w;
            return *this;
        }

        Vec4 &Vec4::operator-=(const Vec4 &other) {
            this->x -= other.x;
            this->y -= other.y;
            this->z -= other.z;
            this->w -= other.w;
            return *this;
        }

        float Vec4::operator*=(const Vec4 &other) {
            return this->x * other.x +
                   this->y * other.y +
                   this->z * other.z +
                   this->w * other.w;
        }

        float Vec4::operator/=(const Vec4 &other) {
            return this->x / other.x +
                   this->y / other.y +
                   this->z / other.z +
                   this->w / other.w;
        }

        Vec4 Vec4::operator+(const Vec4 &other) const {
            return Vec4(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
        }

        Vec4 Vec4::operator-(const Vec4 &other) const {
            return Vec4(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
        }

        Vec4 Vec4::operator*(float scalar) const {
            return Vec4(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar);
        }

        Vec4 Vec4::operator/(float scalar) const {
            return Vec4(this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar);
        }

        Vec4 &Vec4::operator+=(float other) {
            this->x += other;
            this->y += other;
            this->z += other;
            this->w - other;
            return *this;
        }

        Vec4 &Vec4::operator-=(float other) {
            this->x -= other;
            this->y -= other;
            this->z -= other;
            this->w -= other;
            return *this;
        }

        Vec4 &Vec4::operator*=(float other) {
            this->x *= other;
            this->y *= other;
            this->z *= other;
            this->w *= other;
            return *this;
        }

        Vec4 &Vec4::operator/=(float other) {
            this->x /= other;
            this->y /= other;
            this->z /= other;
            return *this;
        }

        std::ostream &operator<<(std::ostream &os, const Vec4 &Vec4) {
            os << "x: " << Vec4.x << " r: " << Vec4.r << " s: " << Vec4.s << " y: " << Vec4.y << " g: " << Vec4.g << " t: "
               << Vec4.t << " z: " << Vec4.z << " b: " << Vec4.b << " p: " << Vec4.p << " w: " << Vec4.w << " a: " <<
               Vec4.a << " o: " << Vec4.o;
            return os;
        }

        bool Vec4::operator<(const Vec4 &rhs) const {
            if (x < rhs.x)
                return true;
            if (rhs.x < x)
                return false;
            if (y < rhs.y)
                return true;
            if (rhs.y < y)
                return false;
            if (z < rhs.z)
                return true;
            if (rhs.z < z)
                return false;
            return w < rhs.w;
        }

        bool Vec4::operator>(const Vec4 &rhs) const {
            return rhs < *this;
        }

        bool Vec4::operator<=(const Vec4 &rhs) const {
            return !(rhs < *this);
        }

        bool Vec4::operator>=(const Vec4 &rhs) const {
            return !(*this < rhs);
        }

        float Vec4::dot(const Vec4 &other) const {
            return (this->x * other.x) +
                   (this->y * other.y) +
                   (this->z * other.z) +
                   (this->w * other.w);
        }
        

        float Vec4::length() const {
            return std::sqrtf(std::pow(this->x, 2) +
                              std::pow(this->y, 2) +
                              std::pow(this->z, 2) +
                              std::pow(this->w, 2));
        }

        float Vec4::dot(const Vec4 &vec1, const Vec4 &vec2) {
            return (vec1.x * vec2.x) +
                   (vec1.y * vec2.y) +
                   (vec1.z * vec2.z) +
                   (vec1.w * vec2.w);
        }

        Vec4::Vec4(const Vec4 &other) : x(other.x), y(other.y), z(other.z), w(other.w) {

        }

    }
}