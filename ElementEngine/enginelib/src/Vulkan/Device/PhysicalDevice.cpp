#include "PhysicalDevice.h"

#include <element/GameSettings.h>

#include "Vulkan/VkFunctions.h"
#include <stdexcept>
#include <set>

const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

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
            m_selectedPhysicalDevice = device;
            bestScore = device.score;
        }
    }
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

VkSampleCountFlagBits Element::PhysicalDevice::getMaxUsableSampleCount(Element::PhysicalDevice::PhysicalDeviceData& data) {

    VkSampleCountFlags counts = data.m_properties.limits.framebufferColorSampleCounts & data.m_properties.limits.framebufferDepthSampleCounts;

    data.maxMsaaSamples = (counts & VK_SAMPLE_COUNT_64_BIT) ? VK_SAMPLE_COUNT_64_BIT :
                          (counts & VK_SAMPLE_COUNT_32_BIT) ? VK_SAMPLE_COUNT_32_BIT :
                          (counts & VK_SAMPLE_COUNT_16_BIT) ? VK_SAMPLE_COUNT_16_BIT :
                           (counts & VK_SAMPLE_COUNT_8_BIT) ? VK_SAMPLE_COUNT_8_BIT :
                            (counts & VK_SAMPLE_COUNT_4_BIT) ? VK_SAMPLE_COUNT_4_BIT :
                             (counts & VK_SAMPLE_COUNT_2_BIT) ? VK_SAMPLE_COUNT_2_BIT :
                          VK_SAMPLE_COUNT_1_BIT;
    auto& instance = GameSettings::get();
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

void Element::PhysicalDevice::GetDeviceScore(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, Element::PhysicalDevice::PhysicalDeviceData& deviceData) {
    QueueIndices indices = Element::VkFunctions::findQueueFamilies(physicalDevice, surface);

    deviceData.score = ScoreDevice(physicalDevice);
    deviceData.requiredExtensions = deviceExtensions;
    if (deviceData.score == 0){
        return;
    }

    bool swapChainAdequate = false;
    deviceData.swapChainSupport = querySwapChainSupport(physicalDevice, surface);
    swapChainAdequate = !deviceData.swapChainSupport.formats.empty() && !deviceData.swapChainSupport.presentModes.empty();


    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceData.m_supportedFeatures);
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceData.m_properties);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceData.m_memoryProperties);

    deviceData.m_physicalDevice = physicalDevice;

    if (deviceData.m_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        deviceData.score += 5000;
    }

    bool supportsFormat = findSupportedFormat(physicalDevice, deviceData.requiredFormats["depth"],
                                              { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
                                                 VK_FORMAT_D24_UNORM_S8_UINT },
                                              VK_IMAGE_TILING_OPTIMAL,
                                              VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

    if (!indices.canRender() || !swapChainAdequate ||  !deviceData.m_supportedFeatures.samplerAnisotropy ||
    !supportsFormat) {
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

bool Element::PhysicalDevice::findSupportedFormat(VkPhysicalDevice physicalDevice, VkFormat& requiredFormat, const
std::vector<VkFormat>&
        candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            requiredFormat = format;
            return true;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            requiredFormat = format;
            return true;
        }
    }

    return false;
//    throw std::runtime_error("failed to find supported format!");
}

SwapChainSupportDetails Element::PhysicalDevice::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}