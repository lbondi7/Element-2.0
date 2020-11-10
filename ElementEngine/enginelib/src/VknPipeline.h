#pragma once

#include "VknConstants.h"
#include "VknRenderPass.h"
#include "VknSwapChain.h"
#include "Shader.h"
#include "Pipeline.h"

#include <element/ElementConstants.h>

#include "PipelineData.h"
#include "ShaderInfo.h"
#include "VknDescriptorPool.h"

#include <vector>

namespace Element {

	struct VknPipeline
	{
	public:
		VknPipeline() = default;
		VknPipeline(SwapChain* swapChain, RenderPass* renderPass, const std::string& name, const PipelineData& pipelineInfo);
		~VknPipeline();

		void init();

		void destroy();

		void reInitVknPipeline(SwapChain* swapChain, RenderPass* renderPass);

		void flush();

		void bind(VkCommandBuffer vkCommandBuffer);

		VkDescriptorSetLayout GetVkDescriptorSetLayout();
		VkPipeline GetVkPipeline();
		VkPipelineLayout GetVkPipelineLayout();
		VkDescriptorPool GetVkDescriptorPool();

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
		SwapChain* m_swapChain;
		RenderPass* m_renderPass;
		VkDescriptorSetLayout m_descriptorSetLayout{};
		VkPipelineLayout m_pipelineLayout{};
		VkPipeline m_vkPipeline{};
		VkDescriptorPool m_descriptorPool{};
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		PipelineData m_pipelineData;
		bool flushed = false;
		bool bound = false;

		std::vector<VknDescriptorPool*> m_descriptorPools;

		std::string name;
	};
}