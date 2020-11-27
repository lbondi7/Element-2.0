#pragma once

#include "Vulkan/VknConstants.h"

#include "VknCommandPool.h"

namespace Element {

	class VknCommandBuffer
	{
	public:
		explicit VknCommandBuffer(VknCommandPool* commandPool, bool begin = true, VkQueueFlagBits queueType = VK_QUEUE_GRAPHICS_BIT, VkCommandBufferLevel bufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		~VknCommandBuffer();

		void destroy();

		void begin(VkCommandBufferUsageFlags usage = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		void end();

		void Submit(const VkSemaphore& waitSemaphore, const VkSemaphore& signalSemaphore, VkFence fence);


		//void Submit(const VkSemaphore& waitSemaphore = VK_NULL_HANDLE, const VkSemaphore& signalSemaphore = VK_NULL_HANDLE, VkFence fence = VK_NULL_HANDLE);

		void SubmitIdle();

		VkCommandBuffer GetVkCommandBuffer();

		VkQueue getQueue();

	private:

		VkQueue queue;
		VkQueueFlagBits queueType;
		VkCommandBuffer m_vkCommandBuffer;
		VknCommandPool* m_commandPool;
		bool m_running = false;

	};
}

