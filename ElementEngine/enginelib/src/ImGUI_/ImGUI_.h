#pragma once

#include <Vulkan/VknConstants.h>
#include <Vulkan/VknBuffer.h>
#include <Vulkan/VknImage.h>

namespace Element {

	class ImGUI
	{
	public:
		// UI params are set via push constants
		struct PushConstBlock {
			glm::vec2 scale;
			glm::vec2 translate;
		} pushConstBlock;


		static ImGUI& Get() {
			static ImGUI imGui;
			imGui.started = true;
			return imGui;
		}

		// Initialize styles, keys, etc.
		void init();

		// Initialize all Vulkan resources used by the ui
		void initResources(VkRenderPass renderPass);

		void newFrame();

		void start();

		void end();

		void updateBuffers();

		void drawFrame(VkCommandBuffer commandBuffer);

		void deint();

		bool isPrepared();

		void updateMouse(float windowWidth, float windowHeight);

	private:
		VkSampler sampler;
		VknBuffer vertexBuffer;
		VknBuffer indexBuffer;
		int32_t vertexCount = 0;
		int32_t indexCount = 0;
		VkDeviceMemory fontMemory = VK_NULL_HANDLE;
		VknImage fImage;
		VkImage fontImage = VK_NULL_HANDLE;
		VkImageView fontView = VK_NULL_HANDLE;
		VkPipelineCache pipelineCache;
		VkPipelineLayout pipelineLayout;
		VkPipeline pipeline;
		VkDescriptorPool descriptorPool;
		VkDescriptorSetLayout descriptorSetLayout;
		VkDescriptorSet descriptorSet;

		bool started = false;
		bool prepared = false;

	};
}
