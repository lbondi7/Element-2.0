#include "PipelineManager.h"

Element::PipelineManager::PipelineManager(SwapChain* swapChain, RenderPass* renderPass) : swapChain(swapChain), renderPass(renderPass)
{
	//m_pipelines.emplace("default", std::make_unique<VknPipeline>(swapChain, renderPass, "default", *pipelineData));
}

Element::PipelineManager::~PipelineManager()
{
	//for (auto& pipeline : m_pipelines)
	//{
	//	pipeline.second = nullptr;
	//}
	m_pipelines.clear();
}

void Element::PipelineManager::destroy()
{
	for (const auto& pipeline : m_pipelines)
		pipeline.second->destroy();

	m_pipelines.clear();
}

void Element::PipelineManager::flush()
{
	for (const auto& pipeline : m_pipelines)
		pipeline.second->flush();
}

void Element::PipelineManager::regeneratePipelines(SwapChain* _swapChain, RenderPass* _renderPass)
{
	swapChain = _swapChain;
	renderPass = _renderPass;
	for (const auto& pipeline : m_pipelines)
		pipeline.second->reInitVknPipeline(swapChain, renderPass);
}

void Element::PipelineManager::regeneratePipeline(const std::string name)
{
	vkDeviceWaitIdle(Device::getVkDevice());
	m_pipelines[name]->destroy();
	m_pipelines[name]->init();
}

Element::VknPipeline* Element::PipelineManager::getPipeline(std::string name)
{
	for (const auto& pipeline : m_pipelines)
	{
		if (pipeline.first == name)
			return m_pipelines[name].get();
	}
	return nullptr;
}

void Element::PipelineManager::generatePipeline(std::string name, const PipelineData& pipelineData)
{
	for (const auto& pipeline : m_pipelines)
	{
		if (pipeline.first == name)
			return;
	}
//	bool hasVertex = false;
//	bool hasFragment = false;
	
//	for (const auto& shader : pipelineData->shaderInfo)
//	{
//		if (shader.shaderType == ShaderType::VERTEX)
//			hasVertex = true;
//
//		if (shader.shaderType == ShaderType::FRAGMENT)
//			hasFragment = true;
//	}

//	PipelineData newPipelineData{};
//
//	if (!hasFragment || !hasVertex)
//	{
//		newPipelineData = *pipelineData;
//		if (!hasFragment)
//			newPipelineData.shaders.emplace_back(DefaultResources::GetFragmentShader());
//		if (!hasVertex)
//			newPipelineData.shaders.emplace_back(DefaultResources::GetVertexShader());
//	}

	auto newPipeline = std::make_unique<VknPipeline>(swapChain, renderPass, name, pipelineData);
	m_pipelines[name] = std::move(newPipeline);
}

Element::VknPipeline* Element::PipelineManager::getDefaultPipeline()
{
	return m_pipelines["default"].get();
}

void Element::PipelineManager::BindPipeline(VknPipeline* pipeline, VkCommandBuffer commandBuffer)
{
	if (pipeline->isBound())
		return;

	UnbindPipelines();

	pipeline->bind(commandBuffer);
}

void Element::PipelineManager::UnbindPipelines()
{
	for (auto& pipeline : m_pipelines)
		pipeline.second->setBound(false);
}
