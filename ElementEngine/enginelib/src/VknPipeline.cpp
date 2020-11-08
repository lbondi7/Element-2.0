#include "VknPipeline.h"

#include "VkFunctions.h"
#include "Device.h"
#include "VkInitializers.h"
#include <element/GameSettings.h>

#include <stdexcept>

Element::VknPipeline::VknPipeline(SwapChain* swapChain, RenderPass* renderPass, const std::string& name, const PipelineData& pipelineInfo) : m_swapChain(swapChain), m_renderPass(renderPass), name(name), m_pipelineData(pipelineInfo)
{
    for (const auto& bindInfo : m_pipelineData.descriptorInfo)
        bindingsData.emplace_back(Element::VkInitializers::descriptorSetLayoutBinding(Shader::GetVkShaderStageFlag(bindInfo.descriptorShader), Element::VkFunctions::getDescriptorType(bindInfo.descriptorType), bindInfo.binding));

    for (const auto& shader : m_pipelineData.shaders)
    {
        if (!shader)
            throw std::runtime_error("shader is null");

        shaderStages.emplace_back(Element::VkInitializers::pipelineShaderStageCreateInfo(shader->GetVkShaderModule(), shader->GetVkShaderStageFlag()));
    }

    init();
}

Element::VknPipeline::~VknPipeline()
{
    for (auto& shader : m_pipelineData.shaders)
    {
        shader = nullptr;
    }
    for (auto& shader : m_pipelineData.shaders)
    {
        shader = nullptr;
    }
    m_pipelineData.shaders.clear();
    shaderStages.clear();
    bindingsData.clear();
}

void Element::VknPipeline::init() {

    createDescriptorSetLayout();
    createPipelineLayout();
    createVknPipeline();
    createDescriptorPool();
}

void Element::VknPipeline::destroy()
{
    flush();

    auto logicalDevice = Device::getVkDevice();
    vkDestroyPipelineLayout(logicalDevice, m_pipelineLayout, nullptr);
    vkDestroyDescriptorSetLayout(logicalDevice, m_descriptorSetLayout, nullptr);
}

void Element::VknPipeline::reInitVknPipeline(SwapChain* swapChain, RenderPass* renderPass) {

    flushed = false;
    m_swapChain = swapChain;
    m_renderPass = renderPass;
    createVknPipeline();
    createDescriptorPool();
}

void Element::VknPipeline::flush() {

    if (flushed)
        return;

    flushed = true;
    auto logicalDevice = Device::getVkDevice();
    vkDestroyDescriptorPool(logicalDevice, m_descriptorPool, nullptr);
    vkDestroyPipeline(logicalDevice, m_vkPipeline, nullptr);
    m_swapChain = nullptr;
    m_renderPass = nullptr;
}

void Element::VknPipeline::bind(VkCommandBuffer vkCommandBuffer)
{
    bound = true;
    vkCmdBindPipeline(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_vkPipeline);
}


void Element::VknPipeline::createPipelineLayout() {

    auto logicalDevice = Device::getVkDevice();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = Element::VkInitializers::pipelineLayoutCreateInfo(&m_descriptorSetLayout);

    if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void Element::VknPipeline::createDescriptorSetLayout() {

    VkDescriptorSetLayoutCreateInfo layoutInfo = Element::VkInitializers::descriptorSetLayoutCreateInfo(bindingsData.data(), static_cast<uint32_t>(bindingsData.size()));

    if (vkCreateDescriptorSetLayout(Device::getVkDevice(), &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void Element::VknPipeline::createDescriptorPool() {
    uint32_t requireSetInfo = static_cast<uint32_t>(m_swapChain->getImageCount() * 10000);
    std::vector<VkDescriptorPoolSize> poolSizes;
    for (const auto& binding : bindingsData)
    {
        poolSizes.emplace_back(Element::VkInitializers::descriptorPoolSizeCreateInfo(binding.descriptorType,  requireSetInfo));
    }

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = requireSetInfo;

    if (vkCreateDescriptorPool(Device::getVkDevice(), &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

Element::PipelineData& Element::VknPipeline::GetPipelineData()
{
    return m_pipelineData;
}

bool Element::VknPipeline::isBound()
{
    return bound;
}

void Element::VknPipeline::setBound(bool _bound)
{
    bound = _bound;
}

const std::string& Element::VknPipeline::getName()
{
    return name;
}

void Element::VknPipeline::createVknPipeline() {

    auto logicalDevice = Device::getVkDevice();
    const auto& physicalDevice = Device::GetPhysicalDevice()->GetSelectedDevice();

    bool depthEnabled = GameSettings::Instance().depthEnabled;

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = Element::VkInitializers::pipelineVertexInputCreateInfo(1);

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = Element::VkInitializers::pipelineInputAssemblyCreateInfo();
    VkPipelineViewportStateCreateInfo viewportState = Element::VkInitializers::pipelineViewportCreateInfo(nullptr, 1, nullptr, 1);
    VkPipelineRasterizationStateCreateInfo rasterizer = Element::VkInitializers::pipelineRasterizerCreateInfo(VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_POLYGON_MODE_FILL, depthEnabled ? VK_FALSE : VK_TRUE);
    VkPipelineMultisampleStateCreateInfo multisampling = Element::VkInitializers::pipelineMultisamplerCreateInfo(physicalDevice.msaaSamples, VK_FALSE);  
    VkPipelineDepthStencilStateCreateInfo depthStencil = Element::VkInitializers::pipelineDepthStencilCreateInfo(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS, VK_FALSE, VK_FALSE);
    VkPipelineColorBlendAttachmentState colorBlendAttachment = Element::VkInitializers::pipelineColourBlendAttachment(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
    VkPipelineColorBlendStateCreateInfo colorBlending = Element::VkInitializers::pipelineColourBlendCreateInfo(&colorBlendAttachment, 1, VK_FALSE, VK_LOGIC_OP_COPY);
    std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dynamicState = Element::VkInitializers::dynamicStateCreateInfo(dynamicStateEnables);

    VkGraphicsPipelineCreateInfo pipelineInfo = Element::VkInitializers::graphicsPipelineCreateInfo();
    pipelineInfo.stageCount = shaderStages.size();
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState =  &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = m_pipelineLayout;
    pipelineInfo.renderPass = m_renderPass->GetVkRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_vkPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
}

VkDescriptorSetLayout Element::VknPipeline::GetVkDescriptorSetLayout()
{
    return m_descriptorSetLayout;
}

VkPipeline Element::VknPipeline::GetVkPipeline()
{
    return m_vkPipeline;
}

VkPipelineLayout Element::VknPipeline::GetVkPipelineLayout()
{
    return m_pipelineLayout;
}

VkDescriptorPool Element::VknPipeline::GetVkDescriptorPool()
{
    return m_descriptorPool;
}

//const VkDescriptorSetLayout Element::VknPipeline::GetVkDescriptorSetLayout() const
//{
//    return m_descriptorSetLayout;
//}
//
//const VkPipeline Element::VknPipeline::GetVkPipeline() const
//{
//    return m_vkPipeline;
//}
//
//const VkPipelineLayout Element::VknPipeline::GetVkPipelineLayout() const
//{
//    return m_pipelineLayout;
//}
//
//const VkDescriptorPool Element::VknPipeline::GetVkDescriptorPool() const
//{
//    return m_descriptorPool;
//}
