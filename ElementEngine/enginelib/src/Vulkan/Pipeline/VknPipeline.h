#pragma once

#include "Pipeline.h"

#include <Vulkan/VknConstants.h>
#include <Vulkan/RenderPass/VknRenderPass.h>
#include <Vulkan/SwapChain/VknSwapChain.h>
#include <Shader/Shader.h>

#include <element/ElementConstants.h>

#include "PipelineData.h"
#include "VknDescriptorPool.h"
#include <Shader/ShaderInfo.h>

#include <vector>

namespace Element {

	struct VknPipeline
	{
	public:
		VknPipeline() = default;
		VknPipeline(VknSwapChain* swapChain, VknRenderPass* renderPass, const std::string& name, const PipelineData& pipelineInfo);
		~VknPipeline();

		void init();

		void destroy();

		void reInitVknPipeline();

		void flush();

		void bind(VkCommandBuffer vkCommandBuffer);

		VkDescriptorSetLayout getVkDescriptorSetLayout(int layout);
		VkPipeline GetVkPipeline();
		VkPipelineLayout GetVkPipelineLayout();
		//VkDescriptorPool GetVkDescriptorPool();

		uint32_t getDescriptorLayoutsCount();

		VkDescriptorPool allocateDescriptorPool(uint32_t descriptorCount);

		void createVknPipeline();

		void createDescriptorSetLayout();

		void createPipelineLayout();

		void createDescriptorPool();

		PipelineData& GetPipelineData();

		std::vector<VkDescriptorSetLayoutBinding> bindingsData;

		bool isBound();

		void setBound(bool _bound);

		const std::string& getName();
	private:
		VknSwapChain* m_swapChain;
		VknRenderPass* m_renderPass;

		VkPipeline m_vkPipeline;
		VkPipelineLayout m_pipelineLayout;
        std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

		PipelineData m_pipelineData;
		bool flushed = false;
		bool bound = false;

        std::vector<std::unique_ptr<VknDescriptorPool>> descriptorPools;
		std::string name;

        void reCreateDescriptorPool();
    };
}