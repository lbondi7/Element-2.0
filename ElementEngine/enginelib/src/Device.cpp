#include "Device.h"

#include "VkFunctions.h"

#include <stdexcept>
#include <set>
#include <string>
#include <iostream>

std::unique_ptr<Element::PhysicalDevice> Element::Device::m_phyicalDevice;
std::unique_ptr<Element::LogicalDevice> Element::Device::m_logicalDevice;
std::unique_ptr<Element::CommandPool> Element::Device::commandPool;

void Element::Device::CreateLogicalDevice(VkSurfaceKHR surface)
{
    findQueueFamilies(surface);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    //std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
    std::set<uint32_t> uniqueQueueFamilies = { queueFamilyIndices.graphics.value(), queueFamilyIndices.present.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
   

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(m_phyicalDevice->GetSelectedDevice().m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(m_device, queueFamilyIndices.graphics.value(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, queueFamilyIndices.present.value(), 0, &m_presentQueue);


    //m_commandPool = createCommandPool(queueFamilyIndices.graphics.value());

}
//
//void Element::Device::CreateLogicalDevice(VkSurfaceKHR surface)
//{
//    findQueueFamilies(surface);
//
//    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
//    //std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
//    std::set<uint32_t> uniqueQueueFamilies = { queueFamilyIndices.graphics.value(), queueFamilyIndices.present.value() };
//
//    float queuePriority = 1.0f;
//    for (uint32_t queueFamily : uniqueQueueFamilies) {
//        VkDeviceQueueCreateInfo queueCreateInfo{};
//        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//        queueCreateInfo.queueFamilyIndex = queueFamily;
//        queueCreateInfo.queueCount = 1;
//        queueCreateInfo.pQueuePriorities = &queuePriority;
//        queueCreateInfos.push_back(queueCreateInfo);
//    }
//
//    VkPhysicalDeviceFeatures deviceFeatures{};
//    deviceFeatures.samplerAnisotropy = VK_TRUE;
//
//
//    VkDeviceCreateInfo createInfo{};
//    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//
//    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
//    createInfo.pQueueCreateInfos = queueCreateInfos.data();
//
//    createInfo.pEnabledFeatures = &deviceFeatures;
//
//    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
//    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
//
//
//    if (enableValidationLayers) {
//        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
//        createInfo.ppEnabledLayerNames = validationLayers.data();
//    }
//    else {
//        createInfo.enabledLayerCount = 0;
//    }
//
//    if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
//        throw std::runtime_error("failed to create logical device!");
//    }
//
//    vkGetDeviceQueue(m_device, queueFamilyIndices.graphics.value(), 0, &m_graphicsQueue);
//    vkGetDeviceQueue(m_device, queueFamilyIndices.present.value(), 0, &m_presentQueue);
//
//
//    m_commandPool = createCommandPool(queueFamilyIndices.graphics.value());
//
//}

void Element::Device::PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device, surface)) {
            m_physicalDevice = device;
            break;
        }
    }

    if (m_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}


bool Element::Device::isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    QueueIndices indices = Element::VkFunctions::findQueueFamilies(physicalDevice, surface);

    bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = Element::VkFunctions::querySwapChainSupport(physicalDevice, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);

    return indices.canRender() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

bool Element::Device::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
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

VkCommandPool Element::Device::creatCommandPool(uint32_t queueFamily, VkCommandPoolCreateFlags flags) {

    VkCommandPool commandPool;
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamily;
    poolInfo.flags = flags;

    if (vkCreateCommandPool(getVkDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics command pool!");
    }

    return commandPool;
}

void Element::Device::findQueueFamilies(VkSurfaceKHR surface) {


    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_phyicalDevice->GetSelectedDevice().m_physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_phyicalDevice->GetSelectedDevice().m_physicalDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueFamilyIndices.graphics = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_phyicalDevice->GetSelectedDevice().m_physicalDevice, i, surface, &presentSupport);

        if (presentSupport) {
            queueFamilyIndices.present = i;
        }

        if (queueFamilyIndices.graphics.has_value() && queueFamilyIndices.present.has_value()) {
            return;
        }

        i++;
    }
}

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
