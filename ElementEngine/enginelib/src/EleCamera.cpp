//
// Created by lbondi7 on 08/11/2020.
//

#include "EleCamera.h"
#include "Utilities.h"

#include <glm/gtc/matrix_transform.hpp>

//#include <algorithm>

Element::EleCamera::EleCamera()
{
    worldUp = Vec3(0.0f, 1.0f, 0.0f );

    viewport = { 0, 0, 1, 1 };
    rect = { 0, 0, 1, 1 };
    zoom = 1.f;
}

Element::EleCamera::EleCamera(Element::CameraType _type) {
    type = _type;
    transform.setPosition(0.0f, 0.0f, -5.0f);
    worldUp = Vec3( 0.0f, 1.0f, 0.0f );

    viewport = { 0, 0, 1, 1 };
    rect = { 0, 0, 1, 1 };
    zoom = 1.f;
    cameraChanged = true;
}


Element::EleCamera::EleCamera(CameraType _type, VknPipeline* pipeline)
{
    type = _type;
    transform.setPosition(0.0f, 0.0f, -5.0f);
    worldUp = Vec3( 0.0f, 1.0f, 0.0f );

    viewport = { 0, 0, 1, 1 };
    rect = { 0, 0, 1, 1 };
    zoom = 1.f;
    cameraChanged = true;

//    uniformBuffers.resize(3);
//    VkDeviceSize bufferSize = sizeof(UniformBufferViewObject);
//    for (auto& buffer : uniformBuffers) {
//        for (auto& buffer : uniformBuffers)
//        {
//            buffer.Create(bufferSize, 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
//            buffer.Map();
//        }
//    }
}

const glm::vec4& Element::EleCamera::getViewport() const
{
    return viewport;
}

const glm::vec4& Element::EleCamera::getRect() const
{
    return rect;
}

const glm::mat4& Element::EleCamera::getViewMatrix() const
{
    return viewMatrix;
}

const glm::mat4& Element::EleCamera::getProjMatrix() const
{
    return projectionMatrix;
}

void Element::EleCamera::setRect(glm::vec4 _rect)
{
    rect = _rect;
    cameraChanged = true;
}

void Element::EleCamera::setViewport(glm::vec4 _viewport)
{
    viewport = _viewport;
    cameraChanged = true;
}

void Element::EleCamera::update(float windowWidth, float windowHeight, uint32_t imageIndex)
{
    if (!cameraChanged) return;

    transformMatrix = glm::translate(glm::mat4(1.0f), Utilities::vec3RefToGlmvec3(transform.getPosition()));

    transformMatrix = glm::rotate(transformMatrix, glm::radians(transform.getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
    transformMatrix = glm::rotate(transformMatrix, glm::radians(transform.getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    transformMatrix = glm::rotate(transformMatrix, glm::radians(transform.getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));

    inverted = glm::inverse(transformMatrix);
    right = Utilities::glmvec3RefToVec3(normalize(glm::vec3(inverted[0])));
    up = Utilities::glmvec3RefToVec3(normalize(glm::vec3(-inverted[1])));
    forward = Utilities::glmvec3RefToVec3(normalize(glm::vec3(inverted[2])));

    lookAtPos = transform.getPosition() + forward;
    viewMatrix = glm::lookAt(Utilities::vec3RefToGlmvec3(transform.getPosition()),
                             Utilities::vec3RefToGlmvec3(lookAtPos),
                             Utilities::vec3RefToGlmvec3(up));

    auto widthLonger = windowWidth >= windowHeight;
    auto aspectRatio = widthLonger ? windowWidth / windowHeight : windowHeight / windowWidth;
    if (type == CameraType::PERSPECTIVE)
    {
        projectionMatrix = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 1000.0f);
    }
    else
    {
        auto x = (widthLonger ? aspectRatio : 1.0f);
        auto y = (widthLonger ? 1.0f : aspectRatio);

        projectionMatrix = glm::ortho(-x * zoom, x * zoom, -y * zoom, y * zoom, 0.1f, 1000.0f);
    }

    //projectionMatrix = glm::ortho(-zoom * windowWidth, windowWidth * zoom, -zoom * windowHeight,  windowHeight * zoom, 0.1f, 1000.0f);
    projectionMatrix[1][1] *= -1;
}
