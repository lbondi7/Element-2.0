//
// Created by lbondi7 on 09/11/2020.
//

#include "VknDescriptorPool.h"

#include <Vulkan/VkInitializers.h>
#include <Vulkan/VkFunctions.h>
#include <Vulkan/Device/Device.h>

#include <stdexcept>

Element::VknDescriptorPool::~VknDescriptorPool() {

}

void Element::VknDescriptorPool::init(const PipelineData& _pipelineData,
                                      uint32_t imageCount) {
    if(!flushed)
        return;

    flushed = false;
    pipelineData = _pipelineData;
    uint32_t requiredSetInfo = 0;
    std::vector<VkDescriptorPoolSize> poolSizes;
    for (const auto& binding : pipelineData.descriptorInfo)
    {
        auto descriptorCount = imageCount * 300;
        poolSizes.emplace_back(VkInitializers::descriptorPoolSizeCreateInfo(
                VkFunctions::getDescriptorType(binding.bindObjectType), descriptorCount));
        requiredSetInfo += descriptorCount;
    }
    maxSets = requiredSetInfo;
    VkDescriptorPoolCreateInfo poolInfo =
            VkInitializers::descriptorPoolCreateInfo(poolSizes.data(),
                                                     static_cast<uint32_t>(poolSizes.size()),
                                                     maxSets);

    if (vkCreateDescriptorPool(Device::getVkDevice(), &poolInfo, nullptr, &m_vkDescriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void Element::VknDescriptorPool::destroy() {
    vkDestroyDescriptorPool(Device::getVkDevice(), m_vkDescriptorPool, nullptr);
}

VkDescriptorPool Element::VknDescriptorPool::getVkDescriptorPool() {
    return m_vkDescriptorPool;
}

void Element::VknDescriptorPool::flush() {

    if(flushed)
        return;

    flushed = true;
    currentSetCount = 0;
    maxSets = 0;
    vkResetDescriptorPool(Device::getVkDevice(), m_vkDescriptorPool, 0);
}

bool Element::VknDescriptorPool::isFull(uint32_t setCount) {
    if(currentSetCount + setCount >= maxSets)
        return true;
    else
        currentSetCount += setCount;

    return false;
}

bool Element::VknDescriptorPool::isFlushed() const {
    return flushed;
}

