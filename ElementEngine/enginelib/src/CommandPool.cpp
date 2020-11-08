#include "CommandPool.h"

#include "VkFunctions.h"
#include "Device.h"

#include <stdexcept>

Element::CommandPool::CommandPool(const std::thread::id& threadID) : threadID(threadID)
{
    //auto logicalDevice = Device::GetLogicalDevice();

    //m_commandPool = Device::creatCommandPool(Device::GetLogicalDevice()->GetQueueIndices().graphics.value());
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = Device::GetLogicalDevice()->GetQueueIndices().graphics.value();
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

    if (vkCreateCommandPool(Device::getVkDevice(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics command pool!");
    }

}

Element::CommandPool::~CommandPool()
{
    //if(m_commandPool != VK_NULL_HANDLE)
    //    vkDestroyCommandPool(Device::GetVkDevice(), m_commandPool, nullptr);
}


VkCommandPool Element::CommandPool::GetVkCommandPool()
{
    return m_commandPool;
}


void Element::CommandPool::createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {

    QueueIndices queueFamilyIndices = Element::VkFunctions::findQueueFamilies(physicalDevice, surface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphics.value();
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics command pool!");
    }
}

void Element::CommandPool::destroyVkCommandPool()
{
    if (m_commandPool != VK_NULL_HANDLE)
        vkDestroyCommandPool(Device::getVkDevice(), m_commandPool, nullptr);
}