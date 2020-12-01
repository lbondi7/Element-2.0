//
// Created by lbondi7 on 08/11/2020.
//

#include "EleCamera.h"
#include <Vulkan/Pipeline/VknPipeline.h>
#include <Misc/Utilities.h>
#include <element/Debugger.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

//#include <algorithm>
struct CamConst{
    alignas(16) glm::mat4 view = glm::mat4(1.0f);
    alignas(16) glm::mat4 proj = glm::mat4(1.0f);
    alignas(16) Vec3 camPos = Vec3(0.0f);
    float padding;
};

Element::EleCamera::EleCamera(ViewType _type, ViewDimension dimension, VknPipeline* pipeline, uint32_t imageCount)
{
    init(_type, dimension, pipeline, imageCount);
}

void Element::EleCamera::init(Element::ViewType _type, ViewDimension _dimension, Element::VknPipeline *pipeline,
                              uint32_t imageCount) {
    type = _type;
    dimension = _dimension;
    transform.setPosition(0.0f, 0.0f, -1.0f);
    worldUp = Vec3( 0.0f, 1.0f, 0.0f );

    viewport = Vec4(0, 0, 1, 1 );
    rect = Vec4(0, 0, 1, 1 );
    zoom = 1.f;
    nearPlane = type == ViewType::PERSPECTIVE ? 0.1f : 0.0f;
    farPlane = 1000.0f;
    cameraChanged = true;
    enabled = true;

    uniformBuffers.resize(imageCount);
    for (auto& buffer : uniformBuffers)
    {
        Debugger::get().log("Camera Buffer", Colour::BRIGHT_CYAN);
        buffer.Create(sizeof(CamConst), 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        buffer.Map();
    }

    addDescriptorSet(pipeline, imageCount);
}

void Element::EleCamera::reInit(uint32_t imageCount) {

    for (auto& descriptor : descriptorSets){
        descriptor.second->init(descriptor.second->getPipeline(), imageCount, descriptor.second->getId());
        descriptor.second->createDescWritesAndUpdate();
    }
}

const glm::mat4& Element::EleCamera::getViewMatrix() const
{
    return viewMatrix;
}

const glm::mat4& Element::EleCamera::getProjMatrix() const
{
    return projectionMatrix;
}

void Element::EleCamera::update(float windowWidth, float windowHeight, uint32_t imageIndex)
{
    if (!cameraChanged) return;

    transformMatrix = glm::translate(glm::mat4(1.0f),
                                     Utilities::vec3RefToGlmvec3(transform.getPosition()));

    transformMatrix *= glm::yawPitchRoll(glm::radians(transform.getRotation().y),
                                         glm::radians(transform.getRotation().x),
                                         glm::radians(transform.getRotation().z));

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
    if (type == ViewType::PERSPECTIVE)
    {
            projectionMatrix =
                    glm::perspective(glm::radians(fov),
                                     aspectRatio,
                                     nearPlane, farPlane);
    }
    else
    {
        auto x = dimension == ViewDimension::_3D ? (widthLonger ? aspectRatio : 1.0f) : windowWidth;
        auto y = dimension == ViewDimension::_3D ? (widthLonger ? 1.0f : aspectRatio) : windowHeight;

        projectionMatrix = glm::ortho(-x * zoom, x * zoom,
                           -y * zoom, y * zoom,
                           nearPlane, farPlane);
    }

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

void Element::EleCamera::initDescSet(Element::VknPipeline *pipeline, uint32_t imageCount, int id) {
//    descriptorSet = Locator::getVknResource()->allocateDescriptorSet();
//    descriptorSet->init(pipeline, imageCount, id);
//    descriptorSet->addData(uniformBuffers.data(), 0);
//    descriptorSet->createDescWritesAndUpdate();

}

void Element::EleCamera::addDescriptorSet(Element::VknPipeline *pipeline, uint32_t imageCount) {
    const auto& name = pipeline->getName();
    descriptorSets.emplace(name, std::make_unique<VknDescriptorSet>(pipeline, imageCount, 0));
    descriptorSets[name]->addData(uniformBuffers.data(), 0);
    descriptorSets[name]->createDescWritesAndUpdate();
}

Element::VknDescriptorSet *Element::EleCamera::getDescriptorSet(const std::string& pipelineName) {
    return descriptorSets[pipelineName].get();
}

Element::EleCamera::~EleCamera() {


}

void Element::EleCamera::destroy() {

    if(destroyed)
        return;

    for (auto& buffer: uniformBuffers)
        buffer.Destroy();

    for (auto& set : descriptorSets)
        set.second->flush();

    descriptorSets.clear();

    destroyed = true;
}

void Element::EleCamera::setViewportAndRect(VkCommandBuffer vkCmdBuffer, Vec2 windowSize) {


    vkViewport.minDepth = 0.0f;
    vkViewport.maxDepth = 1.0f;
    vkViewport.x = viewport.x * windowSize.x;
    vkViewport.y = viewport.y * windowSize.y;
    vkViewport.width = static_cast<float>(viewport.z * windowSize.x);
    vkViewport.height = static_cast<float>(viewport.w * windowSize.y);

    scissorRect.offset = {static_cast<int>(rect.x * windowSize.x),
                          static_cast<int>(rect.y * windowSize.y)};
    scissorRect.extent = {static_cast<uint32_t>(rect.z * windowSize.x),
                          static_cast<uint32_t>(rect.w * windowSize.y)};
//    Debugger::get().log(static_cast<float>(scissorRect.extent.width));
//    Debugger::get().log(static_cast<float>(scissorRect.extent.height));
    vkCmdSetViewport(vkCmdBuffer, 0, 1, &vkViewport);
    vkCmdSetScissor(vkCmdBuffer, 0, 1, &scissorRect);

}


