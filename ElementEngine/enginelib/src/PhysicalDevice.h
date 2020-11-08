#pragma once

#include "VknConstants.h"

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
			uint16_t score = 0;
			uint32_t id = 0;
		};

		PhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
		~PhysicalDevice();

		const PhysicalDeviceData& GetSelectedDevice();

		VkSampleCountFlagBits getMaxUsableSampleCount(const Element::PhysicalDevice::PhysicalDeviceData& data);

	private:


		std::vector<PhysicalDeviceData> m_physicalDevices;
		PhysicalDeviceData m_selectedPhysicalDevice;

		void GetDeviceScore(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, Element::PhysicalDevice::PhysicalDeviceData& deviceData);

		uint32_t ScoreDevice(VkPhysicalDevice device);

		void PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

		bool isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	};
}
