#include "VknSwapChain.h"
#include "VkFunctions.h"

#include <stdexcept>


Element::SwapChain::SwapChain(GLFWwindow* window, VkSurfaceKHR surface)
{
    init(window, surface);
}

Element::SwapChain::~SwapChain()
{
    int x = 0;
}

void Element::SwapChain::init(GLFWwindow* window, VkSurfaceKHR surface)
{
    createSwapChain(window, surface);
    createImageViews();
    createColourImage();
    createDepthImage();
}

VkSwapchainKHR Element::SwapChain::VkSwapChain()
{
    return m_swapChain;
}

const std::vector<VkImage>& Element::SwapChain::Images()
{
    return m_images;
}

const std::vector<VkImageView>& Element::SwapChain::ImageViews()
{
    return m_imageViews;
}

VkExtent2D Element::SwapChain::Extent()
{
    return m_extent;
}

VkFormat Element::SwapChain::ImageFormat()
{
    return m_imageFormat;
}

uint32_t Element::SwapChain::getImageCount()
{
    return m_imageCount;
}

uint32_t& Element::SwapChain::CurrentImageIndex()
{
    return m_imageIndex;
}

//void Element::SwapChain::DestroyDepthResource()
//{
//    vkDestroyImageView(Device::getVkDevice(), m_depthImageView, nullptr);
//    m_depthImage.Destroy();
//}
//
//
//void Element::SwapChain::DestroyColourResource()
//{
//    vkDestroyImageView(Device::getVkDevice(), m_colourImageView, nullptr);
//    m_colourImage.Destroy();
//}

void Element::SwapChain::Destroy()
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

void Element::SwapChain::createSwapChain(GLFWwindow* window, VkSurfaceKHR surface) {

    if(created)
        return;

    auto logicalDevice = Device::getVkDevice();
    auto physicalDevice = Device::GetVkPhysicalDevice();

    SwapChainSupportDetails swapChainSupport = Element::VkFunctions::querySwapChainSupport(physicalDevice, surface);

    VkSurfaceFormatKHR surfaceFormat = Element::VkFunctions::chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = Element::VkFunctions::chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(window, swapChainSupport.capabilities);

    m_imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && m_imageCount > swapChainSupport.capabilities.maxImageCount) {
        m_imageCount = swapChainSupport.capabilities.maxImageCount;
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

    QueueIndices indices = Element::VkFunctions::findQueueFamilies(physicalDevice, surface);
    uint32_t queueFamilyIndices[] = { indices.graphics.value(), indices.present.value() };

    if (indices.graphics != indices.present) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
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

void Element::SwapChain::createImageViews() {
    m_imageViews.resize(m_images.size());
    const auto& logicalDevice = Device::getVkDevice();
    for (uint32_t i = 0; i < m_images.size(); i++) {
        m_imageViews[i] = Element::VkFunctions::createImageView(logicalDevice, m_images[i], 1, m_imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

VkExtent2D Element::SwapChain::chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }
    else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

void Element::SwapChain::createDepthImage() {

    m_depthImage.Create(m_extent.width, m_extent.height, 1, findDepthFormat(), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, Device::GetPhysicalDevice()->GetSelectedDevice().msaaSamples);
    m_depthImageView = Element::VkFunctions::createImageView(Device::getVkDevice(), m_depthImage.m_vkImage, 1, m_depthImage.m_format, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void Element::SwapChain::createColourImage() {

    m_colourImage.Create(m_extent.width, m_extent.height, 1, m_imageFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, Device::GetPhysicalDevice()->GetSelectedDevice().msaaSamples);
    m_colourImageView = Element::VkFunctions::createImageView(Device::getVkDevice(), m_colourImage.m_vkImage, 1, m_colourImage.m_format, VK_IMAGE_ASPECT_COLOR_BIT);
}

VkResult Element::SwapChain::AquireNextImage(VkSemaphore& graphicSemaphore, VkFence fence)
{
    vkWaitForFences(Device::getVkDevice(), 1, &fence, VK_TRUE, UINT64_MAX);

    return vkAcquireNextImageKHR(Device::getVkDevice(), m_swapChain, UINT64_MAX, graphicSemaphore, VK_NULL_HANDLE, &m_imageIndex);
}

VkResult Element::SwapChain::Present(VkSemaphore& presentSemaphore)
{
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &presentSemaphore;

    VkSwapchainKHR swapChains[] = { m_swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapChain ;

    presentInfo.pImageIndices = &m_imageIndex;

    return vkQueuePresentKHR(Device::GetPresentQueue(), &presentInfo);
}

VkFormat Element::SwapChain::findDepthFormat() {
    return Element::VkFunctions::findSupportedFormat(Device::GetVkPhysicalDevice(),
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

bool Element::SwapChain::isCreated() const {
    return created;
}
