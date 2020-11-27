//
// Created by lbondi7 on 10/11/2020.
//

#include "element/Light.h"

const Vec3 &Element::Light::getPosition() const {
    return transform.getPosition();
}

Element::LightType Element::Light::getType() const {
    return type;
}

void Element::Light::setType(Element::LightType type) {
    Light::type = type;
}

float Element::Light::getIntensity() const {
    return intensity;
}

void Element::Light::setIntensity(float intensity) {
    Light::intensity = intensity;
}

const Vec3 &Element::Light::getColour() const {
    return colour;
}

void Element::Light::setColour(const Vec3 &colour) {
    Light::colour = colour;
}

const Vec3 &Element::Light::getDirection() const {
    return direction;
}

void Element::Light::setDirection(const Vec3 &direction) {
    Light::direction = direction;
}

float Element::Light::getCutOff() const {
    return cutOff;
}

void Element::Light::setCutOff(float cutOff) {
    Light::cutOff = cutOff;
}

float Element::Light::getOuterCutOff() const {
    return outerCutOff;
}

void Element::Light::setOuterCutOff(float outerCutOff) {
    Light::outerCutOff = outerCutOff;
}

float Element::Light::getConstant() const {
    return constant;
}

void Element::Light::setConstant(float constant) {
    Light::constant = constant;
}

float Element::Light::getLinear() const {
    return linear;
}

void Element::Light::setLinear(float linear) {
    Light::linear = linear;
}

float Element::Light::getQuadratic() const {
    return quadratic;
}

void Element::Light::setQuadratic(float quadratic) {
    Light::quadratic = quadratic;
}

void Element::Light::setPosition(const Vec3 &position) {
    Light::transform.setPosition(position);
}

void Element::Light::setPosition(float x, float y, float z) {
    Light::transform.setPosition(x, y, z);
}

void Element::Light::setPosX(float x) {
    Light::transform.setPositionX(x);
}

void Element::Light::setPosY(float y) {
    Light::transform.setPositionY(y);
}

void Element::Light::setPosZ(float z) {
    Light::transform.setPositionZ(z);
}

void Element::Light::setColour(float r, float g, float b) {
    colour = Vec3(r, g, b);
}

void Element::Light::setColR(float r) {
    colour.r = r;
}

void Element::Light::setColG(float g) {
colour.g = g;
}

void Element::Light::setColB(float b) {
colour.b = b;
}

void Element::Light::setDirection(float x, float y, float z) {
direction = Vec3(x, y, z);
}

void Element::Light::setDirX(float x) {
    direction.x = x;
}

void Element::Light::setDirY(float y) {
direction.y = y;
}

void Element::Light::setDirZ(float z) {
direction.z = z;
}
