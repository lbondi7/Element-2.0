#include "PhysicalDevice.h"

#include <element/GameSettings.h>

#include "VkFunctions.h"
#include <stdexcept>
#include <set>

Element::PhysicalDevice::PhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    m_physicalDevices.resize(devices.size());

    int i = 0;
    for (const auto& device : devices) {
        m_physicalDevices[i].id = static_cast<uint32_t>(i);
        GetDeviceScore(device, surface, m_physicalDevices[i]);
        m_physicalDevices[i].msaaSamples = getMaxUsableSampleCount(m_physicalDevices[i]);
        ++i;
    }
    int bestScore = 0;
    for (const auto& device : m_physicalDevices) {
        if (bestScore < device.score) {
            m_selectedPhysicalDevice.score = device.score;
            m_selectedPhysicalDevice.m_physicalDevice = device.m_physicalDevice;
            m_selectedPhysicalDevice.id = device.id;
            m_selectedPhysicalDevice.msaaSamples = device.msaaSamples;
            bestScore = device.score;
        }
    }
    //int mssa = static_cast<int>(GameSettings::Instance().msaaLevel);
    vkGetPhysicalDeviceFeatures(m_selectedPhysicalDevice.m_physicalDevice, &m_selectedPhysicalDevice.m_supportedFeatures);
    vkGetPhysicalDeviceProperties(m_selectedPhysicalDevice.m_physicalDevice, &m_selectedPhysicalDevice.m_properties);
    vkGetPhysicalDeviceMemoryProperties(m_selectedPhysicalDevice.m_physicalDevice, &m_selectedPhysicalDevice.m_memoryProperties);

    if (m_selectedPhysicalDevice.m_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

Element::PhysicalDevice::~PhysicalDevice()
{
}

const Element::PhysicalDevice::PhysicalDeviceData& Element::PhysicalDevice::GetSelectedDevice()
{
    return m_selectedPhysicalDevice;

}

VkSampleCountFlagBits Element::PhysicalDevice::getMaxUsableSampleCount(const Element::PhysicalDevice::PhysicalDeviceData& data) {

    VkSampleCountFlags counts = data.m_properties.limits.framebufferColorSampleCounts & data.m_properties.limits.framebufferDepthSampleCounts;

    auto& instance = GameSettings::Instance();
    int multisample = static_cast<int>(instance.msaaLevel);
    if (multisample >= 6 && counts & VK_SAMPLE_COUNT_64_BIT) { 
        instance.msaaLevel = GameSettings::Multisample::MSAA_64;  
        return VK_SAMPLE_COUNT_64_BIT; 
    }
    if (multisample >= 5 && counts & VK_SAMPLE_COUNT_32_BIT) { 
        instance.msaaLevel = GameSettings::Multisample::MSAA_32;
        return VK_SAMPLE_COUNT_32_BIT; 
    }
    if (multisample >= 4 && counts & VK_SAMPLE_COUNT_16_BIT) { 
        instance.msaaLevel = GameSettings::Multisample::MSAA_16;
        return VK_SAMPLE_COUNT_16_BIT; 
    }
    if (multisample >= 3 && counts & VK_SAMPLE_COUNT_8_BIT) { 
        instance.msaaLevel = GameSettings::Multisample::MSAA_8;
        return VK_SAMPLE_COUNT_8_BIT; 
    }
    if (multisample >= 2 && counts & VK_SAMPLE_COUNT_4_BIT) { 
        instance.msaaLevel = GameSettings::Multisample::MSAA_4;
        return VK_SAMPLE_COUNT_4_BIT; 
    }
    if (multisample >= 1 && counts & VK_SAMPLE_COUNT_2_BIT) { 
        instance.msaaLevel = GameSettings::Multisample::MSAA_2;
        return VK_SAMPLE_COUNT_2_BIT; 
    }
    instance.msaaLevel = GameSettings::Multisample::MSAA_1;
    return VK_SAMPLE_COUNT_1_BIT;

}

void Element::PhysicalDevice::PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
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
            m_selectedPhysicalDevice.m_physicalDevice = device;
            break;
        }
    }

    if (m_selectedPhysicalDevice.m_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}


bool Element::PhysicalDevice::isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    QueueIndices indices = Element::VkFunctions::findQueueFamilies(physicalDevice, surface, QueueIndices::QueueType::GRAPHICS_COMPUTE);

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
    
    return indices.canRenderandCompute() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

void Element::PhysicalDevice::GetDeviceScore(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, Element::PhysicalDevice::PhysicalDeviceData& deviceData) {
    QueueIndices indices = Element::VkFunctions::findQueueFamilies(physicalDevice, surface);

    deviceData.score = ScoreDevice(physicalDevice);

    if (deviceData.score == 0){
        return;
    }
    //bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice);

    bool swapChainAdequate = false;
    SwapChainSupportDetails swapChainSupport = Element::VkFunctions::querySwapChainSupport(physicalDevice, surface);
    swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();


    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceData.m_supportedFeatures);
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceData.m_properties);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceData.m_memoryProperties);

    deviceData.m_physicalDevice = physicalDevice;

    if (deviceData.m_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        deviceData.score += 5000;
    }

    if (!indices.canRender() || !swapChainAdequate ||  !deviceData.m_supportedFeatures.samplerAnisotropy) {
        deviceData.score = 0;
        return;
    }
}

uint32_t Element::PhysicalDevice::ScoreDevice(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    uint32_t score = 0;
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    int requredExtensionCount = requiredExtensions.size();
    for (const auto& reqExt : requiredExtensions)
    {
        for (int i = 0; i < availableExtensions.size(); i++)
        {
            //if (i == 43)
            //{
            //    int goop = 0;
            //}
            if (reqExt == availableExtensions[i].extensionName) {
                requredExtensionCount--;
            }
            score += 100;
        }
    }

    return requredExtensionCount == 0 ? score : 0;
}

bool Element::PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
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

//void Element::PhysicalDevice::findQueueFamilies(VkSurfaceKHR surface) {
//
//
//    uint32_t queueFamilyCount = 0;
//    vkGetPhysicalDeviceQueueFamilyProperties(m_selectedPhysicalDevice.m_physicalDevice, &queueFamilyCount, nullptr);
//
//    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
//    vkGetPhysicalDeviceQueueFamilyProperties(m_selectedPhysicalDevice.m_physicalDevice, &queueFamilyCount, queueFamilies.data());
//
//    int i = 0;
//    for (const auto& queueFamily : queueFamilies) {
//        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
//            queueFamilyIndices.graphics = i;
//        }
//
//        VkBool32 presentSupport = false;
//        vkGetPhysicalDeviceSurfaceSupportKHR(m_selectedPhysicalDevice.m_physicalDevice, i, surface, &presentSupport);
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