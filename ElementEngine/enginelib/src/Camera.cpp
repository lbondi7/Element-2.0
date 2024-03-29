#include <element/Camera.h>
#include <element/GameSettings.h>

#include <iostream>
//
//Element::Camera::Camera()
//{
////    worldUp = { 0.0f, 1.0f, 0.0f };
////
////    viewport = { 0, 0, 1, 1 };
////    rect = { 0, 0, 1, 1 };
////    zoom = 1.f;
//}
//
//Element::Camera::Camera(CameraType _type) : type(_type)
//{
////    type = _type;
////    transform.setPosition(0.0f, 0.0f, -5.0f);
////    worldUp = { 0.0f, 1.0f, 0.0f };
////
////    viewport = { 0, 0, 1, 1 };
////    rect = { 0, 0, 1, 1 };
////    zoom = 1.f;
////    cameraChanged = true;
//}


const Vec3& Element::Camera::GetForward() const
{
    return forward;
}

const Vec3& Element::Camera::GetRight() const
{
    return right;
}

const Vec3& Element::Camera::GetUp() const
{
    return up;
}

void Element::Camera::setCameraChanged(bool _cameraChanged)
{
    cameraChanged = _cameraChanged;
}

bool Element::Camera::hasCameraChanged()
{
    return cameraChanged;
}

float Element::Camera::GetZoom()
{
    return zoom;
}

void Element::Camera::SetZoom(float _zoom)
{
    zoom = std::min(1000.0f, std::max(0.01f, _zoom));
    cameraChanged = true;
}

float Element::Camera::GetFOV()
{
    return FOV;
}

void Element::Camera::SetFOV(float fov)
{
    FOV = std::min(179.99f, std::max(0.01f, fov));
    cameraChanged = true;
}

//void Element::Camera::update(float windowWidth, float windowHeight)
//{
//    if (!cameraChanged) return;
//
////    transformMatrix = glm::translate(glm::mat4(1.0f), transform.getPosition());
////
////    transformMatrix = glm::rotate(transformMatrix, glm::radians(transform.getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
////    transformMatrix = glm::rotate(transformMatrix, glm::radians(transform.getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
////    transformMatrix = glm::rotate(transformMatrix, glm::radians(transform.getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
////
////    inverted = glm::inverse(transformMatrix);
////    right = normalize(glm::vec3(inverted[0]));
////    up = normalize(glm::vec3(-inverted[1]));
////    forward = normalize(glm::vec3(inverted[2]));
////
////    lookAtPos = transform.getPosition() + forward;
////    viewMatrix = glm::lookAt(transform.getPosition(), lookAtPos, up);
////    auto widthLonger = windowWidth >= windowHeight;
////    auto aspectRatio = widthLonger ? windowWidth / windowHeight : windowHeight / windowWidth;
////    if (type == CameraType::PERSPECTIVE)
////    {
////        projectionMatrix = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 1000.0f);
////    }
////    else
////    {
////        auto x = (widthLonger ? aspectRatio : 1.0f);
////        auto y = (widthLonger ? 1.0f : aspectRatio);
////
////        projectionMatrix = glm::ortho(-x * zoom, x * zoom, -y * zoom, y * zoom, 0.1f, 1000.0f);
////    }
////
////    //projectionMatrix = glm::ortho(-zoom * windowWidth, windowWidth * zoom, -zoom * windowHeight,  windowHeight * zoom, 0.1f, 1000.0f);
////    projectionMatrix[1][1] *= -1;
//}

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

float Element::Camera::getPosX()
{
    return transform.getPositionX();
}

float Element::Camera::getPosY()
{
    return transform.getPositionY();
}

float Element::Camera::getPosZ()
{
    return transform.getPositionZ();
}

const Element::Maths::Vec3& Element::Camera::getRot() const
{
    return transform.getRotation();
}

float Element::Camera::getRotX()
{
    return transform.getRotationX();
}

float Element::Camera::getRotY()
{
    return transform.getRotationY();
}

float Element::Camera::getRotZ()
{
    return transform.getRotationZ();
}

bool Element::Camera::isReady() {
    return ready;
}

void Element::Camera::setReady(bool _ready) {
    ready = _ready;
}

const Element::Transform &Element::Camera::getTransform() const {
    return transform;
}

void Element::Camera::setTransform(const Element::Transform& _transform) {
transform = _transform;
}

const Vec3 &Element::Camera::GetLookAt() const {
    return lookAtPos;
}
