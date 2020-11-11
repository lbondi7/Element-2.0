#pragma once

#include "VknConstants.h"
#include "Buffer.h"
#include "Texture.h"

namespace Element{

    class VknPipeline;

	class DescriptorSet
	{
	public:
		DescriptorSet() = default;
		//DescriptorSet(Pipeline* _pipeline, uint32_t imageCount);
		~DescriptorSet();

		void init(VknPipeline* _pipeline, uint32_t imageCount);

        void init(VknPipeline *_pipeline, uint32_t imageCount, int d);

		void createDescWriteAndUpdate(std::vector<void*>& data);

        void createDescWritesAndUpdate();

		void update(const std::vector<Buffer>& _uniformBuffers, const Texture* _texture);

		void flush();

		uint32_t getCount();

		VknPipeline* getPipeline();

		void addData(void* _data);

		void replaceData(void* _data, int placement);

		//void updateDescriptorSet(void* _data, uint32_t binding = -1);

        void updateBufferInfo(const VkDescriptorBufferInfo *bufferInfo, uint32_t binding = 1);

        void updateImageInfo(const VkDescriptorImageInfo *imageInfo, uint32_t binding = -1);

		std::vector<VkDescriptorSet> descriptorSets;
        std::vector<void*> data;

        //std::vector<std::pair<VkDescriptorSet, std::vector<VkWriteDescriptorSet>>> descriptors;

        std::pair<std::vector<VkDescriptorSet>, std::vector<std::vector<VkWriteDescriptorSet>>> descriptors;
	private:
		uint32_t count;
		VknPipeline* pipeline;

		bool created = false;

        void writeDescriptor(VkWriteDescriptorSet &writeDescriptorSet, VkDescriptorSet descriptor,
                             const VkDescriptorBufferInfo *bufferInfo, VkDescriptorType type, uint32_t binding);

        void writeDescriptor(VkWriteDescriptorSet &writeDescriptorSet, VkDescriptorSet descriptor,
                             const VkDescriptorImageInfo *imageInfo, VkDescriptorType type, uint32_t binding);
    };
}

