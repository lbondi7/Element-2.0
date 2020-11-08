#pragma once

#include "VknConstants.h"

#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "CommandPool.h"
#include <memory>

namespace Element {

	struct Device
	{
		void CreateLogicalDevice(VkSurfaceKHR surface);

		void PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

		bool isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

		bool checkDeviceExtensionSupport(VkPhysicalDevice device);

		//VkCommandPool createCommandPool(uint32_t queueFamily, VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

		static VkCommandPool creatCommandPool(uint32_t queueFamily, VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);


		void findQueueFamilies(VkSurfaceKHR surface);

		VkDevice m_device = VK_NULL_HANDLE;
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

		VkQueue m_graphicsQueue = VK_NULL_HANDLE;
		VkQueue m_presentQueue = VK_NULL_HANDLE;

		VkCommandPool m_commandPool;

		struct
		{
			std::optional<uint32_t> graphics;
			std::optional<uint32_t> present;
			std::optional<uint32_t> compute;
			std::optional<uint32_t> transfer;
		} queueFamilyIndices;


		static void setupPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
		static void setupLogicalDevice(VkSurfaceKHR surface);
		static void destroy();

		static PhysicalDevice* GetPhysicalDevice();
		static LogicalDevice* GetLogicalDevice();
		static CommandPool* GetCommandPool();

		static VkPhysicalDevice GetVkPhysicalDevice();
		static VkDevice getVkDevice();
		static VkQueue GetGraphicsQueue();
		static VkQueue GetComputeQueue();
		static VkQueue GetPresentQueue();

		static std::unique_ptr<PhysicalDevice> m_phyicalDevice;
		static std::unique_ptr<LogicalDevice> m_logicalDevice;
		static std::unique_ptr<CommandPool> commandPool;
	};
}

