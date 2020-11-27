#pragma once

#include "Vulkan/VknConstants.h"
#include <thread>

namespace Element {

	struct VknCommandPool
	{

	public:

		VknCommandPool(const std::thread::id &threadID = std::this_thread::get_id());
		~VknCommandPool();

		void createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

		void destroyVkCommandPool();

		VkCommandPool GetVkCommandPool();

		VkCommandPool m_commandPool;

	private:
		std::thread::id threadID;
	};
}