#include "Device.h"

#include "VkFunctions.h"

#include <stdexcept>
#include <set>
#include <string>
#include <iostream>

std::unique_ptr<Element::PhysicalDevice> Element::Device::m_phyicalDevice;
std::unique_ptr<Element::LogicalDevice> Element::Device::m_logicalDevice;
std::unique_ptr<Element::CommandPool> Element::Device::commandPool;



void Element::Device::setupPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
{
    m_phyicalDevice = std::make_unique<PhysicalDevice>(instance, surface);
}

VkPhysicalDevice Element::Device::GetVkPhysicalDevice()
{
    return m_phyicalDevice->GetSelectedDevice().m_physicalDevice;
}

Element::PhysicalDevice* Element::Device::GetPhysicalDevice()
{
    return m_phyicalDevice.get();
}

void Element::Device::setupLogicalDevice(VkSurfaceKHR surface)
{
    m_logicalDevice = std::make_unique<LogicalDevice>(m_phyicalDevice.get(), surface);
    commandPool = std::make_unique<CommandPool>();
}

void Element::Device::destroy()
{
    commandPool->destroyVkCommandPool();
    m_logicalDevice->Destroy();
}

VkDevice Element::Device::getVkDevice()
{
    return m_logicalDevice.get()->GetVkDevice();
}

Element::LogicalDevice* Element::Device::GetLogicalDevice()
{
    return m_logicalDevice.get();
}

VkQueue Element::Device::GetGraphicsQueue()
{
    return m_logicalDevice->GetGraphicsQueue();
}

VkQueue Element::Device::GetComputeQueue()
{
    return m_logicalDevice->GetGraphicsQueue();
}

VkQueue Element::Device::GetPresentQueue()
{
    return m_logicalDevice->GetPresentQueue();
}

Element::CommandPool* Element::Device::GetCommandPool()
{
    return commandPool.get();
}
