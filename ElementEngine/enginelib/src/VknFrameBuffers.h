#pragma once

#include "VknConstants.h"
#include "VknSwapChain.h"
#include "Device.h"

namespace Element {

	class FrameBuffers
	{
	public:

		FrameBuffers(SwapChain* swapChain, VkRenderPass vkRenderPass);

		~FrameBuffers();

		void init(SwapChain* swapChain, VkRenderPass vkRenderPass);

		void Destroy();

		const std::vector<VkFramebuffer>& GetVkFrameBuffers() const;

		VkFramebuffer GetVkFrameBuffer(int i);

	private:

		std::vector<VkFramebuffer> m_vkFrameBuffers;

	};
}

