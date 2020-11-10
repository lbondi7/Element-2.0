#include "VkInitializers.h"


VkAttachmentDescription Element::VkInitializers::attachmentDescription(VkFormat format, VkSampleCountFlagBits samples, VkImageLayout finalLayout, VkImageLayout initialLayout, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp loadStencilOp, VkAttachmentStoreOp storeStencilOp)
{
    VkAttachmentDescription attachment{};
    attachment.format = format;
    attachment.samples = samples;
    attachment.initialLayout = initialLayout;
    attachment.finalLayout = finalLayout;
    attachment.loadOp = loadOp;
    attachment.storeOp = storeOp;
    attachment.stencilLoadOp = loadStencilOp;
    attachment.stencilStoreOp = storeStencilOp;
    return attachment;
}

VkAttachmentReference Element::VkInitializers::attachmentReference(uint32_t attachment, VkImageLayout imageLayout)
{
    VkAttachmentReference attachmentRef{};
    attachmentRef.attachment = attachment;
    attachmentRef.layout = imageLayout;
    return attachmentRef;
}

VkShaderModuleCreateInfo Element::VkInitializers::shaderModuleCreateInfo(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    
    return createInfo;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
///  Pipeline Create Info
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


VkPipelineLayoutCreateInfo  Element::VkInitializers::pipelineLayoutCreateInfo(const VkDescriptorSetLayout* descriptorSetLayouts, uint32_t descriptorSetLayoutCount) {
    VkPipelineLayoutCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.setLayoutCount = descriptorSetLayoutCount;
    createInfo.pSetLayouts = descriptorSetLayouts;

    return createInfo;
}

VkGraphicsPipelineCreateInfo Element::VkInitializers::graphicsPipelineCreateInfo() {

    VkGraphicsPipelineCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

    return createInfo;
}

VkPipelineShaderStageCreateInfo Element::VkInitializers::pipelineShaderStageCreateInfo(VkShaderModule shaderModule, VkShaderStageFlagBits shaderStage, const char* entryFunctionName) {

   VkPipelineShaderStageCreateInfo createInfo{};
   createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
   createInfo.stage = shaderStage;
   createInfo.module = shaderModule;
   createInfo.pName = entryFunctionName;

    return createInfo;
}

VkPipelineMultisampleStateCreateInfo Element::VkInitializers::pipelineMultisamplerCreateInfo(VkSampleCountFlagBits sampleCount, VkBool32 sampleShading) {

   VkPipelineMultisampleStateCreateInfo createInfo{};
   createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
   createInfo.sampleShadingEnable = sampleShading;
   createInfo.rasterizationSamples = sampleCount;

    return createInfo;
}

VkPipelineDepthStencilStateCreateInfo Element::VkInitializers::pipelineDepthStencilCreateInfo(VkBool32 depthTest, VkBool32 depthWrite, VkCompareOp compareOp, VkBool32 depthBoundsTest, VkBool32 stencilTest) {

    VkPipelineDepthStencilStateCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    createInfo.depthTestEnable = depthTest;
    createInfo.depthWriteEnable = depthWrite;
    createInfo.depthCompareOp = compareOp;
    createInfo.depthBoundsTestEnable = depthBoundsTest;
    createInfo.stencilTestEnable = stencilTest;
    
    return createInfo;
}

VkPipelineColorBlendAttachmentState Element::VkInitializers::pipelineColourBlendAttachment(VkColorComponentFlags flags, VkBool32 blend) {

    VkPipelineColorBlendAttachmentState colourBlendAttachment{};
    colourBlendAttachment.colorWriteMask = flags;
    colourBlendAttachment.blendEnable = blend;
    
    return colourBlendAttachment;
}

VkPipelineColorBlendStateCreateInfo  Element::VkInitializers::pipelineColourBlendCreateInfo(const VkPipelineColorBlendAttachmentState* attachments, uint32_t attachmentCount, VkBool32 logicOpEnabled, VkLogicOp logicOp) {

    VkPipelineColorBlendStateCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    createInfo.logicOpEnable = logicOpEnabled;
    createInfo.logicOp = logicOp;
    createInfo.attachmentCount = attachmentCount;
    createInfo.pAttachments = attachments;
    for (auto& b : createInfo.blendConstants)
        b = 0.0f;
    return createInfo;
}

VkPipelineInputAssemblyStateCreateInfo Element::VkInitializers::pipelineInputAssemblyCreateInfo(VkPrimitiveTopology topology, VkBool32 primitiveRestart) {

    VkPipelineInputAssemblyStateCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    createInfo.topology = topology;
    createInfo.primitiveRestartEnable = primitiveRestart;

    return createInfo;
}

VkPipelineVertexInputStateCreateInfo Element::VkInitializers::pipelineVertexInputCreateInfo(uint32_t bindingCount) {

    VkPipelineVertexInputStateCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    createInfo.vertexBindingDescriptionCount = bindingCount;

    return createInfo;
}

VkPipelineRasterizationStateCreateInfo Element::VkInitializers::pipelineRasterizerCreateInfo(VkCullModeFlags cullmode, VkFrontFace frontFace, VkPolygonMode polygonMode, VkBool32 depthClamp, VkBool32 depthBias, float lineWidth, VkBool32 discard) {

    VkPipelineRasterizationStateCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    createInfo.depthClampEnable = depthClamp;
    createInfo.rasterizerDiscardEnable = discard;
    createInfo.polygonMode =  polygonMode;
    createInfo.lineWidth = lineWidth;
    createInfo.cullMode = cullmode;
    createInfo.frontFace = frontFace;
    createInfo.depthBiasEnable = depthBias;

    return createInfo;
}

VkPipelineViewportStateCreateInfo Element::VkInitializers::pipelineViewportCreateInfo(const VkViewport* viewports, uint32_t viewportCount, const VkRect2D* scissors, uint32_t scissorCount) {

    VkPipelineViewportStateCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    createInfo.viewportCount = viewportCount;
    createInfo.pViewports = viewports;
    createInfo.scissorCount = scissorCount;
    createInfo.pScissors = scissors;

    return createInfo;
}

VkVertexInputBindingDescription Element::VkInitializers::vertexInputBindingDescription(VkVertexInputRate vertexInputRate, uint32_t binding, uint32_t stride)
{
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = binding;
    bindingDescription.stride = stride;
    bindingDescription.inputRate = vertexInputRate;

    return bindingDescription;
}

VkVertexInputAttributeDescription Element::VkInitializers::vertexInputAttributeDescription(uint32_t binding, uint32_t location, VkFormat format, uint32_t offset)
{
    VkVertexInputAttributeDescription attributeDescriptions{};
    attributeDescriptions.binding = binding;
    attributeDescriptions.location = location;
    attributeDescriptions.format = format;
    attributeDescriptions.offset = offset;

    return attributeDescriptions;
}

VkPipelineDynamicStateCreateInfo Element::VkInitializers::dynamicStateCreateInfo(const std::vector<VkDynamicState>& pDynamicStates, VkPipelineDynamicStateCreateFlags flags)
{
    VkPipelineDynamicStateCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    createInfo.pDynamicStates = pDynamicStates.data();
    createInfo.dynamicStateCount = static_cast<uint32_t>(pDynamicStates.size());
    createInfo.flags = flags;
    return createInfo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
///  Desciptor Create Info
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VkDescriptorSetLayoutCreateInfo Element::VkInitializers::descriptorSetLayoutCreateInfo(const VkDescriptorSetLayoutBinding* bindings, uint32_t count) {

    VkDescriptorSetLayoutCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createInfo.bindingCount = count;
    createInfo.pBindings = bindings;

    return createInfo;
}

VkDescriptorSetLayoutBinding Element::VkInitializers::descriptorSetLayoutBinding(VkShaderStageFlags shaderStage, VkDescriptorType descriptorType, uint32_t binding, uint32_t count, const VkSampler* immutableSampler) {
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = binding;
    layoutBinding.descriptorCount = count;
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.pImmutableSamplers = immutableSampler;
    layoutBinding.stageFlags = shaderStage;

    return layoutBinding;
}

VkWriteDescriptorSet Element::VkInitializers::writeDesciptorSet(VkDescriptorSet descriptorSet, VkDescriptorType descriptorType, const VkDescriptorBufferInfo* bufferInfo, uint32_t binding, uint32_t count)
{
    VkWriteDescriptorSet descriptorWrites{};

    descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites.dstSet = descriptorSet;
    descriptorWrites.dstBinding = binding;
    descriptorWrites.dstArrayElement = 0;
    descriptorWrites.descriptorType = descriptorType;
    descriptorWrites.descriptorCount = count;
    descriptorWrites.pBufferInfo = bufferInfo;


    return descriptorWrites;
}

VkWriteDescriptorSet Element::VkInitializers::writeDesciptorSet(VkDescriptorSet descriptorSet, VkDescriptorType descriptorType, const VkDescriptorImageInfo* imageInfo, uint32_t binding, uint32_t count)
{
    VkWriteDescriptorSet descriptorWrites{};

    descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites.dstSet = descriptorSet;
    descriptorWrites.dstBinding = binding;
    descriptorWrites.dstArrayElement = 0;
    descriptorWrites.descriptorType = descriptorType;
    descriptorWrites.descriptorCount = count;
    descriptorWrites.pImageInfo = imageInfo;


    return descriptorWrites;
}

VkDescriptorPoolSize Element::VkInitializers::descriptorPoolSizeCreateInfo(VkDescriptorType descriptorType, uint32_t descriptorCount)
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = descriptorType;
    poolSize.descriptorCount = descriptorCount;
    return poolSize;
}

VkDescriptorPoolCreateInfo
Element::VkInitializers::descriptorPoolCreateInfo(const VkDescriptorPoolSize *descriptorPoolSizeData,
                                                  uint32_t poolSizeCount, uint32_t maxSets) {
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.pPoolSizes = descriptorPoolSizeData;
    poolInfo.poolSizeCount = poolSizeCount;
    poolInfo.maxSets = maxSets;
    return poolInfo;
}
