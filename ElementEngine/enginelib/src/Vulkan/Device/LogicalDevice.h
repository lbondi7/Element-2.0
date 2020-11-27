#pragma once

#include "Vulkan/VknConstants.h"
#include "PhysicalDevice.h"

namespace Element {
	class LogicalDevice
	{
	public:

		LogicalDevice(PhysicalDevice* _physicalDevice, VkSurfaceKHR surface);
		~LogicalDevice();

		void CreateLogicalDevice(VkSurfaceKHR surface);
		void Destroy();
		VkDevice GetVkDevice();

		VkQueue GetGraphicsQueue();
		VkQueue GetPresentQueue();
		VkQueue GetComputeQueue();
		VkQueue GetTransferQueue();

		QueueIndices GetQueueIndices();

	private:

		const VkInstance* instance;
		PhysicalDevice* physicalDevice;
		const VkSurfaceKHR* surface;

		VkDevice logicalDevice = VK_NULL_HANDLE;
		VkPhysicalDeviceFeatures enabledFeatures = {};

		VkQueueFlags supportedQueues = {};
		uint32_t graphicsFamily = 0;
		uint32_t presentFamily = 0;
		uint32_t computeFamily = 0;
		uint32_t transferFamily = 0;

		QueueIndices queueIndices;

		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkQueue computeQueue;
		VkQueue transferQueue;
		//VkQueue graphicsQueue = VK_NULL_HANDLE;
		//VkQueue presentQueue = VK_NULL_HANDLE;
		//VkQueue computeQueue = VK_NULL_HANDLE;
		//VkQueue transferQueue = VK_NULL_HANDLE;
	};
}

