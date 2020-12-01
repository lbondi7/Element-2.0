#pragma once

#include "VknFrameBuffers.h"

#include <Vulkan/VknConstants.h>
#include <Vulkan/SwapChain/VknSwapChain.h>

#include <memory>

namespace Element {

	class VknRenderPass
	{

	public:

		VknRenderPass() = default;
		VknRenderPass(VkFormat imageFormat);
		VknRenderPass(VknSwapChain* swapChain);
		~VknRenderPass();

		void init(VknSwapChain* swapChain);

		void createVkRenderPass();
		void createFrameBuffers();
		void Destroy();
		void flush();
		void reInit();

		VkRenderPass GetVkRenderPass();

		void begin(VkCommandBuffer vkCommandBuffer, int i);
		void end(VkCommandBuffer vkCommandBuffer);

		void setClearColour(const Vec3& clearColour);

	private:

		VkRenderPass m_vkRenderPass;
		std::unique_ptr<FrameBuffers> m_frameBuffers;
		VknSwapChain* m_swapChain;

		Vec3 m_clearColour = Vec3(0.0f, 0.3f, 0.8f);

		bool created  = false;
	};
}