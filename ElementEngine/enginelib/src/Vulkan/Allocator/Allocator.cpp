//
// Created by lbondi7 on 25/11/2020.
//

#include "Allocator.h"

#include "Device/Device.h"
#include "VkFunctions.h"
#include "VknBuffer.h"

#include <element/Debugger.h>

const int ALLOC_SIZE = 1000;

Element::Allocator::Allocator() {}

Element::Allocator::~Allocator() {

}

Element::VknMemoryData &Element::Allocator::getMemory(const VkMemoryRequirements& memRequirements,
                                                      VkMemoryPropertyFlags properties) {
    const auto& logicalDevice = Device::getVkDevice();
    const auto& physicalDevice = Device::GetPhysicalDevice()->GetSelectedDevice();

    for (auto& mem : memoryPool) {
        if (mem.properties == properties &&
        memRequirements.size + mem.currentOffset < mem.maxSize) {
            return mem;
        }
    }
    auto& mem = memoryPool.emplace_back();
    mem.id = availableMemoryID++;
    mem.properties = properties;
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = mem.maxSize > memRequirements.size ? mem.maxSize : memRequirements.size;
    allocInfo.memoryTypeIndex = VkFunctions::GetMemoryType(
            physicalDevice.m_memoryProperties, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &mem.memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    return mem;
}

void Element::Allocator::Allocate(VkDescriptorBufferInfo& bufferInfo, VkDeviceMemory& memory,
                                  VkDeviceSize& offset, VkDeviceSize size, VkBufferUsageFlags usage,
                                  VkMemoryPropertyFlags properties) {

    const auto& logicalDevice = Device::getVkDevice();
    const auto& physicalDevice = Device::GetPhysicalDevice()->GetSelectedDevice();

    auto& buffer = getBuffer(bufferInfo, offset, size, usage, properties);
    bufferInfo.buffer = buffer.vk_buffer;
    bufferInfo.range = size;
    memory = buffer.memoryData->memory;
}

Element::VknBufferData &Element::Allocator::getBuffer(VkDescriptorBufferInfo& descInfo, VkDeviceSize& offset,
                                                      VkDeviceSize size,
                                                      VkBufferUsageFlags usage,
                                                       VkMemoryPropertyFlags properties) {

    const auto& logicalDevice = Device::getVkDevice();
    const auto& physicalDevice = Device::GetPhysicalDevice()->GetSelectedDevice();

    auto minAlignment = physicalDevice.m_properties.limits.minUniformBufferOffsetAlignment;
    int amount = static_cast<int>(std::ceil(static_cast<float>(size) / static_cast<float>(minAlignment)));
    minAlignment = minAlignment * static_cast<VkDeviceSize>(amount);

    for (auto& buffer : bufferPool) {
        if (usage == buffer.usage && (minAlignment + buffer.size <= buffer.maxSize)) {
            descInfo.offset = buffer.size;
            offset = buffer.memoryData->currentOffset;
            buffer.size += minAlignment;
            buffer.memoryData->currentOffset += minAlignment;
            return buffer;
        }
    }

    auto& buffer = bufferPool.emplace_back();
    buffer.usage = usage;
    buffer.id = availableBufferID++;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = buffer.maxSize > size ? buffer.maxSize : size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer.vk_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements{};
    vkGetBufferMemoryRequirements(logicalDevice, buffer.vk_buffer, &memRequirements);

    auto& memoryData = getMemory(memRequirements, properties);

    vkBindBufferMemory(logicalDevice, buffer.vk_buffer, memoryData.memory, 0);
    offset = memoryData.currentOffset;
    descInfo.offset = buffer.size;
    buffer.size += minAlignment;
    buffer.memoryData = &memoryData;

    memoryData.currentOffset += minAlignment;
    return buffer;
}

void Element::Allocator::deallocate() {
    const auto& logicalDevice = Device::getVkDevice();

    for (auto& buffer : bufferPool) {
        buffer.memoryData = nullptr;
        if (buffer.vk_buffer)
        {
            vkDestroyBuffer(logicalDevice, buffer.vk_buffer, nullptr);
            buffer.vk_buffer = nullptr;
        }
    }

    for (auto& memory : memoryPool) {
        if (memory.memory)
        {
            vkFreeMemory(logicalDevice, memory.memory, nullptr);
            memory.memory = nullptr;
        }
    }

}
