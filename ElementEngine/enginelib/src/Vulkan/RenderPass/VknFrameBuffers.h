#pragma once

#include <Vulkan/VknConstants.h>
#include <Vulkan/SwapChain/VknSwapChain.h>

namespace Element {

	class FrameBuffers
	{
	public:

		FrameBuffers(VknSwapChain* swapChain, VkRenderPass vkRenderPass);

		~FrameBuffers();

		void init(VknSwapChain* swapChain, VkRenderPass vkRenderPass);

		void Destroy();

		const std::vector<VkFramebuffer>& GetVkFrameBuffers() const;

		VkFramebuffer GetVkFrameBuffer(int i);

	private:

		std::vector<VkFramebuffer> m_vkFrameBuffers;

	};
}

