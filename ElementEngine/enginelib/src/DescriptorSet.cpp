#include "DescriptorSet.h"

#include "Locator.h"
#include "Resources.h"
#include "VknPipeline.h"
#include "VkFunctions.h"
#include "Texture.h"
#include <element/Debugger.h>

#include <stdexcept>

Element::DescriptorSet::DescriptorSet(Element::VknPipeline *_pipeline, uint32_t imageCount, int id) {

    init(_pipeline, imageCount, id);
}

Element::DescriptorSet::~DescriptorSet()
{
    pipeline = nullptr;
    flush();
}

//void Element::DescriptorSet::init(VknPipeline* _pipeline, uint32_t imageCount)
//{
//    const auto& logicalDevice = Device::getVkDevice();
//    vkDeviceWaitIdle(logicalDevice);
//
//    pipeline = _pipeline;
//    std::vector<VkDescriptorSetLayout> layouts(imageCount, pipeline->GetVkDescriptorSetLayout());
//    VkDescriptorSetAllocateInfo allocInfo{};
//    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
//    allocInfo.descriptorSetCount = count = static_cast<uint32_t>(imageCount);
//    allocInfo.descriptorPool = pipeline->allocateDescriptorPool(count);
//    allocInfo.pSetLayouts = layouts.data();
//
//    descriptorSets.resize(imageCount);
//    if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
//        throw std::runtime_error("failed to allocate descriptor sets!");
//    }
//}

void Element::DescriptorSet::init(VknPipeline* _pipeline, uint32_t imageCount, int _id)
{
    flush();
    const auto& logicalDevice = Device::getVkDevice();
    vkDeviceWaitIdle(logicalDevice);

    pipeline = _pipeline;
    id = _id;
    std::vector<VkDescriptorSetLayout> layouts(imageCount, pipeline->getVkDescriptorSetLayout(id));
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
    created  = true;
}

void Element::DescriptorSet::reInit(uint32_t imageCount)
{
    flush();
    const auto& logicalDevice = Device::getVkDevice();
    vkDeviceWaitIdle(logicalDevice);

    std::vector<VkDescriptorSetLayout> layouts(imageCount, pipeline->getVkDescriptorSetLayout(id));
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorSetCount = count = imageCount;
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
    created  = true;
}

void Element::DescriptorSet::createDescWritesAndUpdate()
{
    const auto& logicalDevice = Device::getVkDevice();
    int i  = 0;
    for (auto& descriptor : descriptors.first) {

        auto& descriptorWrites = descriptors.second[i];
        int j = 0;
        for (const auto& binding : pipeline->GetPipelineData().shaderInfo)
        {
            if(id != binding.set)
                continue;

            auto d = data_map[binding.binding];
            auto desType = VkFunctions::getDescriptorType(binding.bindObjectType);
            switch (desType)
            {
                case VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
                case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                case VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
                case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC: {

                    auto buffer = static_cast<Buffer*>(d);
                    if(buffer) {
                        //Debugger::get().log(static_cast<float>(buffer[0].m_size));
                        descriptorWrites.emplace_back();

                        buffer[i].binding = binding.binding;
                        writeDescriptor(descriptorWrites[j], descriptor,
                                        &buffer[i].m_descriptorInfo, desType, binding.binding);
                        break;
                    }
                }
                case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: {

                    const auto& texture = static_cast<Texture*>(d);
                    if(texture) {
//                        if(binding.binding >= descriptorWrites.size())
                            descriptorWrites.emplace_back();

                        texture->m_image.binding = binding.binding;
                        writeDescriptor(descriptorWrites[j], descriptor,
                                        &texture->m_image.m_descriptorInfo, desType, binding.binding);

                        break;
                    }
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
        descriptorWrites.reserve(descriptorWrites.size());
    }
    created  = true;
}

void Element::DescriptorSet::flush()
{
    created = false;

    for (int i = 0; i < descriptors.first.size(); ++i)
    {
        descriptors.first.clear();
        for (auto& writes : descriptors.second)
            writes.clear();
        descriptors.second.clear();
    }
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

void Element::DescriptorSet::addData(void *_data, int binding) {
    data_map[binding] = _data;
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

void Element::DescriptorSet::updateImageInfo(Texture* oldTexture, Texture* newTexture) {

    if(!created)
        return;

    const auto& logicalDevice = Device::getVkDevice();
    vkDeviceWaitIdle(logicalDevice);

    for (auto& descriptor : descriptors.second)
    {

        for (auto& write : descriptor)
        {
            if(write.pImageInfo && write.pImageInfo == &oldTexture->m_image.m_descriptorInfo){
                write.pImageInfo = &newTexture->m_image.m_descriptorInfo;
                break;
            }
        }

        vkUpdateDescriptorSets(logicalDevice,
                               static_cast<uint32_t>(descriptor.size()),
                               descriptor.data(), 0, nullptr);
    }
}

int Element::DescriptorSet::getId() const {
    return id;
}

VkDescriptorSet Element::DescriptorSet::getDescriptorSet(int i) const {
    return descriptors.first[i];
}
