//
// Created by lbondi7 on 01/12/2020.
//

#include "BoxCollider.h"
#include "Misc/Utilities.h"
#include "Misc/Locator.h"
#include "Resource/Resources.h"
#include <Pipeline/VknPipeline.h>

#include <glm/gtc/matrix_transform.hpp>

bool Element::BoxCollider::intersect(const Element::BoxCollider &other) {

    return false;
}

void Element::BoxCollider::init(const glm::mat4 *_objectTransform) {
    objectTransform = _objectTransform;

    uint32_t imageCount = 3;
    uniformBuffers.resize(imageCount);
    VkDeviceSize bufferSize = sizeof(glm::mat4);
    for (auto& buffer : uniformBuffers)
    {
        buffer.CreateAlloc(bufferSize, 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }
    mesh = Locator::getResource()->mesh("cube");
}

void Element::BoxCollider::deInit() {
    objectTransform = nullptr;
    for (auto& buffer : uniformBuffers)
    {
        buffer.Destroy2();
    }
    mesh = nullptr;
}

void Element::BoxCollider::update(uint32_t imageIndex){

    auto pos = Utilities::vec3RefToGlmvec3(transform.getPosition());
    pos.y *= -1;
    transformMatrix = glm::translate(*objectTransform, pos);
    transformMatrix = glm::scale(transformMatrix, Utilities::vec3RefToGlmvec3(transform.getScale()));

    uniformBuffers[imageIndex].MapCopyMemory2(&transformMatrix);
}

void Element::BoxCollider::initDescriptorSet(Element::VknPipeline *pipeline, uint32_t imageCount) {
    descriptorSet = std::make_unique<VknDescriptorSet>(pipeline, imageCount, 1);
    descriptorSet->addData(uniformBuffers.data(), 0);
    descriptorSet->createDescWritesAndUpdate();
}

Element::VknDescriptorSet *Element::BoxCollider::getDescriptorSet() {
    return descriptorSet.get();
}
