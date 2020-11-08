#include "DebugRenderer.h"

#include <element/ElementConstants.h>
#include <element/Debugger.h>
#include "ImGUI_.h"


void Element::DebugRenderer::init(VkRenderPass renderPass)
{
	ImGUI::Get().init();
	ImGUI::Get().initResources(renderPass);
	ImGUI::Get().start();
	ImGUI::Get().end();
	ImGUI::Get().updateBuffers();
}


void Element::DebugRenderer::update()
{
	ImGUI::Get().updateBuffers();
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