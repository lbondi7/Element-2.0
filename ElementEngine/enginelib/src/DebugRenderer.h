#pragma once

#include "VknConstants.h"
#include "Buffer.h"
#include "Image.h"
#include "DescriptorSet.h"

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

