#include "DescriptorSet.h"

#include "Locator.h"
#include "Resources.h"
#include "VknPipeline.h"
#include <element/Debugger.h>

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
    allocInfo.descriptorSetCount = count = static_cast<uint32_t>(imageCount);
    allocInfo.descriptorPool = pipeline->allocateDescriptorPool(count);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(imageCount);
    if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }
}

void Element::DescriptorSet::update(std::vector<void*>& data)
{
    const auto& logicalDevice = Device::getVkDevice();
    for (size_t i = 0; i < descriptorSets.size(); ++i) {

        std::vector<VkWriteDescriptorSet> descriptorWrites;
        for (const auto& binding : pipeline->bindingsData)
        {
            auto d = data[binding.binding];
            switch (binding.descriptorType)
            {
            case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER: {

                auto buffer = reinterpret_cast<Buffer*>(d);
                Debugger::get().log(static_cast<float>(buffer[0].m_size));
                descriptorWrites.emplace_back(Element::VkInitializers::writeDesciptorSet(descriptorSets[i], binding.descriptorType, 
                    &buffer[i].m_descriptorInfo, binding.binding));
                break;
            }
            case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: {

                const auto& texture = d ? reinterpret_cast<Texture*>(d) : Locator::getResource()->texture("default");
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
                                                                                         &Locator::getResource()->texture("default")->m_image
                                                                                         .m_descriptorInfo, bindingData.binding));
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

void Element::DescriptorSet::update() {
    const auto& logicalDevice = Device::getVkDevice();
    for (size_t i = 0; i < descriptorSets.size(); ++i) {

        std::vector<VkWriteDescriptorSet> descriptorWrites;
        for (const auto& binding : pipeline->bindingsData)
        {
            const auto& d = data[binding.binding];
            switch (binding.descriptorType)
            {
                case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER: {

                    const auto& buffer = reinterpret_cast<Buffer*>(d);
                    descriptorWrites.emplace_back(Element::VkInitializers::writeDesciptorSet(descriptorSets[i], binding.descriptorType,
                                                                                             &buffer[i].m_descriptorInfo, binding.binding));
                    break;
                }
                case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: {

                    const auto& texture = d ? reinterpret_cast<Texture*>(d) : Locator::getResource()->texture("default");
                    descriptorWrites.emplace_back(Element::VkInitializers::writeDesciptorSet(descriptorSets[i], binding.descriptorType,
                                                                                             &texture->m_image.m_descriptorInfo, binding.binding));
                    break;
                }
            }
        }

        vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}
