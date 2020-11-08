#pragma once

#include "Resources.h"
#include "Image.h"
#include "Device.h"
#include <string>

namespace Element {

	struct Texture
	{

		void Load(std::string file, VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		void destroy();

		void createSampler();

		void updateImageDescriptor();

		Element::TextureData data;
		Image m_image;
		VkImageView m_imageView;
		VkSampler m_sampler;
		VkImageLayout m_imageLayout;
		uint32_t mipLevels;
	};
}

