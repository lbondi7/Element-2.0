#pragma once

#include "VknConstants.h"
#include <thread>

namespace Element {

	struct CommandPool
	{

	public:

		CommandPool(const std::thread::id &threadID = std::this_thread::get_id());
		~CommandPool();

		void createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

		void destroyVkCommandPool();

		VkCommandPool GetVkCommandPool();

		VkCommandPool m_commandPool;

	private:
		std::thread::id threadID;
	};
}