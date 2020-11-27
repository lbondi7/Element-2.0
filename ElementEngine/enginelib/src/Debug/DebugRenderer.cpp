#include "DebugRenderer.h"

#include <element/ElementConstants.h>
#include <element/Debugger.h>
#include <ImGUI_/ImGUI_.h>


void Element::DebugRenderer::init(VkRenderPass renderPass)
{
    auto& imgui = ImGUI::Get();

	imgui.init();
	imgui.initResources(renderPass);
	imgui.start();
	imgui.end();
	imgui.updateBuffers();
}


void Element::DebugRenderer::update(float windowWidth, float windowHeight)
{
    auto& imgui = ImGUI::Get();

	imgui.updateMouse(windowWidth, windowHeight);
	imgui.updateBuffers();
}

void Element::DebugRenderer::draw(VkCommandBuffer commandBuffer)
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