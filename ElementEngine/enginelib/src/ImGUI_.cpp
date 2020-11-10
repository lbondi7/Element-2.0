#include "ImGUI_.h"

#include "VkFunctions.h"
#include "Resources.h"
#include "Locator.h"
#include <element/Debugger.h>
#include <element/Inputs.h>

#include <imgui.h>
//#include "imgui/imgui.h"

#include <stdexcept>

void Element::ImGUI::init()
{
	if (!started) return;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// Color scheme
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.6f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
	style.Colors[ImGuiCol_Header] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	// Dimensions
	auto& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(1920, 1080);
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
}

void Element::ImGUI::initResources(VkRenderPass renderPass)

{
	if (!started) return;

	const auto& logicalDevice = Device::getVkDevice();
	ImGuiIO& io = ImGui::GetIO();

	// Create font texture
	unsigned char* fontData = nullptr;
	int texWidth = 0, texHeight = 0;
	io.Fonts->GetTexDataAsRGBA32(&fontData, &texWidth, &texHeight);
	VkDeviceSize uploadSize = texWidth * texHeight * 4 * sizeof(char);

	fImage.Create(texWidth, texHeight, 1, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	fontView = Element::VkFunctions::createImageView(logicalDevice, fImage.m_vkImage, 1, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

	// Staging buffers for font data upload
	Buffer stagingBuffer;

	stagingBuffer.Create(uploadSize, 0, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

	stagingBuffer.MapCopyMemory(fontData);

	fImage.transitionLayout(VK_FORMAT_R8G8B8A8_SRGB, 1, VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 0, VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_HOST_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT);

	fImage.CopyFromBuffer(stagingBuffer.m_buffer, texWidth, texHeight);

	fImage.transitionLayout(VK_FORMAT_R8G8B8A8_SRGB, 1, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.maxAnisotropy = 1.0f;
	samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

	if (vkCreateSampler(logicalDevice, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
		throw std::runtime_error("ImGui - failed to create texture sampler!");
	}

	fImage.m_descriptorInfo.imageView = fontView;
	fImage.m_descriptorInfo.sampler = sampler;
	fImage.m_descriptorInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;


	// Descriptor pool
	std::vector<VkDescriptorPoolSize> poolSizes = {
		Element::VkInitializers::descriptorPoolSizeCreateInfo(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)
	};

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = 2;

	if (vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("ImGui - failed to create descriptor pool!");
	}

	VkDescriptorSetLayoutBinding samplerLayoutBinding = Element::VkInitializers::descriptorSetLayoutBinding(VK_SHADER_STAGE_FRAGMENT_BIT, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0);

	std::vector<VkDescriptorSetLayoutBinding> bindings = { samplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo = Element::VkInitializers::descriptorSetLayoutCreateInfo(bindings.data(), static_cast<uint32_t>(bindings.size()));

	if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("ImGui - failed to create descriptor set layout!");
	}

	uint32_t imageCount = 3;

	std::vector<VkDescriptorSetLayout> layouts(1, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = layouts.data();
	if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, &descriptorSet) != VK_SUCCESS) {
		throw std::runtime_error("ImGui - failed to allocate descriptor sets!");
	}

	std::vector<VkWriteDescriptorSet> descriptorWrites;
	descriptorWrites.emplace_back(Element::VkInitializers::writeDesciptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		&fImage.m_descriptorInfo, 0));

	vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

	// Pipeline layout
	// Push constants for UI rendering parameters
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(PushConstBlock);

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = Element::VkInitializers::pipelineLayoutCreateInfo(&descriptorSetLayout, 1);
	pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
	pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

	if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("Imgui - failed to create pipeline layout!");
	}

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = Element::VkInitializers::pipelineInputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);

	VkPipelineRasterizationStateCreateInfo rasterizationState = Element::VkInitializers::pipelineRasterizerCreateInfo(VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_FALSE);

	// Enable blending
	VkPipelineColorBlendAttachmentState blendAttachmentState{};
	blendAttachmentState.blendEnable = VK_TRUE;
	blendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlendState = Element::VkInitializers::pipelineColourBlendCreateInfo(&blendAttachmentState, 1, VK_FALSE, VK_LOGIC_OP_COPY);

	VkPipelineDepthStencilStateCreateInfo depthStencilState = Element::VkInitializers::pipelineDepthStencilCreateInfo(VK_FALSE, VK_FALSE, VK_COMPARE_OP_LESS_OR_EQUAL, VK_FALSE, VK_FALSE);

	VkPipelineViewportStateCreateInfo viewportState = Element::VkInitializers::pipelineViewportCreateInfo(nullptr, 1, nullptr, 1);
	VkPipelineMultisampleStateCreateInfo multisampling = Element::VkInitializers::pipelineMultisamplerCreateInfo(VK_SAMPLE_COUNT_1_BIT, VK_FALSE);;
	std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	VkPipelineDynamicStateCreateInfo dynamicState = Element::VkInitializers::dynamicStateCreateInfo(dynamicStateEnables);

	std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages{};

	VkGraphicsPipelineCreateInfo pipelineCreateInfo = Element::VkInitializers::graphicsPipelineCreateInfo();

	pipelineCreateInfo.stageCount = shaderStages.size();
	pipelineCreateInfo.pStages = shaderStages.data();
	pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
	pipelineCreateInfo.pViewportState = &viewportState;
	pipelineCreateInfo.pRasterizationState = &rasterizationState;
	pipelineCreateInfo.pMultisampleState = &multisampling;
	pipelineCreateInfo.pDepthStencilState = &depthStencilState;
	pipelineCreateInfo.pColorBlendState = &colorBlendState;
	pipelineCreateInfo.pDynamicState = &dynamicState;
	pipelineCreateInfo.layout = pipelineLayout;
	pipelineCreateInfo.renderPass = renderPass;
	pipelineCreateInfo.subpass = 0;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;


	pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
	pipelineCreateInfo.pStages = shaderStages.data();

	// Vertex bindings an attributes based on ImGui vertex definition
	std::vector<VkVertexInputBindingDescription> vertexInputBindings = {
		Element::VkInitializers::vertexInputBindingDescription(VK_VERTEX_INPUT_RATE_VERTEX, 0, sizeof(ImDrawVert))
	};
	std::vector<VkVertexInputAttributeDescription> vertexInputAttributes = {

		Element::VkInitializers::vertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, pos)),
		Element::VkInitializers::vertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, uv)),
		Element::VkInitializers::vertexInputAttributeDescription(0, 2, VK_FORMAT_R8G8B8A8_UNORM,offsetof(ImDrawVert, col))
	};

	VkPipelineVertexInputStateCreateInfo vertexInputState = Element::VkInitializers::pipelineVertexInputCreateInfo(1);
	vertexInputState.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInputBindings.size());
	vertexInputState.pVertexBindingDescriptions = vertexInputBindings.data();
	vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributes.size());
	vertexInputState.pVertexAttributeDescriptions = vertexInputAttributes.data();

	pipelineCreateInfo.pVertexInputState = &vertexInputState;

	const auto& vert = Locator::getResource()->shader("ui", ShaderType::VERTEX);
    const auto& frag = Locator::getResource()->shader("ui", ShaderType::FRAGMENT);

	shaderStages[0] = Element::VkInitializers::pipelineShaderStageCreateInfo(vert->GetVkShaderModule(),
                                                                          vert->GetVkShaderStageFlag(), "main");
	shaderStages[1] = Element::VkInitializers::pipelineShaderStageCreateInfo(frag->GetVkShaderModule(),
                                                                          frag->GetVkShaderStageFlag(), "main");

	if (vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline) != VK_SUCCESS) {
		throw std::runtime_error("ImGui - failed to create graphics pipeline!");
	}
}

void Element::ImGUI::newFrame()
{
	if (!started || !prepared) return;

	ImGui::NewFrame();

	// Init imGui windows and elements

	ImVec4 clear_color = ImColor(114, 144, 154);
	static float f = 0.0f;
	//ImGui::TextUnformatted("mech");
	//ImGui::TextUnformatted("some");

	//ImGui::PlotLines("Frame Times", &aVar, 50, 0, "", 0, 10, ImVec2(0, 80));

	//ImGui::Text("Camera");

	//ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiSetCond_FirstUseEver));

	//for (const auto& window : debugWindows)
	//{
	//	if (window->getType() == DebugWindow::WindowType::DEMO)
	//	{
	//		ImGui::ShowDemoWindow();
	//		continue;
	//	}
	//	else if (window->getType() == DebugWindow::WindowType::ABOUT)
	//	{
	//		ImGui::ShowAboutWindow();
	//		continue;
	//	}

		//ImGui::Begin(window->getLabel() );
		//for (const auto& section : window->getSections())
		//{
		//	switch (section.type)
		//	{
		//	case DebugWindow::SectionType::TEXT:
		//	{
		//		ImGui::Text(section.label.c_str());
		//		break;
		//	}
		//	case DebugWindow::SectionType::INT:
		//	{
		//		ImGui::Text(section.label.c_str(), *(int*)section.data);
		//		break;
		//	}
		//	case DebugWindow::SectionType::FLOAT:
		//	{
		//		ImGui::Text(section.label.c_str(), *(float*)section.data);
		//		break;
		//	}
		//	case DebugWindow::SectionType::CHECK_BOX:
		//	{
		//		ImGui::Checkbox(section.label.c_str(), (bool*)section.data);
		//		break;
		//	}
		//	case DebugWindow::SectionType::GRAPH:
		//	{
		//		ImGui::PlotLines(section.label.c_str(), (const float*)section.data, section.amount, 0, section.textOverlay.c_str(), section.bounds.x, section.bounds.y, ImVec2(section.size.x, section.size.y));
		//		break;
		//	}

		//	}
		//}
		//ImGui::End();
	//}

	//ImGui::Begin("Example settings");
	//ImGui::Text("Light speed");
	//
	//ImGui::End();

	//ImGui::SetNextWindowPos(ImVec2(50, 20));
	//ImGui::ShowDemoWindow();

	// Render to generate draw buffers
	ImGui::Render(); 
}

void Element::ImGUI::start()
{
	prepared = true;

	ImGui::NewFrame();
}

void Element::ImGUI::end()
{
	ImGui::Render();
	prepared = false;
}

void Element::ImGUI::updateBuffers()
{
	if (!started || prepared) return;

	ImDrawData* imDrawData = ImGui::GetDrawData();

	// Note: Alignment is done inside buffer creation
	VkDeviceSize vertexBufferSize = imDrawData->TotalVtxCount * sizeof(ImDrawVert);
	VkDeviceSize indexBufferSize = imDrawData->TotalIdxCount * sizeof(ImDrawIdx);

	if ((vertexBufferSize == 0) || (indexBufferSize == 0)) {
		return;
	}

	// Update buffers only if vertex or index count has been changed compared to current buffer size

	// Vertex buffer
	if ((vertexBuffer.m_buffer == VK_NULL_HANDLE) || (vertexCount != imDrawData->TotalVtxCount)) {
		vertexBuffer.Unmap();
		vertexBuffer.Destroy();
		vertexBuffer.Create(vertexBufferSize, 0, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		vertexCount = imDrawData->TotalVtxCount;
		vertexBuffer.Map();
		//Debugger::Get().log("Vertex Buffers Mapped", DebuggerColours::GREEN);
	}

	if ((indexBuffer.m_buffer == VK_NULL_HANDLE) || (vertexCount != imDrawData->TotalVtxCount)) {
		indexBuffer.Unmap();
		indexBuffer.Destroy();
		indexBuffer.Create(vertexBufferSize, 0, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		indexCount = imDrawData->TotalVtxCount;
		indexBuffer.Map();
		//Debugger::Get().log("Index Buffers Mapped", DebuggerColours::BRIGHT_MAGENTA);
	}

	// Upload data
	ImDrawVert* vtxDst = (ImDrawVert*)vertexBuffer.mapped;
	ImDrawIdx* idxDst = (ImDrawIdx*)indexBuffer.mapped;

	for (int n = 0; n < imDrawData->CmdListsCount; n++) {
		const ImDrawList* cmd_list = imDrawData->CmdLists[n];
		memcpy(vtxDst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
		memcpy(idxDst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
		vtxDst += cmd_list->VtxBuffer.Size;
		idxDst += cmd_list->IdxBuffer.Size;
	}
	VkMappedMemoryRange mappedRange = {};
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = vertexBuffer.m_memory;
	mappedRange.offset = vertexBuffer.m_offset;
	mappedRange.size = VK_WHOLE_SIZE;
	vkFlushMappedMemoryRanges(Device::getVkDevice(), 1, &mappedRange);

	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = indexBuffer.m_memory;
	mappedRange.offset = indexBuffer.m_offset;
	mappedRange.size = VK_WHOLE_SIZE;
	vkFlushMappedMemoryRanges(Device::getVkDevice(), 1, &mappedRange);
}

void Element::ImGUI::drawFrame(VkCommandBuffer commandBuffer)
{
	if (!started) return;

	ImGuiIO& io = ImGui::GetIO();

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

	VkViewport viewport{};
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = io.DisplaySize.x;
	viewport.height = io.DisplaySize.y;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	// UI scale and translate via push constants
	pushConstBlock.scale = glm::vec2(2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y);
	pushConstBlock.translate = glm::vec2(-1.0f);
	vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstBlock), &pushConstBlock);

	// Render commands
	ImDrawData* imDrawData = ImGui::GetDrawData();
	int32_t vertexOffset = 0;
	int32_t indexOffset = 0;

	if (imDrawData->CmdListsCount > 0) {

		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer.m_buffer, offsets);
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer.m_buffer, 0, VK_INDEX_TYPE_UINT16);

		for (int32_t i = 0; i < imDrawData->CmdListsCount; i++)
		{
			const ImDrawList* cmd_list = imDrawData->CmdLists[i];
			for (int32_t j = 0; j < cmd_list->CmdBuffer.Size; j++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[j];
				VkRect2D scissorRect;
				scissorRect.offset.x = std::max((int32_t)(pcmd->ClipRect.x), 0);
				scissorRect.offset.y = std::max((int32_t)(pcmd->ClipRect.y), 0);
				scissorRect.extent.width = (uint32_t)(pcmd->ClipRect.z - pcmd->ClipRect.x);
				scissorRect.extent.height = (uint32_t)(pcmd->ClipRect.w - pcmd->ClipRect.y);
				vkCmdSetScissor(commandBuffer, 0, 1, &scissorRect);
				vkCmdDrawIndexed(commandBuffer, pcmd->ElemCount, 1, indexOffset, vertexOffset, 0);
				indexOffset += pcmd->ElemCount;
			}
			vertexOffset += cmd_list->VtxBuffer.Size;
		}
	}
}

void Element::ImGUI::deint()
{
	if (!started) return;

	ImGui::DestroyContext();
	// Release all Vulkan resources required for rendering imGui
	fImage.Destroy();
	vertexBuffer.Destroy();
	indexBuffer.Destroy();
	const auto& logicalDevice = Device::getVkDevice();
	vkDestroyImageView(logicalDevice, fontView, nullptr);
	vkFreeMemory(logicalDevice, fontMemory, nullptr);
	vkDestroySampler(logicalDevice, sampler, nullptr);
	vkDestroyPipelineCache(logicalDevice, pipelineCache, nullptr);
	vkDestroyPipeline(logicalDevice, pipeline, nullptr);
	vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
	vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
}


bool Element::ImGUI::isPrepared()
{
	return prepared;
}

void Element::ImGUI::updateMouse(float windowWidth, float windowHeight) {
    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize = ImVec2(windowWidth, windowHeight);

    auto mousePos = Inputs::get().getCursorPos();
    io.MousePos = ImVec2(mousePos.x, mousePos.y);
    //Debugger::Get().log("Mouse X", io.MousePos.x);
    //Debugger::Get().log("Mouse y", io.MousePos.y);
    io.MouseDown[0] = Inputs::get().buttonDown(Element::MOUSE_BUTTON::LEFT) || Inputs::get().buttonHeld(Element::MOUSE_BUTTON::LEFT);
    io.MouseDown[1] = Inputs::get().buttonDown(Element::MOUSE_BUTTON::RIGHT);
}
