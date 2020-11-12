//
// Created by lbondi7 on 08/11/2020.
//

#include "EleCamera.h"
#include "Utilities.h"
#include "Locator.h"
#include "VknResources.h"

#include <glm/gtc/matrix_transform.hpp>

//#include <algorithm>
struct CamConst{
    alignas(16) glm::mat4 view = glm::mat4(1.0f);
    alignas(16) glm::mat4 proj = glm::mat4(1.0f);
    alignas(16) Vec3 camPos = Vec3(0.0f);
    float padding;
};



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

    uniformBuffers.resize(3);
    for (auto& buffer : uniformBuffers)
    {
        buffer.Create(sizeof(CamConst), 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        buffer.Map();
    }
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
        projectionMatrix = glm::perspective(glm::radians(40.f), aspectRatio, 0.1f, 1000.0f);
    }
    else
    {
        auto x = (widthLonger ? aspectRatio : 1.0f);
        auto y = (widthLonger ? 1.0f : aspectRatio);

        projectionMatrix = glm::ortho(-x * zoom, x * zoom, -y * zoom, y * zoom, 0.1f, 1000.0f);
    }

    //projectionMatrix = glm::ortho(-zoom * windowWidth, windowWidth * zoom, -zoom * windowHeight,  windowHeight * zoom, 0.1f, 1000.0f);
    projectionMatrix[1][1] *= -1;


    CamConst camUBO{};
    camUBO.view = viewMatrix;
    camUBO.proj = projectionMatrix;
    camUBO.camPos = transform.getPosition();
    camUBO.camPos.y *= -1;

    uniformBuffers[imageIndex].CopyMemory(&camUBO, sizeof(camUBO));

}

const VkViewport &Element::EleCamera::getVkViewport() const {
    return vkViewport;
}

void Element::EleCamera::setVkViewport(const VkViewport &vkViewport) {
    EleCamera::vkViewport = vkViewport;
}

const VkRect2D &Element::EleCamera::getScissorRect() const {
    return scissorRect;
}

void Element::EleCamera::setScissorRect(const VkRect2D &scissorRect) {
    EleCamera::scissorRect = scissorRect;
}

void Element::EleCamera::setVkViewport(float x,float y,float w,float h, float minDepth, float maxDepth) {

    vkViewport.x = x;
    vkViewport.y = y;
    vkViewport.width = w;
    vkViewport.height = h;
    vkViewport.minDepth = minDepth;
    vkViewport.maxDepth = maxDepth;
}

Element::EleCamera::EleCamera(Element::CameraType _type, const glm::vec4 &viewport, const glm::vec4& rect) :
viewport(viewport), rect(rect){

    type = _type;
    transform.setPosition(0.0f, 0.0f, -5.0f);
    worldUp = Vec3( 0.0f, 1.0f, 0.0f );

    zoom = 1.f;
    cameraChanged = true;
}

void Element::EleCamera::initDescSet(Element::VknPipeline *pipeline, uint32_t imageCount, int id) {
    descriptorSet = Locator::getVknResource()->allocateDescriptorSet();
    descriptorSet->init(pipeline, imageCount, id);
    descriptorSet->addData(uniformBuffers.data(), 0);
    descriptorSet->createDescWritesAndUpdate();

}
