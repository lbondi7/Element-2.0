#include "VknRenderPass.h"

#include <Vulkan/VkFunctions.h>
#include <Vulkan/Device/Device.h>
#include <Vulkan/VkInitializers.h>

#include <element/GameSettings.h>

#include <stdexcept>

Element::RenderPass::RenderPass(VkFormat imageFormat)
{
   // createVkRenderPass(imageFormat);
}

Element::RenderPass::RenderPass(SwapChain* swapChain)
{
    init(swapChain);
}

Element::RenderPass::~RenderPass()
{
    Destroy();
}

void Element::RenderPass::init(SwapChain* swapChain)
{
    if(created)
        return;

    m_swapChain = swapChain;

    createVkRenderPass();
    createFrameBuffers();
    created = true;
}

void Element::RenderPass::createVkRenderPass() {

    const auto& physicalDevice = Device::GetPhysicalDevice()->GetSelectedDevice();

    bool isMSAAx1 = physicalDevice.msaaSamples & VK_SAMPLE_COUNT_1_BIT;
    bool depthEnabled = GameSettings::get().depthEnabled;

    VkAttachmentDescription colorAttachment = Element::VkInitializers::attachmentDescription(
            m_swapChain->ImageFormat(),
            physicalDevice.msaaSamples, isMSAAx1 ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_UNDEFINED, 
        VK_ATTACHMENT_LOAD_OP_CLEAR, 
        VK_ATTACHMENT_STORE_OP_STORE, 
        VK_ATTACHMENT_LOAD_OP_DONT_CARE, 
        VK_ATTACHMENT_STORE_OP_DONT_CARE);
    VkAttachmentDescription depthAttachment = Element::VkInitializers::attachmentDescription(
            physicalDevice.requiredFormats.at("depth"),
            physicalDevice.msaaSamples, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_UNDEFINED, 
        VK_ATTACHMENT_LOAD_OP_CLEAR, 
        VK_ATTACHMENT_STORE_OP_DONT_CARE, 
        VK_ATTACHMENT_LOAD_OP_DONT_CARE, 
        VK_ATTACHMENT_STORE_OP_DONT_CARE);
    VkAttachmentDescription colorAttachmentResolve = Element::VkInitializers::attachmentDescription(
            m_swapChain->ImageFormat(),
            VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        VK_IMAGE_LAYOUT_UNDEFINED, 
        VK_ATTACHMENT_LOAD_OP_DONT_CARE, 
        VK_ATTACHMENT_STORE_OP_STORE, 
        VK_ATTACHMENT_LOAD_OP_DONT_CARE, 
        VK_ATTACHMENT_STORE_OP_DONT_CARE);

    VkAttachmentReference colorAttachmentRef =Element::VkInitializers::attachmentReference(
            0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    VkAttachmentReference depthAttachmentRef = Element::VkInitializers::attachmentReference(
            1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    VkAttachmentReference colorAttachmentResolveRef = Element::VkInitializers::attachmentReference(
            depthEnabled ? 2 : 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = depthEnabled ? &depthAttachmentRef : nullptr;
    subpass.pResolveAttachments = !isMSAAx1 ? &colorAttachmentResolveRef : nullptr;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
 
    std::vector<VkAttachmentDescription> attachments;
    attachments.emplace_back(colorAttachment);
    if (depthEnabled) attachments.emplace_back(depthAttachment);
    if (!isMSAAx1) attachments.emplace_back(colorAttachmentResolve);

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(Device::getVkDevice(), &renderPassInfo, nullptr, &m_vkRenderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

void Element::RenderPass::createFrameBuffers()
{
    m_frameBuffers = std::make_unique<FrameBuffers>(m_swapChain, m_vkRenderPass);
}

void Element::RenderPass::Destroy()
{
    if(!created)
        return;

    created = false;

    if(m_vkRenderPass != VK_NULL_HANDLE)
        vkDestroyRenderPass(Device::getVkDevice(), m_vkRenderPass, nullptr);
    m_vkRenderPass = VK_NULL_HANDLE;
    if (m_frameBuffers) {
        m_frameBuffers->Destroy();
        m_frameBuffers.reset();
    }
    m_swapChain = nullptr;
}

VkRenderPass Element::RenderPass::GetVkRenderPass()
{
    return m_vkRenderPass;
}

void Element::RenderPass::begin(VkCommandBuffer vkCommandBuffer, int i)
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_vkRenderPass;
    renderPassInfo.framebuffer = m_frameBuffers->GetVkFrameBuffer(i);
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = m_swapChain->Extent();

    std::vector<VkClearValue> clearValues;
    clearValues.emplace_back();
    clearValues[0].color = { m_clearColour.r, m_clearColour.g, m_clearColour.b, 1.0f };

    if (GameSettings::get().depthEnabled)
    {
        clearValues.emplace_back();
        clearValues[1].depthStencil = { 1.0f, 0 };
    }

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(vkCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void Element::RenderPass::end(VkCommandBuffer vkCommandBuffer)
{
    vkCmdEndRenderPass(vkCommandBuffer);
}

void Element::RenderPass::setClearColour(const Vec3& clearColour)
{
    m_clearColour = clearColour;
}
