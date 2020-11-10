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

		void update(std::vector<void*>& data);

        void update();

		void update(const std::vector<Buffer>& _uniformBuffers, const Texture* _texture);

		void flush();

		uint32_t getCount();

		VknPipeline* getPipeline();

		std::vector<VkDescriptorSet> descriptorSets;
        std::vector<void*> data;
	private:
		uint32_t count;
		VknPipeline* pipeline;
	};
}

