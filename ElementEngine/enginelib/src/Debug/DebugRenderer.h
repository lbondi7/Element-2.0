#pragma once

#include <Vulkan/VknConstants.h>
#include <Vulkan/VknBuffer.h>
#include <Vulkan/VknImage.h>
#include <Vulkan/Pipeline/VknDescriptorSet.h>

#include <string>

namespace Element {
	class DebugRenderer
	{
	private:

		bool invalid = false;

	public:

		DebugRenderer() = default;
		~DebugRenderer() = default;

		void init(VkRenderPass renderPass);

		void update(float windowWidth, float windowHeight);

		void draw(VkCommandBuffer commandBuffer);

		void deint();

		bool isInvalid();
	};

}

