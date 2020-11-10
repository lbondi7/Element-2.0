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

void Element::DescriptorSet::init(VknPipeline* _pipeline, uint32_t imageCount, int d)
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

    //descriptors.resize(imageCount);
    descriptors.first.reserve(imageCount);
    descriptors.first.resize(imageCount);
    descriptors.second.reserve(imageCount);
    descriptors.second.resize(imageCount);
        if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, descriptors.first.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }
}

void Element::DescriptorSet::createDescWriteAndUpdate(std::vector<void*>& data)
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
                //Debugger::get().log(static_cast<float>(buffer[0].m_size));
                auto& write = descriptorWrites.emplace_back();
                writeDescriptor(write, descriptorSets[i],
                                &buffer[i].m_descriptorInfo, binding.descriptorType, binding.binding);
//                descriptorWrites.emplace_back(Element::VkInitializers::writeDesciptorSet(descriptorSets[i], binding.descriptorType,
//                    &buffer[i].m_descriptorInfo, binding.binding));
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
    created = true;
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

void Element::DescriptorSet::createDescWritesAndUpdate()
{
    const auto& logicalDevice = Device::getVkDevice();
    int i  = 0;
    for (auto& descriptor : descriptors.first) {

        auto& descriptorWrites = descriptors.second[i];
        int j = 0;
        for (const auto& binding : pipeline->bindingsData)
        {
            auto d = data[binding.binding];
            switch (binding.descriptorType)
            {
                case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER: {

                    auto buffer = reinterpret_cast<Buffer*>(d);
                    //Debugger::get().log(static_cast<float>(buffer[0].m_size));
                    if(binding.binding >= descriptorWrites.size())
                        descriptors.second[i].emplace_back();

                    buffer[i].binding = binding.binding;
                    writeDescriptor(descriptorWrites[binding.binding], descriptor,
                                    &buffer[i].m_descriptorInfo, binding.descriptorType, binding.binding);
//                    descriptor.second[binding.binding] = Element::VkInitializers::writeDesciptorSet(
//                            descriptor.first, binding.descriptorType,
//                            &buffer[i].m_descriptorInfo, binding.binding);
                    break;
                }
                case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC: {

                    break;
                }
                case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: {

                    const auto& texture =
                            d ? reinterpret_cast<Texture*>(d) : Locator::getResource()->texture("default");

                    if(binding.binding >= descriptorWrites.size())
                        descriptorWrites.emplace_back();

                    texture->m_image.binding = binding.binding;
                    writeDescriptor(descriptorWrites[binding.binding], descriptor,
                                    &texture->m_image.m_descriptorInfo, binding.descriptorType, binding.binding);

//                    descriptor.second.emplace_back(Element::VkInitializers::writeDesciptorSet(
//                            descriptor.first, binding.descriptorType,
//                    &texture->m_image.m_descriptorInfo, binding.binding));
                    break;
                }
                case VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER: {

                    break;
                }
                case VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: {

                    break;
                }
                case VkDescriptorType::VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: {

                    break;
                }
            }
            ++j;
        }
        vkUpdateDescriptorSets(logicalDevice,
                               static_cast<uint32_t>(descriptorWrites.size()),
                               descriptorWrites.data(), 0, nullptr);
        ++i;
    }
    created  = true;
}

void Element::DescriptorSet::flush()
{
    created = false;
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

void Element::DescriptorSet::writeDescriptor(VkWriteDescriptorSet& writeDescriptorSet, VkDescriptorSet descriptor,
                                             const VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type,
                                             uint32_t binding) {
    writeDescriptorSet = Element::VkInitializers::writeDesciptorSet(descriptor, type, bufferInfo, binding);
}

void Element::DescriptorSet::writeDescriptor(VkWriteDescriptorSet& writeDescriptorSet, VkDescriptorSet descriptor,
                                             const VkDescriptorImageInfo* imageInfo, VkDescriptorType type,
                                             uint32_t binding) {
    writeDescriptorSet = Element::VkInitializers::writeDesciptorSet(descriptor, type, imageInfo, binding);
}

void Element::DescriptorSet::addData(void *_data) {
    data.push_back(_data);
}

void Element::DescriptorSet::updateBufferInfo(const VkDescriptorBufferInfo* bufferInfo, uint32_t binding) {

    if(!created)
        return;

    const auto& logicalDevice = Device::getVkDevice();
    vkDeviceWaitIdle(logicalDevice);
    for (auto& descriptor : descriptors.second)
    {
        descriptor[binding].pBufferInfo = bufferInfo;

        vkUpdateDescriptorSets(logicalDevice,
                               static_cast<uint32_t>(descriptor.size()),
                               descriptor.data(), 0, nullptr);
    }
}

void Element::DescriptorSet::updateImageInfo(const VkDescriptorImageInfo* imageInfo, uint32_t binding) {

    if(!created)
        return;

    const auto& logicalDevice = Device::getVkDevice();
    vkDeviceWaitIdle(logicalDevice);
    for (auto& descriptor : descriptors.second)
    {
        descriptor[binding].pImageInfo = imageInfo;

        vkUpdateDescriptorSets(logicalDevice,
                               static_cast<uint32_t>(descriptor.size()),
                               descriptor.data(), 0, nullptr);
    }
}
