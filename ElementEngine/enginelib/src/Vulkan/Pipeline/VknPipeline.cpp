#include "VknPipeline.h"

#include <Vulkan/VkFunctions.h>
#include <Vulkan/Device/Device.h>
#include <Vulkan/VkInitializers.h>
#include <Resource/Resources.h>
#include <Misc/Locator.h>

#include <element/GameSettings.h>
#include <nlohmann/json.hpp>

#include <stdexcept>
#include <fstream>

using json = nlohmann::json;

Element::VknPipeline::VknPipeline(VknSwapChain* swapChain, VknRenderPass* renderPass, const std::string& name, const PipelineData& pipelineInfo) :
m_swapChain(swapChain), m_renderPass(renderPass), name(name), m_pipelineData(pipelineInfo)
{
    DescriptorInfo descriptorInfo = {BindObjectType::STATIC_UNIFORM_BUFFER, ShaderType::VERTEX,
                                     m_pipelineData.descriptorInfo[0].shader, 0, 0};
    m_pipelineData.descriptorInfo.emplace(m_pipelineData.descriptorInfo.cbegin(), descriptorInfo);
    if(m_pipelineData.lightingEnabled) {
        descriptorInfo = {BindObjectType::STATIC_STORAGE_BUFFER, ShaderType::FRAGMENT,
                          m_pipelineData.descriptorInfo[0].shader, 1, 0};
        m_pipelineData.descriptorInfo.emplace(m_pipelineData.descriptorInfo.cbegin(), descriptorInfo);
        descriptorInfo = {BindObjectType::STATIC_UNIFORM_BUFFER, ShaderType::FRAGMENT,
                          m_pipelineData.descriptorInfo[0].shader, 1, 1};
        m_pipelineData.descriptorInfo.emplace(m_pipelineData.descriptorInfo.cbegin(), descriptorInfo);
    }

    int numberOfSets = 0;
    for (const auto& shaderInfo : m_pipelineData.descriptorInfo) {
//        bindingsData.emplace_back(
//                VkInitializers::descriptorSetLayoutBinding(
//                        Shader::GetVkShaderStageFlag(shaderInfo.shaderType),
//                        VkFunctions::getDescriptorType(shaderInfo.bindObjectType),
//                        shaderInfo.binding));

        if(shaderInfo.set > numberOfSets)
            numberOfSets = shaderInfo.set;

        auto shader = Locator::getResource()->shader(shaderInfo.shader, shaderInfo.shaderType);
        bool loaded = false;
        for (const auto& shaderStage : shaderStages) {
                if(shader->GetVkShaderStageFlag() == shaderStage.stage)
                    loaded = true;
        }
        if(!loaded)
            shaderStages.emplace_back(VkInitializers::pipelineShaderStageCreateInfo(
                            shader->GetVkShaderModule(), shader->GetVkShaderStageFlag()));
    }

    m_descriptorSetLayouts.resize(numberOfSets + 1);

    init();
}

//Element::VknPipeline::VknPipeline(VknSwapChain* swapChain, VknRenderPass* renderPass, const std::string& name, const PipelineData& pipelineInfo) :
//        m_swapChain(swapChain), m_renderPass(renderPass), name(name), m_pipelineData(pipelineInfo)
//{
//    std::ifstream file("resources/pipelines/" + name + ".json");
//    json j;
//    file >> j;
//    auto shaderCount = j["shaderCount"];
//    DescriptorInfo descriptorInfo;
//    int numberOfSets = 0;
//    for(auto shad : j["shaders"])
//    {
//        auto shader = Locator::getResource()->shader(shad["name"], static_cast<ShaderType>(shad["type"]));
//        shaderStages.emplace_back(VkInitializers::pipelineShaderStageCreateInfo(
//                shader->GetVkShaderModule(), shader->GetVkShaderStageFlag()));
//
//        for(auto buffer : shad["buffers"])
//        {
//            descriptorInfo = {static_cast<BindObjectType>(buffer["type"]), static_cast<ShaderType>(shad["type"]),
//                             shad["name"], buffer["set"], buffer["binding"]};
//
//            if(buffer["set"] > numberOfSets)
//                numberOfSets = buffer["set"];
//            m_pipelineData.descriptorInfo.emplace_back(descriptorInfo);
//        }
//
//    }
//    m_descriptorSetLayouts.resize(numberOfSets + 1);
//
////    DescriptorInfo descriptorInfo = {BindObjectType::STATIC_UNIFORM_BUFFER, ShaderType::VERTEX,
////                                     m_pipelineData.descriptorInfo[0].shader, 0, 0};
////    m_pipelineData.descriptorInfo.emplace(m_pipelineData.descriptorInfo.cbegin(), descriptorInfo);
////    if(m_pipelineData.lightingEnabled) {
////        descriptorInfo = {BindObjectType::STATIC_STORAGE_BUFFER, ShaderType::FRAGMENT,
////                          m_pipelineData.descriptorInfo[0].shader, 1, 0};
////        m_pipelineData.descriptorInfo.emplace(m_pipelineData.descriptorInfo.cbegin(), descriptorInfo);
////        descriptorInfo = {BindObjectType::STATIC_UNIFORM_BUFFER, ShaderType::FRAGMENT,
////                          m_pipelineData.descriptorInfo[0].shader, 1, 1};
////        m_pipelineData.descriptorInfo.emplace(m_pipelineData.descriptorInfo.cbegin(), descriptorInfo);
////    }
////
////    int numberOfSets = 0;
////    for (const auto& shaderInfo : m_pipelineData.descriptorInfo) {
//////        bindingsData.emplace_back(
//////                VkInitializers::descriptorSetLayoutBinding(
//////                        Shader::GetVkShaderStageFlag(shaderInfo.shaderType),
//////                        VkFunctions::getDescriptorType(shaderInfo.bindObjectType),
//////                        shaderInfo.binding));
////
////        if(shaderInfo.set > numberOfSets)
////            numberOfSets = shaderInfo.set;
////
////        auto shader = Locator::getResource()->shader(shaderInfo.shader, shaderInfo.shaderType);
////        bool loaded = false;
////        for (const auto& shaderStage : shaderStages) {
////            if(shader->GetVkShaderStageFlag() == shaderStage.stage)
////                loaded = true;
////        }
////        if(!loaded)
////            shaderStages.emplace_back(VkInitializers::pipelineShaderStageCreateInfo(
////                    shader->GetVkShaderModule(), shader->GetVkShaderStageFlag()));
////    }
////
////    m_descriptorSetLayouts.resize(numberOfSets + 1);
//
//    init();
//}

Element::VknPipeline::~VknPipeline()
{
    shaderStages.clear();
    bindingsData.clear();
    m_swapChain = nullptr;
    m_renderPass = nullptr;
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
    for (auto& pool : descriptorPools) {
        pool->destroy();
    }
    descriptorPools.clear();

    auto logicalDevice = Device::getVkDevice();
    vkDestroyPipelineLayout(logicalDevice, m_pipelineLayout, nullptr);
    for (auto& layout : m_descriptorSetLayouts)
        vkDestroyDescriptorSetLayout(logicalDevice, layout, nullptr);
}

void Element::VknPipeline::reInitVknPipeline() {

    flushed = false;
    createVknPipeline();
}

void Element::VknPipeline::flush() {

    if (flushed)
        return;

    flushed = true;
    auto logicalDevice = Device::getVkDevice();
    for (auto& pool : descriptorPools) {
        pool->flush();
    }
    vkDestroyPipeline(logicalDevice, m_vkPipeline, nullptr);
}

void Element::VknPipeline::bind(VkCommandBuffer vkCommandBuffer)
{
    bound = true;
    vkCmdBindPipeline(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_vkPipeline);
}


void Element::VknPipeline::createPipelineLayout() {

    auto logicalDevice = Device::getVkDevice();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = VkInitializers::pipelineLayoutCreateInfo(
            m_descriptorSetLayouts.data(), m_descriptorSetLayouts.size());

    if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void Element::VknPipeline::createDescriptorSetLayout() {

    for (int i = 0; i < m_descriptorSetLayouts.size(); ++i) {
        std::vector<VkDescriptorSetLayoutBinding> bind;
        for (const auto& shaderInfo : m_pipelineData.descriptorInfo) {

            if (shaderInfo.set == i) {
                bind.emplace_back(VkInitializers::descriptorSetLayoutBinding(
                        Shader::GetVkShaderStageFlag(shaderInfo.shaderType),
                        VkFunctions::getDescriptorType(shaderInfo.bindObjectType),
                        shaderInfo.binding));
            }
        }

        VkDescriptorSetLayoutCreateInfo layoutInfo =
                VkInitializers::descriptorSetLayoutCreateInfo(bind.data(), static_cast<uint32_t>(bind.size()));

        if (vkCreateDescriptorSetLayout(Device::getVkDevice(), &layoutInfo,
                                        nullptr,&m_descriptorSetLayouts[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }
}

void Element::VknPipeline::createDescriptorPool() {

    auto& pool = descriptorPools.emplace_back();
    pool = std::make_unique<VknDescriptorPool>();
    pool->init(m_pipelineData, m_swapChain->getImageCount());

}

void Element::VknPipeline::reCreateDescriptorPool() {

    //Locator::getVknResource()->allocateDescriptorPool(m_pipelineData, m_swapChain->getImageCount());

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

    const auto& logicalDevice = Device::getVkDevice();
    const auto& physicalDevice = Device::GetPhysicalDevice()->GetSelectedDevice();

    bool depthEnabled = m_pipelineData.depthEnabled;

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = VkInitializers::pipelineVertexInputCreateInfo(
            1);

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = VkInitializers::pipelineInputAssemblyCreateInfo();
    VkPipelineViewportStateCreateInfo viewportState = VkInitializers::pipelineViewportCreateInfo(
            nullptr, 1, nullptr,1);
    VkPipelineRasterizationStateCreateInfo rasterizer = VkInitializers::pipelineRasterizerCreateInfo(
            VkFunctions::getCullModeFlags(m_pipelineData.cullMode),
            VK_FRONT_FACE_COUNTER_CLOCKWISE,
            VkFunctions::getPolygonMode(m_pipelineData.polygonMode),
            depthEnabled ? VK_FALSE : VK_TRUE);
    VkPipelineMultisampleStateCreateInfo multisampling = VkInitializers::pipelineMultisamplerCreateInfo(
            physicalDevice.msaaSamples, VK_FALSE);
    VkPipelineDepthStencilStateCreateInfo depthStencil =VkInitializers::pipelineDepthStencilCreateInfo(
            VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS, VK_FALSE, VK_FALSE);
    VkPipelineColorBlendAttachmentState colorBlendAttachment = VkInitializers::pipelineColourBlendAttachment(
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
    VkPipelineColorBlendStateCreateInfo colorBlending = VkInitializers::pipelineColourBlendCreateInfo
            (&colorBlendAttachment, 1, VK_FALSE, VK_LOGIC_OP_COPY);
    std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

    VkPipelineDynamicStateCreateInfo dynamicState = VkInitializers::dynamicStateCreateInfo(dynamicStateEnables);

    VkGraphicsPipelineCreateInfo pipelineInfo = VkInitializers::graphicsPipelineCreateInfo();
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

VkPipeline Element::VknPipeline::GetVkPipeline()
{
    return m_vkPipeline;
}

VkPipelineLayout Element::VknPipeline::GetVkPipelineLayout()
{
    return m_pipelineLayout;
}

//VkDescriptorPool Element::VknPipeline::GetVkDescriptorPool()
//{
//    return m_descriptorPool;
//}

VkDescriptorPool Element::VknPipeline::allocateDescriptorPool(uint32_t descriptorCount) {

    for (auto& pool : descriptorPools) {
        if (!pool->isFull(descriptorCount))
            return pool->getVkDescriptorPool();
    }

    auto& pool = descriptorPools.emplace_back();
    pool = std::make_unique<VknDescriptorPool>();
    pool->init(m_pipelineData, m_swapChain->getImageCount());
    return pool->getVkDescriptorPool();
}

VkDescriptorSetLayout Element::VknPipeline::getVkDescriptorSetLayout(int layout) {
    return m_descriptorSetLayouts[layout];
}

uint32_t Element::VknPipeline::getDescriptorLayoutsCount() {
    return static_cast<uint32_t>(m_descriptorSetLayouts.size());
}

