#pragma once

#include "VknConstants.h"
#include "Image.h"
#include "Device.h"

#include <GLFW/glfw3.h>

namespace Element {

    struct SwapChain
    {
    public:

        SwapChain() = default;
        SwapChain(GLFWwindow* window, VkSurfaceKHR surface);
        ~SwapChain();

        void init(GLFWwindow* window, VkSurfaceKHR surface);

        void createDepthImage();

        void createColourImage();

        VkResult AquireNextImage(VkSemaphore& graphicSemaphore, VkFence fence);

        VkResult Present(VkSemaphore& presentSemaphore);

        VkFormat findDepthFormat();

        VkSwapchainKHR VkSwapChain();
        const std::vector<VkImage>& Images();
        const std::vector<VkImageView>& ImageViews();
        VkExtent2D Extent();
        VkFormat ImageFormat();
        uint32_t getImageCount();
        uint32_t& CurrentImageIndex();

        void DestroyDepthResource();

        void DestroyColourResource();

        void Destroy();
        void createSwapChain(GLFWwindow* window, VkSurfaceKHR surface);
        Element::Image m_depthImage;
        VkImageView m_depthImageView;
        Element::Image m_colourImage;
        VkImageView m_colourImageView;
    private:

        VkSwapchainKHR m_swapChain;
        std::vector<VkImage> m_images;
        VkFormat m_imageFormat;
        VkExtent2D m_extent;
        std::vector<VkImageView> m_imageViews;

        uint32_t m_imageIndex = 0;
        uint32_t m_imageCount = 0;

        void createImageViews();

        VkExtent2D chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities);
    };
}

