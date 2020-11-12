#pragma once

#include "VknConstants.h"
#include "VknSwapChain.h"
#include "VknFrameBuffers.h"

#include <memory>
namespace Element {
	class RenderPass
	{

	public:

		RenderPass() = default;
		RenderPass(VkFormat imageFormat);
		RenderPass(SwapChain* swapChain);
		~RenderPass();

		void init(SwapChain* swapChain);

		void createVkRenderPass();
		void createFrameBuffers();
		void Destroy();

		VkRenderPass GetVkRenderPass();

		void begin(VkCommandBuffer vkCommandBuffer, int i);
		void end(VkCommandBuffer vkCommandBuffer);

		void setClearColour(const glm::vec3& clearColour);

	private:

		VkRenderPass m_vkRenderPass;
		std::unique_ptr<FrameBuffers> m_frameBuffers;
		SwapChain* m_swapChain;

		glm::vec3 m_clearColour{0.0f, 0.3f, 0.8f};

		bool created  = false;
	};
}