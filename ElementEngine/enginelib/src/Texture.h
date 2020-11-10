#pragma once

#include "Image.h"
#include "Device.h"
#include <string>

namespace Element {

    struct TextureData {
        void* data;
        int width;
        int height;
        int channels;

        TextureData& operator=(const TextureData& other) {
            //data = new void*();
            data = new void*(other.data);
            //memcpy(data, src_data, static_cast<size_t>(m_size));
            height = other.height;
            width = other.width;
            channels = other.channels;
            return *this;
        }

    };

	struct Texture
	{

	    Texture() = default;

	    TextureData data;

		void Load(VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		void destroy();

		void createSampler();

		void updateImageDescriptor();


		Image m_image;
		VkImageView m_imageView;
		VkSampler m_sampler;
		VkImageLayout m_imageLayout;
		uint32_t mipLevels;
	};
}

