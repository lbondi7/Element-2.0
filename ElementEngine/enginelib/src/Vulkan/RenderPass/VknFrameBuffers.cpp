#include "VknFrameBuffers.h"
#include <element/GameSettings.h>
#include <Vulkan/Device/Device.h>

#include <stdexcept>

Element::FrameBuffers::FrameBuffers(VknSwapChain* swapChain, VkRenderPass vkRenderPass)
{
    init(swapChain, vkRenderPass);
}

Element::FrameBuffers::~FrameBuffers()
{
}

void Element::FrameBuffers::init(VknSwapChain* swapChain, VkRenderPass vkRenderPass){

    m_vkFrameBuffers.resize(swapChain->getImageCount());
    const auto& samples = Device::GetPhysicalDevice()->GetSelectedDevice().msaaSamples;
    bool isMSAAx1 = samples & VK_SAMPLE_COUNT_1_BIT;
    bool depthEnabled = GameSettings::get().depthEnabled;

    for (size_t i = 0; i < swapChain->getImageCount(); i++) {

        std::vector<VkImageView> attachmentVector;

        if (!isMSAAx1 && depthEnabled) attachmentVector = {
            swapChain->m_colourImageView , swapChain->m_depthImageView , swapChain->ImageViews()[i]
        };
        else if (isMSAAx1 && depthEnabled) attachmentVector = {
            swapChain->ImageViews()[i], swapChain->m_depthImageView
        };
        else if (!isMSAAx1 && !depthEnabled) attachmentVector = {
            swapChain->m_colourImageView, swapChain->ImageViews()[i]
        };
        else if (isMSAAx1 && !depthEnabled) attachmentVector = {
            swapChain->ImageViews()[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = vkRenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachmentVector.size());
        framebufferInfo.pAttachments = attachmentVector.data();
        framebufferInfo.width = swapChain->Extent().width;
        framebufferInfo.height = swapChain->Extent().height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(Device::getVkDevice(), &framebufferInfo, nullptr, &m_vkFrameBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}


void Element::FrameBuffers::Destroy()
{
    auto logicalDevice = Device::getVkDevice();
    for (auto framebuffer : m_vkFrameBuffers) {
        vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
    }
}

const std::vector<VkFramebuffer>& Element::FrameBuffers::GetVkFrameBuffers() const
{
    return m_vkFrameBuffers;
}

VkFramebuffer Element::FrameBuffers::GetVkFrameBuffer(int i)
{
    return m_vkFrameBuffers[i];
}
