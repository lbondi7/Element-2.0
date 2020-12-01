#pragma once

#include <Vulkan/VknConstants.h>
#include <string>
#include <map>

namespace Element {

	class PhysicalDevice
	{
	public:
		struct PhysicalDeviceData {
			VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
			VkPhysicalDeviceProperties m_properties;
			VkPhysicalDeviceMemoryProperties m_memoryProperties;
			VkPhysicalDeviceFeatures m_supportedFeatures;
			VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
            VkSampleCountFlagBits maxMsaaSamples = VK_SAMPLE_COUNT_1_BIT;
            std::map<const std::string, VkFormat> requiredFormats;
			uint16_t score = 0;
			uint32_t id = 0;
			SwapChainSupportDetails swapChainSupport;
			std::vector<const char*> requiredExtensions;
			PhysicalDeviceData& operator=(const PhysicalDeviceData& other) = default;
		};

		PhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
		~PhysicalDevice();

		const PhysicalDeviceData& GetSelectedDevice(bool resetSwapChainDetails = false);

		VkSampleCountFlagBits getMaxUsableSampleCount(Element::PhysicalDevice::PhysicalDeviceData& data);

	private:


        VkSurfaceKHR surface;

		std::vector<PhysicalDeviceData> m_physicalDevices;
		PhysicalDeviceData m_selectedPhysicalDevice;

		void GetDeviceScore(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, Element::PhysicalDevice::PhysicalDeviceData& deviceData);

		uint32_t ScoreDevice(VkPhysicalDevice device);

		bool checkDeviceExtensionSupport(VkPhysicalDevice device);

        bool findSupportedFormat(VkPhysicalDevice physicalDevice, VkFormat& requiredFormat,
                                 const std::vector<VkFormat> &candidates, VkImageTiling tiling,
                                 VkFormatFeatureFlags features);

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    };
}
