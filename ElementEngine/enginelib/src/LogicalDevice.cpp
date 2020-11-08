#include "LogicalDevice.h"
#include "VkFunctions.h"
#include <set>
#include <stdexcept>
Element::LogicalDevice::LogicalDevice(PhysicalDevice* _physicalDevice, VkSurfaceKHR surface) : physicalDevice(_physicalDevice)
{
    CreateLogicalDevice(surface);
    //queueFamily = Element::VkFunctions::findQueueFamilies(physicalDevice->GetSelectedDevice().m_physicalDevice, surface, QueueFamilyIndices::QueueType::ALL);

    //std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    //std::set<uint32_t> uniqueQueueFamilies = { queueFamily.graphics.value(), queueFamily.present.value() };

    //float queuePriority = 1.0f;
    //for (uint32_t queueFamily : uniqueQueueFamilies) {
    //    VkDeviceQueueCreateInfo queueCreateInfo{};
    //    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    //    queueCreateInfo.queueFamilyIndex = queueFamily;
    //    queueCreateInfo.queueCount = 1;
    //    queueCreateInfo.pQueuePriorities = &queuePriority;
    //    queueCreateInfos.push_back(queueCreateInfo);
    //}

    //VkPhysicalDeviceFeatures deviceFeatures{};
    //deviceFeatures.samplerAnisotropy = VK_TRUE;


    //VkDeviceCreateInfo createInfo{};
    //createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    //createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    //createInfo.pQueueCreateInfos = queueCreateInfos.data();

    //createInfo.pEnabledFeatures = &deviceFeatures;

    //createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    //createInfo.ppEnabledExtensionNames = deviceExtensions.data();


    //if (enableValidationLayers) {
    //    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    //    createInfo.ppEnabledLayerNames = validationLayers.data();
    //}
    //else {
    //    createInfo.enabledLayerCount = 0;
    //}

    //if (vkCreateDevice(physicalDevice->GetSelectedDevice().m_physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
    //    throw std::runtime_error("failed to create logical device!");
    //}

    //vkGetDeviceQueue(logicalDevice, queueFamily.graphics.value(), 0, &graphicsQueue);
    //vkGetDeviceQueue(logicalDevice, queueFamily.present.value(), 0, &presentQueue);
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

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();


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

//VkCommandPool Element::Device::createCommandPool(uint32_t queueFamily, VkCommandPoolCreateFlags flags) {
//
//    VkCommandPool commandPool;
//    VkCommandPoolCreateInfo poolInfo{};
//    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
//    poolInfo.queueFamilyIndex = queueFamily;
//    poolInfo.flags = flags;
//
//    if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
//        throw std::runtime_error("failed to create graphics command pool!");
//    }
//
//    return commandPool;
//}

//void Element::LogicalDevice::findQueueFamilies(VkSurfaceKHR surface) {
//
//
//    uint32_t queueFamilyCount = 0;
//    vkGetPhysicalDeviceQueueFamilyProperties(m_phyicalDevice->GetSelectedDevice().m_physicalDevice, &queueFamilyCount, nullptr);
//
//    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
//    vkGetPhysicalDeviceQueueFamilyProperties(m_phyicalDevice->GetSelectedDevice().m_physicalDevice, &queueFamilyCount, queueFamilies.data());
//
//    int i = 0;
//    for (const auto& queueFamily : queueFamilies) {
//        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
//            queueFamilyIndices.graphics = i;
//        }
//
//        VkBool32 presentSupport = false;
//        vkGetPhysicalDeviceSurfaceSupportKHR(m_phyicalDevice->GetSelectedDevice().m_physicalDevice, i, surface, &presentSupport);
//
//        if (presentSupport) {
//            queueFamilyIndices.present = i;
//        }
//
//        if (queueFamilyIndices.graphics.has_value() && queueFamilyIndices.present.has_value()) {
//            return;
//        }
//
//        i++;
//    }
//}