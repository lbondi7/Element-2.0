#pragma once

#include "VknPipeline.h"
#include "VknSwapChain.h"
#include "VknRenderPass.h"
#include "PipelineData.h"

#include <memory>
#include <map>
#include <string>

namespace Element {

	class PipelineManager
	{
	public:
		PipelineManager(SwapChain* _swapChain, RenderPass* _renderPass);
		~PipelineManager();

		void destroy();

		void flush();

		void regeneratePipelines(SwapChain* _swapChain, RenderPass* _renderPass);

		void regeneratePipeline(const std::string name);

		VknPipeline* getPipeline(std::string name);

		void generatePipeline(std::string name, const PipelineData& pipelineData);

		VknPipeline* getDefaultPipeline();

		void BindPipeline(VknPipeline* pipeline, VkCommandBuffer commandBuffer);

		void UnbindPipelines();

	private:

		SwapChain* swapChain;
		RenderPass* renderPass;

		std::map<std::string, std::unique_ptr<VknPipeline>> m_pipelines;
		std::unique_ptr<VknPipeline> m_defaultPipeline;
	};
}