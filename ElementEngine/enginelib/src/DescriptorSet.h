#pragma once

#include "VknConstants.h"
#include "VknPipeline.h"
#include "Buffer.h"
#include "DefaultResources.h"

namespace Element{

	class DescriptorSet
	{
	public:
		DescriptorSet() = default;
		//DescriptorSet(Pipeline* _pipeline, uint32_t imageCount);
		~DescriptorSet();

		void init(VknPipeline* _pipeline, uint32_t imageCount);

		void update(void* _uniformBuffers, void* _texture);

		void update(const std::vector<Buffer>& _uniformBuffers, const Texture* _texture);

		void flush();

		uint32_t getCount();

		VknPipeline* getPipeline();

		std::vector<VkDescriptorSet> descriptorSets;

	private:

		uint32_t count;
		VknPipeline* pipeline;
	};
}

