#include "LogicalDevice.h"
#include "VkFunctions.h"
#include "ValidationLayers.h"

#include <set>
#include <stdexcept>
Element::LogicalDevice::LogicalDevice(PhysicalDevice* _physicalDevice, VkSurfaceKHR surface) : physicalDevice(_physicalDevice)
{
    CreateLogicalDevice(surface);
}

Element::LogicalDevice::~LogicalDevice()
{
    //if(logicalDevice != VK_NULL_HANDLE)
    //    vkDestroyDevice(logicalDevice, nullptr);
}

void Element::LogicalDevice::CreateLogicalDevice(VkSurfaceKHR surface)
{

    queueIndices = Element::VkFunctions::findQueueFamilies(physicalDevice->GetSelectedDevice().m_physicalDevice,  surface, QueueIndices::QueueType::ALL);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { queueIndices.graphics.value(), queueIndices.present.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &physicalDevice->GetSelectedDevice().m_supportedFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(physicalDevice->GetSelectedDevice().requiredExtensions.size());
    createInfo.ppEnabledExtensionNames = physicalDevice->GetSelectedDevice().requiredExtensions.data();


    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice->GetSelectedDevice().m_physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(logicalDevice, queueIndices.graphics.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(logicalDevice, queueIndices.present.value(), 0, &presentQueue);


   // m_commandPool = createCommandPool(queueFamilyIndices.graphics.value());

}

void Element::LogicalDevice::Destroy()
{
    if (logicalDevice != VK_NULL_HANDLE)
        vkDestroyDevice(logicalDevice, nullptr);
}

VkDevice Element::LogicalDevice::GetVkDevice()
{
    return logicalDevice;
}

VkQueue Element::LogicalDevice::GetGraphicsQueue()
{
    return graphicsQueue;
}

VkQueue Element::LogicalDevice::GetPresentQueue()
{
    return presentQueue;
}

VkQueue Element::LogicalDevice::GetComputeQueue()
{
    return computeQueue;
}

VkQueue Element::LogicalDevice::GetTransferQueue()
{
    return transferQueue;
}

QueueIndices Element::LogicalDevice::GetQueueIndices()
{
    return queueIndices;
}