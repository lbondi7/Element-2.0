//
// Created by lbondi7 on 08/11/2020.
//

#include <element/Maths/Vec3.h>

namespace Element {
namespace Maths {
    Vec3::Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3::Vec3(float value): x(value), y(value), z(value)  {}

    Vec3 Vec3::operator()(float value) const {
        return Vec3(value);
    }

    Vec3 Vec3::operator()(float x, float y, float z) const {
        return Vec3(x, y, z);
    }

    Vec3 &Vec3::operator=(const Vec3 &other) {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        return *this;
    }

    bool Vec3::operator==(const Vec3 &other) const {
        return this->x == other.x &&
               this->y == other.y &&
               this->z == other.z;
    }

    Vec3 &Vec3::operator+=(const Vec3 &other) {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }

    Vec3 &Vec3::operator-=(const Vec3 &other) {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }

    float Vec3::operator*=(const Vec3 &other) {
        return this->x * other.x +
                this->y * other.y +
                this->z * other.z;
    }

    float Vec3::operator/=(const Vec3 &other) {
        return this->x / other.x +
               this->y / other.y +
               this->z / other.z;
    }

    Vec3 Vec3::operator+(const Vec3 &other) const {
        return Vec3(this->x + other.x, this->y + other.y, this->z + other.z);
    }

    Vec3 Vec3::operator-(const Vec3 &other) const {
        return Vec3(this->x - other.x, this->y - other.y, this->z - other.z);
    }

    Vec3 Vec3::operator*(float scalar) const {
        return Vec3(this->x * scalar, this->y * scalar, this->z * scalar);
    }

    Vec3 Vec3::operator/(float scalar) const {
        return Vec3(this->x / scalar, this->y / scalar, this->z / scalar);
    }

    Vec3 &Vec3::operator+=(float other) {
        this->x += other;
        this->y += other;
        this->z += other;
        return *this;
    }

    Vec3 &Vec3::operator-=(float other) {
        this->x -= other;
        this->y -= other;
        this->z -= other;
        return *this;
    }

    Vec3 &Vec3::operator*=(float other) {
        this->x *= other;
        this->y *= other;
        this->z *= other;
        return *this;
    }

    Vec3 &Vec3::operator/=(float other) {
        this->x /= other;
        this->y /= other;
        this->z /= other;
        return *this;
    }

    std::ostream &operator<<(std::ostream &os, const Vec3 &vec3) {
        os << "x: " << vec3.x << " r: " << vec3.r << " s: " << vec3.s << " y: " << vec3.y << " g: " << vec3.g << " t: "
           << vec3.t << " z: " << vec3.z << " b: " << vec3.b << " p: " << vec3.p;
        return os;
    }

    bool Vec3::operator<(const Vec3 &rhs) const {
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
        if (z < rhs.z)
            return true;
        if (rhs.z < z)
            return false;
        if (b < rhs.b)
            return true;
        if (rhs.b < b)
            return false;
        return p < rhs.p;
    }

    bool Vec3::operator>(const Vec3 &rhs) const {
        return rhs < *this;
    }

    bool Vec3::operator<=(const Vec3 &rhs) const {
        return !(rhs < *this);
    }

    bool Vec3::operator>=(const Vec3 &rhs) const {
        return !(*this < rhs);
    }

}
}