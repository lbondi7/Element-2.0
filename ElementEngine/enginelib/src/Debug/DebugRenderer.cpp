#include "DebugRenderer.h"

#include "Physics/Physics.h"

#include <element/ElementConstants.h>
#include <element/Debugger.h>
#include <ImGUI_/ImGUI_.h>


void Element::DebugRenderer::init(VknSwapChain* _swapChain, VknRenderPass* renderPass)
{
    auto& imgui = ImGUI::Get();

	imgui.init();
	imgui.initResources(renderPass->GetVkRenderPass());
	imgui.start();
	imgui.end();
	imgui.updateBuffers();

    swapChain = _swapChain;

	PipelineData pData{};
    DescriptorInfo descriptorInfo = {BindObjectType::STATIC_UNIFORM_BUFFER, ShaderType::VERTEX,
                                     "collider", 1, 0 };
    pData.descriptorInfo.emplace_back(descriptorInfo);

    descriptorInfo = {BindObjectType::STATIC_UNIFORM_BUFFER, ShaderType::FRAGMENT,
                      "collider", 2, 0 };
    pData.descriptorInfo.emplace_back(descriptorInfo);

    pData.polygonMode = PipelinePolygonMode::LINE;
    pData.cullMode = PipelineCulling::NONE;
	pData.lightingEnabled = false;
	pipeline = std::make_unique<VknPipeline>(swapChain, renderPass, "box", pData);


    VkDeviceSize bufferSize = sizeof(glm::vec4);
    uniformBuffers.resize(swapChain->getImageCount());
    glm::vec4 col(0.12f, 1.0f, 0.34f, 1.0f);
    for (auto& buffer : uniformBuffers)
    {
        buffer.CreateAlloc(bufferSize, 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        buffer.MapCopyMemory2(&col);
    }
    descriptorSet= std::make_unique<VknDescriptorSet>(pipeline.get(), swapChain->getImageCount(), 2);
    descriptorSet->addData(uniformBuffers.data(), 0);
    descriptorSet->createDescWritesAndUpdate();
}


void Element::DebugRenderer::update(float windowWidth, float windowHeight)
{
    auto& imgui = ImGUI::Get();

	imgui.updateMouse(windowWidth, windowHeight);
	imgui.updateBuffers();
}

void Element::DebugRenderer::drawImGUI(VkCommandBuffer commandBuffer)
{
	ImGUI::Get().drawFrame(commandBuffer);
}

void Element::DebugRenderer::deint()
{

	ImGUI::Get().deint();
}

bool Element::DebugRenderer::isInvalid()
{
	return invalid;
}

void Element::DebugRenderer::draw3D(VkCommandBuffer commandBuffer, VkDescriptorSet camDescSet, uint32_t i) {

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetVkPipeline());

    std::vector<VkDescriptorSet> descriptorSets;

    auto& colliders = Physics::get().getColliders();

    for (auto& collider : colliders) {
        if (!collider->getDescriptorSet())
            collider->initDescriptorSet(pipeline.get(), swapChain->getImageCount());

            descriptorSets =
                    {camDescSet, collider->getDescriptorSet()->getDescriptorSet(i), descriptorSet->getDescriptorSet(i)};

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipeline->GetVkPipelineLayout(), 0,
                                static_cast<uint32_t>(descriptorSets.size()),
                                descriptorSets.data(), 0, nullptr);

        collider->mesh->bind(commandBuffer);
        collider->mesh->draw(commandBuffer);
    }
}

bool Element::DebugRenderer::isRenderColliders() const {
    return renderColliders;
}

void Element::DebugRenderer::setRenderColliders(bool _renderColliders) {
    renderColliders = _renderColliders;
}
