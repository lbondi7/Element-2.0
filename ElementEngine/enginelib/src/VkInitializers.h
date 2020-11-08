#pragma once

#include "vulkan/vulkan.h"

#include <vector>

namespace Element {

	namespace VkInitializers
	{
		///RenderPass Create Info///
		//VkAttachmentDescription attachtmnetDescription();
		VkAttachmentDescription attachmentDescription(VkFormat format, VkSampleCountFlagBits samples, VkImageLayout finalLayout, VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED, VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE, VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE, VkAttachmentLoadOp loadStencilOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE, VkAttachmentStoreOp storeStencilOp = VK_ATTACHMENT_STORE_OP_DONT_CARE);
		VkAttachmentReference attachmentReference(uint32_t attachment, VkImageLayout imageLayout);

		///Shader Create Info///
		VkShaderModuleCreateInfo shaderModuleCreateInfo(const std::vector<char>& code);

		///Pipeline Create Info///
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo(const VkDescriptorSetLayout* descriptorSetLayouts, uint32_t descriptorSetLayoutCount = 1);
		VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo();
		VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo(VkShaderModule shaderModule, VkShaderStageFlagBits shaderStage, const char* entryFunctionName = "main");
		VkPipelineMultisampleStateCreateInfo pipelineMultisamplerCreateInfo(VkSampleCountFlagBits sampleCount, VkBool32 sampleShading);
		VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilCreateInfo(VkBool32 depthTest, VkBool32 depthWrite, VkCompareOp compareOp, VkBool32 depthBoundsTest, VkBool32 stencilTest);
		VkPipelineColorBlendAttachmentState pipelineColourBlendAttachment(VkColorComponentFlags flags, VkBool32 blend = VK_FALSE);
		VkPipelineColorBlendStateCreateInfo pipelineColourBlendCreateInfo(const VkPipelineColorBlendAttachmentState* attachments, uint32_t attachmentCount, VkBool32 logicOpEnabled, VkLogicOp logicOp);
		VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyCreateInfo(VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VkBool32 primitiveRestart = VK_FALSE);
		VkPipelineVertexInputStateCreateInfo pipelineVertexInputCreateInfo(uint32_t bindingCount = 1);
		VkPipelineRasterizationStateCreateInfo pipelineRasterizerCreateInfo(VkCullModeFlags cullmode, VkFrontFace frontFace, VkPolygonMode polygonMode, VkBool32 depthClamp, VkBool32 depthBias = VK_FALSE, float lineWidth = 1.0f, VkBool32 discard = VK_FALSE);
		VkPipelineViewportStateCreateInfo pipelineViewportCreateInfo(const VkViewport* viewports, uint32_t viewportCount, const VkRect2D* scissors, uint32_t scissorCount);
		VkVertexInputBindingDescription vertexInputBindingDescription(VkVertexInputRate vertexInputRate, uint32_t binding, uint32_t stride);
		VkVertexInputAttributeDescription vertexInputAttributeDescription(uint32_t binding, uint32_t location , VkFormat format, uint32_t offset);
		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo(const std::vector<VkDynamicState>& pDynamicStates, VkPipelineDynamicStateCreateFlags flags = VK_CULL_MODE_NONE);

		///Desciptor Create Info///
		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo(const VkDescriptorSetLayoutBinding* bindings, uint32_t count);
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding(VkShaderStageFlags shaderStage, VkDescriptorType descriptorType, uint32_t binding, uint32_t count = 1, const VkSampler* immutableSampler = nullptr);
		VkWriteDescriptorSet writeDesciptorSet(VkDescriptorSet descriptorSet, VkDescriptorType descriptorType, const VkDescriptorBufferInfo* bufferInfo, uint32_t binding, uint32_t count = 1);
		VkWriteDescriptorSet writeDesciptorSet(VkDescriptorSet descriptorSet, VkDescriptorType descriptorType, const VkDescriptorImageInfo* imageInfo, uint32_t binding, uint32_t count = 1);
		VkDescriptorPoolSize descriptorPoolSizeCreateInfo(VkDescriptorType descriptorType, uint32_t descriptorCount);
	};
}