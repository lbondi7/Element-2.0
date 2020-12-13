#pragma once

#include <Vulkan/VknConstants.h>
#include <Vulkan/VknBuffer.h>
#include <Vulkan/Pipeline/DescriptorInfo.h>

#include <map>

namespace Element{

    class VknPipeline;
    class Texture;

	class VknDescriptorSet
	{
	public:
		VknDescriptorSet() = default;
		VknDescriptorSet(VknPipeline* _pipeline, uint32_t imageCount, int id);
		~VknDescriptorSet();

        void init(VknPipeline *_pipeline, uint32_t imageCount, int id);

        void reInit(uint32_t imageCount);

        void createDescWritesAndUpdate();

		void flush();

		uint32_t getCount();

		VknPipeline* getPipeline();

		void addData(void* _data, int binding);

        void updateBufferInfo(const VkDescriptorBufferInfo *bufferInfo, uint32_t binding = 1);

        void updateImageInfo(Texture* oldTexture, Texture* newTexture);

		std::vector<VkDescriptorSet> descriptorSets;
        std::vector<void*> data;
        std::map<uint32_t, void*> data_map;

        [[nodiscard]] int getId() const;

        [[nodiscard]] VkDescriptorSet getDescriptorSet(int i) const;

	private:
		uint32_t count;
		VknPipeline* pipeline;
        int id;
		bool created = false;
        std::pair<std::vector<VkDescriptorSet>, std::vector<std::vector<VkWriteDescriptorSet>>> descriptors;

        void writeDescriptor(VkWriteDescriptorSet &writeDescriptorSet, VkDescriptorSet descriptor,
                             const VkDescriptorBufferInfo *bufferInfo, VkDescriptorType type, uint32_t binding);

        void writeDescriptor(VkWriteDescriptorSet &writeDescriptorSet, VkDescriptorSet descriptor,
                             const VkDescriptorImageInfo *imageInfo, VkDescriptorType type, uint32_t binding);
    };
}

