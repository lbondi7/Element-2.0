#pragma once

#include "VknPipeline.h"
#include "PipelineData.h"

#include <Vulkan/SwapChain/VknSwapChain.h>
#include <Vulkan/RenderPass/VknRenderPass.h>

#include <memory>
#include <map>
#include <string>

namespace Element {

	class PipelineManager
	{
	public:
		PipelineManager(VknSwapChain* _swapChain, VknRenderPass* _renderPass);
		~PipelineManager();

		void destroy();

		void flush();

		void regeneratePipelines();

		void regeneratePipeline(const std::string name);

		VknPipeline* getPipeline(std::string name);

		void generatePipeline(std::string name, const PipelineData& pipelineData);

		VknPipeline* getDefaultPipeline();

		void BindPipeline(VknPipeline* pipeline, VkCommandBuffer commandBuffer);

		void UnbindPipelines();

	private:

		VknSwapChain* swapChain;
		VknRenderPass* renderPass;

		std::map<std::string, std::unique_ptr<VknPipeline>> m_pipelines;
		std::unique_ptr<VknPipeline> m_defaultPipeline;
	};
}