#pragma once

#include "VknConstants.h"
#include <element/ElementConstants.h>

#include <string>

namespace Element {

	namespace VkFunctions
	{
		VkDescriptorType getDescriptorType(DescriptorType type);


		uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

		uint32_t GetMemoryType(const VkPhysicalDeviceMemoryProperties& memProperties, uint32_t typeFilter, VkMemoryPropertyFlags properties);

		bool checkDeviceExtensionSupport(VkPhysicalDevice device);

		VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool);

		void endSingleTimeCommands(VkDevice device, VkCommandBuffer commandBuffer, VkCommandPool commandPool, VkQueue queue);

		QueueIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

		QueueIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, QueueIndices::QueueType queueType);

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

		VkImageView createImageView(VkDevice device, VkImage image, uint32_t mipLevels, VkFormat format, VkImageAspectFlags aspectFlags);

		bool hasStencilComponent(VkFormat format);

		VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);

		std::vector<char> readFile(const std::string& filename);

	};
}

