#include "VknSwapChain.h"
#include <Vulkan/VkFunctions.h>
#include <Vulkan/Device/Device.h>

#include <GLFW/glfw3.h>

#include <stdexcept>

Element::VknSwapChain::VknSwapChain(GLFWwindow* window, VkSurfaceKHR surface)
{
    init(window, surface);
}

Element::VknSwapChain::~VknSwapChain()
{
    int x = 0;
}

void Element::VknSwapChain::init(GLFWwindow* window, VkSurfaceKHR surface)
{
    createSwapChain(window, surface);
    createImageViews();
    createColourImage();
    createDepthImage();
}

VkSwapchainKHR Element::VknSwapChain::VkSwapChain()
{
    return m_swapChain;
}

const std::vector<VkImage>& Element::VknSwapChain::Images()
{
    return m_images;
}

const std::vector<VkImageView>& Element::VknSwapChain::ImageViews()
{
    return m_imageViews;
}

VkExtent2D Element::VknSwapChain::Extent()
{
    return m_extent;
}

VkFormat Element::VknSwapChain::ImageFormat()
{
    return m_imageFormat;
}

uint32_t Element::VknSwapChain::getImageCount()
{
    return m_imageCount;
}

uint32_t Element::VknSwapChain::imageIndex()
{
    return m_imageIndex;
}

void Element::VknSwapChain::Destroy()
{
    if(!created)
        return;

    created = false;
    auto logicalDevice = Device::getVkDevice();

    vkDestroyImageView(logicalDevice, m_colourImageView, nullptr);
    m_colourImage.Destroy();
    vkDestroyImageView(logicalDevice, m_depthImageView, nullptr);
    m_depthImage.Destroy();
    for (auto imageView : m_imageViews) {
        vkDestroyImageView(logicalDevice, imageView, nullptr);
    }

    vkDestroySwapchainKHR(logicalDevice, m_swapChain, nullptr);
}

void Element::VknSwapChain::createSwapChain(GLFWwindow* window, VkSurfaceKHR surface) {

    if(created)
        return;

    auto logicalDevice = Device::getVkDevice();
    auto physicalDevice = Device::GetPhysicalDevice()->GetSelectedDevice(true);
    //SwapChainSupportDetails swapChainSupport = Element::VkFunctions::querySwapChainSupport(physicalDevice, surface);

    VkSurfaceFormatKHR surfaceFormat =
            Element::VkFunctions::chooseSwapSurfaceFormat(physicalDevice.swapChainSupport.formats);
    VkPresentModeKHR presentMode =
            Element::VkFunctions::chooseSwapPresentMode(physicalDevice.swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(window,
                                         physicalDevice.swapChainSupport.capabilities);

    m_imageCount = physicalDevice.swapChainSupport.capabilities.minImageCount + 1;
    if (physicalDevice.swapChainSupport.capabilities.maxImageCount > 0 &&
    m_imageCount > physicalDevice.swapChainSupport.capabilities.maxImageCount) {
        m_imageCount = physicalDevice.swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = m_imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueIndices indices = VkFunctions::findQueueFamilies(physicalDevice.m_physicalDevice, surface);
    uint32_t queueFamilyIndices[] = { indices.graphics.value(), indices.present.value() };

    if (indices.graphics != indices.present) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = physicalDevice.swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    if (vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(logicalDevice, m_swapChain, &m_imageCount, nullptr);
    m_images.resize(m_imageCount);
    vkGetSwapchainImagesKHR(logicalDevice, m_swapChain, &m_imageCount, m_images.data());

    m_imageFormat = surfaceFormat.format;
    m_extent = extent;
    created = true;
}

void Element::VknSwapChain::createImageViews() {
    m_imageViews.resize(m_images.size());
    const auto& logicalDevice = Device::getVkDevice();
    for (uint32_t i = 0; i < m_images.size(); i++) {
        m_imageViews[i] = VkFunctions::createImageView(logicalDevice, m_images[i], 1, m_imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

VkExtent2D Element::VknSwapChain::chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    if (static_cast<uint32_t>(width) >= UINT32_MAX) {
        return capabilities.currentExtent;
    }
    else {

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

void Element::VknSwapChain::createDepthImage() {

    m_depthImage.Create(m_extent.width, m_extent.height, 1,
                        Device::GetPhysicalDevice()->GetSelectedDevice().requiredFormats.at("depth"),
                        VK_IMAGE_TILING_OPTIMAL,
                        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, Device::GetPhysicalDevice()->GetSelectedDevice().msaaSamples);
    m_depthImageView = VkFunctions::createImageView(Device::getVkDevice(), m_depthImage.m_vkImage, 1, m_depthImage.m_format, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void Element::VknSwapChain::createColourImage() {

    m_colourImage.Create(m_extent.width, m_extent.height, 1, m_imageFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, Device::GetPhysicalDevice()->GetSelectedDevice().msaaSamples);
    m_colourImageView = VkFunctions::createImageView(Device::getVkDevice(), m_colourImage.m_vkImage, 1, m_colourImage.m_format, VK_IMAGE_ASPECT_COLOR_BIT);
}

VkResult Element::VknSwapChain::AquireNextImage(VkSemaphore& graphicSemaphore, VkFence fence)
{
    const auto logicalDevice = Device::getVkDevice();
    vkWaitForFences(logicalDevice, 1, &fence, VK_TRUE, UINT64_MAX);

    return vkAcquireNextImageKHR(logicalDevice, m_swapChain, UINT64_MAX, graphicSemaphore, VK_NULL_HANDLE, &m_imageIndex);
}

VkResult Element::VknSwapChain::Present(VkSemaphore& presentSemaphore)
{
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &presentSemaphore;

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapChain;

    presentInfo.pImageIndices = &m_imageIndex;

    return vkQueuePresentKHR(Device::GetPresentQueue(), &presentInfo);
}

bool Element::VknSwapChain::isCreated() const {
    return created;
}
