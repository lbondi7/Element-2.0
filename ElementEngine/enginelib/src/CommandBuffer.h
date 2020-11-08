#pragma once

#include "VknConstants.h"

#include "Device.h"
#include "CommandPool.h"

namespace Element {

	class CommandBuffer
	{
	public:
		explicit CommandBuffer(CommandPool* commandPool, bool begin = true, VkQueueFlagBits queueType = VK_QUEUE_GRAPHICS_BIT, VkCommandBufferLevel bufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		~CommandBuffer();

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
		CommandPool* m_commandPool;
		bool m_running = false;

	};
}

