#pragma once

#include "VknConstants.h"
#include "Device.h"

namespace Element {

	struct Image
	{
	public:

		void Create(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);

		void Destroy();

		void CopyFromBuffer(VkCommandPool commandPool, VkQueue queue, VkBuffer buffer, uint32_t width, uint32_t height);

		void CopyFromBuffer(VkBuffer buffer, uint32_t width, uint32_t height);

		void transitionLayout(VkFormat format, uint32_t mipLevels, VkImageLayout oldLayout, VkImageLayout newLayout, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage);

		void generateMipmaps();

		VkImage m_vkImage;
		VkDeviceMemory m_memory;
		VkFormat m_format;
		VkImageUsageFlags m_usage;
		int32_t m_width;
		int32_t m_height;
		uint32_t m_mipLevels;
		VkDescriptorImageInfo m_descriptorInfo;
	};
}
