#include <element/Camera.h>
#include <element/GameSettings.h>

const Vec3& Element::Camera::getForward() const
{
    return forward;
}

const Vec3& Element::Camera::getRight() const
{
    return right;
}

const Vec3& Element::Camera::getUp() const
{
    return up;
}

void Element::Camera::setCameraChanged(bool _cameraChanged)
{
    cameraChanged = _cameraChanged;
}

bool Element::Camera::hasCameraChanged() const
{
    return cameraChanged;
}

float Element::Camera::getZoom() const
{
    return zoom;
}

void Element::Camera::setZoom(float _zoom)
{
    zoom = std::min(1000.0f, std::max(0.01f, _zoom));
    cameraChanged = true;
}

float Element::Camera::getFOV() const
{
    return fov;
}

void Element::Camera::setFOV(float _fov)
{
    fov = std::min(179.99f, std::max(0.01f, _fov));
    cameraChanged = true;
}

void Element::Camera::setPos(const Element::Maths::Vec3& position)
{
    cameraChanged = true;
    transform.setPosition(position);
}

void Element::Camera::setPos(float x, float y, float z)
{
    cameraChanged = true;
    transform.setPosition(x, y, z);
}

void Element::Camera::setPos(float position)
{
    cameraChanged = true;
    transform.setPosition(position, position, position);
}

void Element::Camera::setPosX(float x)
{
    cameraChanged = true;
    transform.setPositionX(x);
}

void Element::Camera::setPosY(float y)
{
    cameraChanged = true;
    transform.setPositionY(y);
}

void Element::Camera::setPosZ(float z)
{
    cameraChanged = true;
    transform.setPositionZ(z);
}

void Element::Camera::setRot(const Element::Maths::Vec3& rotation)
{
    cameraChanged = true;
    transform.setRotation(rotation);
}

void Element::Camera::setRot(float x, float y, float z)
{
    cameraChanged = true;
    transform.setRotation(x, y, z);
}

void Element::Camera::setRotX(float x)
{
    cameraChanged = true;
    transform.setRotationX(x);
}

void Element::Camera::setRotY(float y)
{
    cameraChanged = true;
    transform.setRotationY(y);
}

void Element::Camera::setRotZ(float z)
{
    cameraChanged = true;
    transform.setRotationZ(z);
}

const Element::Maths::Vec3& Element::Camera::getPos() const
{
    return transform.getPosition();
}

float Element::Camera::getPosX() const
{
    return transform.getPositionX();
}

float Element::Camera::getPosY() const
{
    return transform.getPositionY();
}

float Element::Camera::getPosZ() const
{
    return transform.getPositionZ();
}

const Element::Maths::Vec3& Element::Camera::getRot() const
{
    return transform.getRotation();
}

float Element::Camera::getRotX() const
{
    return transform.getRotationX();
}

float Element::Camera::getRotY() const
{
    return transform.getRotationY();
}

float Element::Camera::getRotZ() const
{
    return transform.getRotationZ();
}

bool Element::Camera::isEnabled() const
{
    return enabled;
}

void Element::Camera::setEnabled(bool _enabled)
{
    enabled = _enabled;
}

const Element::Transform &Element::Camera::getTransform() const
{
    return transform;
}

void Element::Camera::setTransform(const Element::Transform& _transform)
{
    cameraChanged = true;
    transform = _transform;
}

const Vec3 &Element::Camera::getLookAt() const {
    return lookAtPos;
}

const Vec4& Element::Camera::getViewport() const
{
    return viewport;
}

const Vec4& Element::Camera::getRect() const
{
    return rect;
}

void Element::Camera::setRect(const Vec4 &_rect)
{
    rect = _rect;
    cameraChanged = true;
}

void Element::Camera::setViewport(const Vec4 &_viewport)
{
    viewport = _viewport;
    cameraChanged = true;
}

Element::ViewDimension Element::Camera::getViewDimension() const
{
    return dimension;
}

void Element::Camera::setViewDimension(Element::ViewDimension _dimension) {
    dimension = _dimension;
    cameraChanged = true;
}

float Element::Camera::getNearPlane() const {
    return nearPlane;
}

void Element::Camera::setNearPlane(float _nearPlane) {
    nearPlane = _nearPlane;
    cameraChanged = true;
}

float Element::Camera::getFarPlane() const {
    return farPlane;
}

void Element::Camera::setFarPlane(float _farPlane) {
    farPlane = _farPlane;
    cameraChanged = true;
}
