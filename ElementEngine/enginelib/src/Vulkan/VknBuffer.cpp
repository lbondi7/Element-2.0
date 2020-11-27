#include "VknBuffer.h"
#include "VkFunctions.h"

#include <Vulkan/CommandPool/VknCommandBuffer.h>
#include <Device/Device.h>
#include <element/Debugger.h>
#include "Allocator/Allocator.h"

#include <stdexcept>

Element::VknBuffer::VknBuffer(VkDeviceSize size, VkDeviceSize offset, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) : m_size(size), m_usage(usage), m_properties(properties)
{
    Create(size, offset, usage, properties);
}

Element::VknBuffer::~VknBuffer()
{
    Destroy();
}

void Element::VknBuffer::Create(VkDeviceSize size, VkDeviceSize offset, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {

    const auto& logicalDevice = Device::getVkDevice();
    const auto& physicalDevice = Device::GetPhysicalDevice()->GetSelectedDevice();
     
    m_size = size;
    m_properties = properties;
    m_usage = usage;
    m_offset = offset;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(logicalDevice, m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Element::VkFunctions::GetMemoryType(
            physicalDevice.m_memoryProperties, memRequirements.memoryTypeBits, properties);

//    Debugger::get().log("Memory Type Bits",
//                        static_cast<int>(memRequirements.memoryTypeBits), Colour::MAGENTA);
//    Debugger::get().log("Memory Alignment",
//                        static_cast<int>(memRequirements.alignment), Colour::MAGENTA);
//    Debugger::get().log("Memory Size",
//                        static_cast<int>(memRequirements.size), Colour::MAGENTA);
//    Debugger::get().log("Memory Allocation Type index",
//                        static_cast<int>(allocInfo.memoryTypeIndex), Colour::MAGENTA);

    if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(logicalDevice, m_buffer, m_memory, 0);
    updateDescriptorInfo();
}

void Element::VknBuffer::CreateAlloc(VkDeviceSize size, VkDeviceSize offset, VkBufferUsageFlags usage,
                                 VkMemoryPropertyFlags properties) {

    m_properties = properties;
    Allocator::Get().Allocate(m_descriptorInfo, m_memory, m_offset, size, usage, properties);
}

void Element::VknBuffer::Destroy()
{
    auto logicalDevice = Device::getVkDevice();

    //if (isMapped)
    //    Unmap();

    //vkDeviceWaitIdle(logicalDevice);

    if (m_buffer)
    {
        vkDestroyBuffer(logicalDevice, m_buffer, nullptr);
        m_buffer = nullptr;
    }
    if (m_memory)
    {
        vkFreeMemory(logicalDevice, m_memory, nullptr);
        m_memory = nullptr;
    }
}

void Element::VknBuffer::Destroy2()
{
    if (m_memory)
    {
        m_memory = nullptr;
    }
}


void Element::VknBuffer::MapCopyMemory(const void* src_data) {

    Map();
    memcpy(mapped, src_data, static_cast<size_t>(m_size));
    Unmap();
}

void Element::VknBuffer::MapCopyMemory2(const void* src_data) {

    vkMapMemory(Device::getVkDevice(), m_memory, m_offset, m_descriptorInfo.range, 0, &mapped);
    memcpy(mapped, src_data, static_cast<size_t>(m_descriptorInfo.range));
    vkUnmapMemory(Device::getVkDevice(), m_memory);
}

void Element::VknBuffer::CopyMemory(const void* src_data, VkDeviceSize size) {

    memcpy(mapped, src_data, static_cast<size_t>(size));
}


void Element::VknBuffer::Map() {

    vkMapMemory(Device::getVkDevice(), m_memory, m_offset, m_size, 0, &mapped);
    isMapped = true;
}

void Element::VknBuffer::Unmap() {

    if (isMapped)
    {
        vkUnmapMemory(Device::getVkDevice(), m_memory);
        isMapped = false;
        mapped = nullptr;
    }
}

void Element::VknBuffer::CopyFromBuffer(Element::VknBuffer &srcBuffer) {

    VknCommandBuffer commandBuffer(Device::GetCommandPool());

    VkBufferCopy copyRegion{};
    copyRegion.size = srcBuffer.m_size;
    vkCmdCopyBuffer(commandBuffer.GetVkCommandBuffer(), srcBuffer.m_buffer, m_buffer, 1, &copyRegion);

    commandBuffer.SubmitIdle();
}

void Element::VknBuffer::CopyFromBuffer2(Element::VknBuffer &srcBuffer) {

    VknCommandBuffer commandBuffer(Device::GetCommandPool());

    VkBufferCopy copyRegion{};
    copyRegion.size = srcBuffer.m_size;
    copyRegion.dstOffset = m_offset;
    vkCmdCopyBuffer(commandBuffer.GetVkCommandBuffer(), srcBuffer.m_buffer, m_descriptorInfo.buffer, 1, &copyRegion);

    commandBuffer.SubmitIdle();
}

void Element::VknBuffer::updateDescriptorInfo()
{
    m_descriptorInfo.buffer = m_buffer;
    m_descriptorInfo.offset = m_offset;
    m_descriptorInfo.range = m_size;
}
