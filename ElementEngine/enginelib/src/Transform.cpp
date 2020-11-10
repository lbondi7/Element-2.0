//
// Created by lbondi7 on 08/11/2020.
//

#include <element/Transform.h>

/*
*  Transform
*/


bool Element::Transform::operator==(const Element::Transform &other) const {
    return m_position == other.m_position &&
           m_rotation == other.m_rotation &&
           m_scale == other.m_scale;
}

Element::Transform &Element::Transform::operator=(const Element::Transform &other) {
    this->m_position = other.m_position;
    this->m_rotation = other.m_rotation;
    this->m_scale = other.m_scale;
    return *this;
}

float Element::Transform::getPositionX() const {
    return m_position.x;
}

float Element::Transform::getPositionY() const {
    return m_position.y;
}

float Element::Transform::getPositionZ() const {
    return m_position.z;
}

void Element::Transform::setPosition(const Element::Maths::Vec3 &position) { updated = true;
    m_position = position;
}

void Element::Transform::setPosition(float x, float y, float z) { updated = true;
    m_position = Element::Maths::Vec3(x, y, z);
}

void Element::Transform::setPositionX(float x) { updated = true;
m_position.x = x;
}

void Element::Transform::setPositionY(float y) { updated = true;
m_position.y = y;
}

void Element::Transform::setPositionZ(float z) { updated = true;
m_position.z = z;
}

const Element::Maths::Vec3 &Element::Transform::getRotation() const {
    return m_rotation;
}

float Element::Transform::getRotationX() const {
    return m_rotation.x;
}

float Element::Transform::getRotationY() const {
    return m_rotation.y;
}

float Element::Transform::getRotationZ() const {
    return m_rotation.z;
}

void Element::Transform::setRotation(const Element::Maths::Vec3 &rotation) { updated = true;
m_rotation = rotation;
}

void Element::Transform::setRotation(float x, float y, float z) { updated = true;
m_rotation = Element::Maths::Vec3(x, y, z);
}

void Element::Transform::setRotationX(float x) { updated = true;
m_rotation.x = x;
}

void Element::Transform::setRotationY(float y) { updated = true;
m_rotation.y = y;
}

void Element::Transform::setRotationZ(float z) { updated = true;
m_rotation.z = z;
}

const Element::Maths::Vec3 &Element::Transform::getScale() const {
    return m_scale;
}

float Element::Transform::getScaleX() const {
    return m_scale.x;
}

float Element::Transform::getScaleY() const {
    return m_scale.y;
}

float Element::Transform::getScaleZ() const {
    return m_scale.z;
}

void Element::Transform::setScale(const Element::Maths::Vec3 &scale) { updated = true;
m_scale = scale;
}

void Element::Transform::setScale(float x, float y, float z) { updated = true;
m_scale = Element::Maths::Vec3(x, y, z);
}

void Element::Transform::setScale(float scale) { updated = true;
m_scale = Element::Maths::Vec3(scale, scale, scale);
}

void Element::Transform::setScaleX(float x) { updated = true;
m_scale.x = x;
}

void Element::Transform::setScaleY(float y) { updated = true;
m_scale.y = y;
}

void Element::Transform::setScaleZ(float z) { updated = true;
m_scale.z = z;
}


/*
*  Transform2D
*/