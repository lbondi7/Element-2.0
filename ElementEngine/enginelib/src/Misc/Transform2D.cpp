//
// Created by lbondi7 on 08/11/2020.
//

#include "element/Transform2D.h"

bool Element::Transform2D::operator==(const Element::Transform2D &other) {
    return m_position == other.m_position &&
           m_rotation == other.m_rotation &&
           m_scale == other.m_scale &&
           m_size == other.m_size;

}

Element::Transform2D &Element::Transform2D::operator=(const Element::Transform2D &other) {
    this->m_position = other.m_position;
    this->m_size = other.m_size;
    this->m_rotation = other.m_rotation;
    this->m_scale = other.m_scale;
    return *this;
}

void Element::Transform2D::setRotationY(float y) {}

void Element::Transform2D::setRotationX(float x) {}

void Element::Transform2D::setRotation(float x, float y, float z) { updated = true;
    m_rotation = Element::Maths::Vec3(m_rotation.x, m_rotation.y, z);
}

void Element::Transform2D::setRotation(const Element::Maths::Vec3 &rotation) { updated = true;
    m_rotation = Element::Maths::Vec3(m_rotation.x, m_rotation.y, rotation.z);
}

void Element::Transform2D::setRotation(float rotation) { updated = true;
    m_rotation.z = rotation;
}

void Element::Transform2D::setScaleZ(float z) {}

void Element::Transform2D::setScale(float x, float y) { updated = true;
    m_scale = Element::Maths::Vec3(x, y , m_scale.z);
}

void Element::Transform2D::setScale(Element::Maths::Vec2 scale) { updated = true; m_scale = Element::Maths::Vec3
            (scale.x, scale.y, m_scale.z);
}

void Element::Transform2D::setScale(float scale) { updated = true;
    m_scale = Element::Maths::Vec3(scale, scale, m_scale.z);
}

void Element::Transform2D::setScale(float x, float y, float z) { updated = true;
    m_scale = Element::Maths::Vec3(x, y, m_scale.z);
}

void Element::Transform2D::setScale(const Element::Maths::Vec3 &scale) { updated = true;
    m_scale = Element::Maths::Vec3(scale.x, scale.y, m_scale.z);
}

Element::Maths::Vec2 Element::Transform2D::getScale2D() {
    return Element::Maths::Vec2(m_scale.x, m_scale.y);
}

void Element::Transform2D::setSizeY(float y) { updated = true;
    m_size.y = y;
}

void Element::Transform2D::setSizeX(float x) { updated = true;
    m_size.x = x;
}

void Element::Transform2D::setSize(float x, float y) { updated = true;
    m_size = Element::Maths::Vec2(x, y);
}

void Element::Transform2D::setSize(Element::Maths::Vec2 size) { updated = true;
    m_size = size;
}

float Element::Transform2D::getSizeX() const {
    return m_size.x;
}

float Element::Transform2D::getSizeY() const {
    return m_size.y;
}

Element::Maths::Vec2 Element::Transform2D::getSize() {
    return m_size;
}

void Element::Transform2D::setPosition(float x, float y) {
    m_position = Element::Maths::Vec3(x, y, m_position.z);
}

void Element::Transform2D::setPosition(const Element::Maths::Vec3 &position) {
    m_position = Element::Maths::Vec3(position.x, position.y, std::max(1.0f, std::min(0.0f, position.z)));
}

void Element::Transform2D::setPosition(Element::Maths::Vec2 position) {
    m_position = Element::Maths::Vec3(position.x, position.y, m_position.z);
}

Element::Maths::Vec2 Element::Transform2D::getPosition2D() {
    return Element::Maths::Vec2(m_position.x, m_position.y);
}

