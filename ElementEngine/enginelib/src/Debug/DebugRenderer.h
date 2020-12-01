#pragma once

#include <Vulkan/VknConstants.h>
#include <Vulkan/VknBuffer.h>
#include <Vulkan/VknImage.h>
#include <Vulkan/Pipeline/VknDescriptorSet.h>
#include <Vulkan/Pipeline/VknPipeline.h>
#include <Vulkan/RenderPass/VknRenderPass.h>

#include <string>

namespace Element {
	class DebugRenderer
	{
	private:

		bool invalid = false;

		bool renderColliders = false;
    public:

		DebugRenderer() = default;
		~DebugRenderer() = default;

		void init(VknSwapChain* swapChain, VknRenderPass* renderPass);

		void update(float windowWidth, float windowHeight);

		void draw3D(VkCommandBuffer commandBuffer, VkDescriptorSet camDescSet, uint32_t i);

		void drawImGUI(VkCommandBuffer commandBuffer);

		void deint();

		bool isInvalid();

        bool isRenderColliders() const;

        void setRenderColliders(bool renderColliders);

		std::unique_ptr<VknPipeline> pipeline;

		VknSwapChain* swapChain;


		std::vector<VknBuffer> uniformBuffers;

        std::unique_ptr<VknDescriptorSet> descriptorSet;

	};

}

