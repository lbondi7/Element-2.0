#include "DescriptorSet.h"

#include <stdexcept>

//Element::DescriptorSet::DescriptorSet(Pipeline* _pipeline, uint32_t imageCount)
//{
//    init(_pipeline, imageCount);
//}

Element::DescriptorSet::~DescriptorSet()
{
    pipeline = nullptr;
//    flush();
}

void Element::DescriptorSet::init(VknPipeline* _pipeline, uint32_t imageCount)
{
    const auto& logicalDevice = Device::getVkDevice();
    vkDeviceWaitIdle(logicalDevice);

    pipeline = _pipeline;
    std::vector<VkDescriptorSetLayout> layouts(imageCount, pipeline->GetVkDescriptorSetLayout());
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pipeline->GetVkDescriptorPool();
    allocInfo.descriptorSetCount = count = static_cast<uint32_t>(imageCount);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(imageCount);
    if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }
}

void Element::DescriptorSet::update(void* _uniformBuffers, void* _texture)
{
    const auto& logicalDevice = Device::getVkDevice();
    for (size_t i = 0; i < descriptorSets.size(); ++i) {

        std::vector<VkWriteDescriptorSet> descriptorWrites;
        for (const auto& binding : pipeline->bindingsData)
        {
            switch (binding.descriptorType)
            {
            case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER: {

                const auto& buffer = reinterpret_cast<Buffer*>(_uniformBuffers);
                descriptorWrites.emplace_back(Element::VkInitializers::writeDesciptorSet(descriptorSets[i], binding.descriptorType, 
                    &buffer->m_descriptorInfo, binding.binding));
                break;
            }
            case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: {

                const auto& texture = _texture ? reinterpret_cast<Texture*>(_texture) : DefaultResources::GetTexture();
                descriptorWrites.emplace_back(Element::VkInitializers::writeDesciptorSet(descriptorSets[i], binding.descriptorType, 
                    &texture->m_image.m_descriptorInfo, binding.binding));
                break;
            }
            }
        }

        vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void Element::DescriptorSet::update(const std::vector<Buffer>& _uniformBuffers, const Texture* _texture)
{
    const auto& logicalDevice = Device::getVkDevice();
    for (size_t i = 0; i < descriptorSets.size(); ++i) {

        std::vector<VkWriteDescriptorSet> descriptorWrites;
        for (const auto& bindingData : pipeline->bindingsData)
        {
            switch (bindingData.descriptorType)
            {
            case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER: {

                //const auto& buffer = reinterpret_cast<Buffer*>(_uniformBuffers);
                descriptorWrites.emplace_back(Element::VkInitializers::writeDesciptorSet(descriptorSets[i], bindingData.descriptorType,
                    &_uniformBuffers[i].m_descriptorInfo, bindingData.binding));
                break;
            }
            case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: {

                //const auto& texture = _texture ? _texture : DefaultResources::GetTexture().;
                descriptorWrites.emplace_back(Element::VkInitializers::writeDesciptorSet(descriptorSets[i], bindingData.descriptorType,
                    &DefaultResources::GetTexture()->m_image.m_descriptorInfo, bindingData.binding));
                break;
            }
            }
        }

        vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void Element::DescriptorSet::flush()
{
    if (!pipeline)
        return;

    const auto& logicalDevice = Device::getVkDevice();
    vkDeviceWaitIdle(logicalDevice);
    vkFreeDescriptorSets(logicalDevice, pipeline->GetVkDescriptorPool(), static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data());
    pipeline = nullptr;
}

uint32_t Element::DescriptorSet::getCount()
{
    return count;
}

Element::VknPipeline* Element::DescriptorSet::getPipeline()
{
    return pipeline;
}
