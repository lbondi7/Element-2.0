#pragma once

#include "VknConstants.h"

namespace Element {

	struct Buffer
	{
		Buffer() = default;
		Buffer(VkDeviceSize size, VkDeviceSize offset = 0, VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		~Buffer();

		void Create(VkDeviceSize size, VkDeviceSize offset = 0, VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void Destroy();

		void MapCopyMemory(const void* src_data);

		void CopyMemory(const void* src_data, VkDeviceSize size);

		void Map();

		void Unmap();

		void CopyFromBuffer(Element::Buffer& dstBuffer);

		void updateDescriptorInfo();

		VkBuffer m_buffer;
		VkDeviceMemory m_memory;
		VkDeviceSize m_size;
		VkDeviceSize m_offset;
		VkBufferUsageFlags m_usage;
		VkMemoryPropertyFlags m_properties;
		VkDescriptorBufferInfo m_descriptorInfo;
		void* mapped = nullptr;
		bool isMapped = false;

		uint32_t binding;
	};
}