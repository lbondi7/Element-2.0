//
// Created by lbondi7 on 08/11/2020.
//

#include "EleCamera.h"
#include "VknPipeline.h"
#include "Utilities.h"

#include <glm/gtc/matrix_transform.hpp>

//#include <algorithm>
struct CamConst{
    alignas(16) glm::mat4 view = glm::mat4(1.0f);
    alignas(16) glm::mat4 proj = glm::mat4(1.0f);
    alignas(16) Vec3 camPos = Vec3(0.0f);
    float padding;
};


//Element::EleCamera::EleCamera(Element::CameraType _type) {
//    type = _type;
//    transform.setPosition(0.0f, 0.0f, -5.0f);
//    worldUp = Vec3( 0.0f, 1.0f, 0.0f );
//
//    viewport = Vec4(0, 0, 1, 1 );
//    rect = Vec4( 0, 0, 1, 1 );
//    zoom = 1.f;
//    cameraChanged = true;
//
//    uniformBuffers.resize(3);
//    for (auto& buffer : uniformBuffers)
//    {
//        buffer.Create(sizeof(CamConst), 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
//        buffer.Map();
//    }
//}


Element::EleCamera::EleCamera(CameraType _type, VknPipeline* pipeline, uint32_t imageCount)
{
    init(_type, pipeline, imageCount);
}

void Element::EleCamera::init(Element::CameraType _type, Element::VknPipeline *pipeline, uint32_t imageCount) {
    type = _type;
    transform.setPosition(0.0f, 0.0f, -5.0f);
    worldUp = Vec3( 0.0f, 1.0f, 0.0f );

    viewport = Vec4(0, 0, 1, 1 );
    rect = Vec4(0, 0, 1, 1 );
    zoom = 1.f;
    cameraChanged = true;

    enabled = true;

    uniformBuffers.resize(imageCount);
    for (auto& buffer : uniformBuffers)
    {
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
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
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

void Element::EleCamera::initDescSet(Element::VknPipeline *pipeline, uint32_t imageCount, int id) {
//    descriptorSet = Locator::getVknResource()->allocateDescriptorSet();
//    descriptorSet->init(pipeline, imageCount, id);
//    descriptorSet->addData(uniformBuffers.data(), 0);
//    descriptorSet->createDescWritesAndUpdate();

}

void Element::EleCamera::addDescriptorSet(Element::VknPipeline *pipeline, uint32_t imageCount) {
    const auto& name = pipeline->getName();
    descriptorSets.emplace(name, std::make_unique<DescriptorSet>(pipeline, imageCount, 0));
    descriptorSets[name]->addData(uniformBuffers.data(), 0);
    descriptorSets[name]->createDescWritesAndUpdate();
}

Element::DescriptorSet *Element::EleCamera::getDescriptorSet(const std::string& pipelineName) {
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

void Element::EleCamera::setViewportandRect(VkCommandBuffer vkCmdBuffer, Vec2 windowSize) {


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
    vkCmdSetViewport(vkCmdBuffer, 0, 1, &vkViewport);
    vkCmdSetScissor(vkCmdBuffer, 0, 1, &scissorRect);

}



