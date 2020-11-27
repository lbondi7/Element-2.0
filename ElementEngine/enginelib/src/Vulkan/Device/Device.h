#pragma once

#include <Vulkan/VknConstants.h>

#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include <Vulkan/CommandPool/VknCommandPool.h>

#include <memory>

namespace Element {

	struct Device
	{
//		struct
//		{
//			std::optional<uint32_t> graphics;
//			std::optional<uint32_t> present;
//			std::optional<uint32_t> compute;
//			std::optional<uint32_t> transfer;
//		} queueFamilyIndices;


		static void setupPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
		static void setupLogicalDevice(VkSurfaceKHR surface);
		static void destroy();

		static PhysicalDevice* GetPhysicalDevice();
		static LogicalDevice* GetLogicalDevice();
		static VknCommandPool* GetCommandPool();

		static VkPhysicalDevice GetVkPhysicalDevice();
		static VkDevice getVkDevice();
		static VkQueue GetGraphicsQueue();
		static VkQueue GetComputeQueue();
		static VkQueue GetPresentQueue();

		static std::unique_ptr<PhysicalDevice> m_phyicalDevice;
		static std::unique_ptr<LogicalDevice> m_logicalDevice;
		static std::unique_ptr<VknCommandPool> commandPool;
	};
}

