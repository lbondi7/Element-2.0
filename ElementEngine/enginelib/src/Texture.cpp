#include "Texture.h"

#include "Buffer.h"
#include "VkFunctions.h"

#include <stb_image.h>

#include <stdexcept>

//void Element::Texture::Load(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue)
//{
//	Element::TextureData texture = Element::Resources::LoadTextureData("textures/texture.jpg");
//
//	VkDeviceSize imageSize = data.width * data.height * 4;
//
//	Buffer stagingBuffer;
//
//	stagingBuffer.Create(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
//	stagingBuffer.MapMemory(data.data);
//	stbi_image_free(data.data);
//
//	m_image.Create(data.width, data.height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//
//
//	m_image.transitionLayout(m_image.m_format, 
//		VK_IMAGE_LAYOUT_UNDEFINED, 
//		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
//		0, 
//		VK_ACCESS_TRANSFER_WRITE_BIT, 
//		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
//		VK_PIPELINE_STAGE_TRANSFER_BIT);
//
//	m_image.CopyFromBuffer(stagingBuffer.m_buffer, data.width, data.height);
//	m_image.transitionLayout(m_image.m_format, 
//		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
//		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
//		VK_ACCESS_TRANSFER_WRITE_BIT, 
//		VK_ACCESS_SHADER_READ_BIT, 
//		VK_PIPELINE_STAGE_TRANSFER_BIT, 
//		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
//
//	stagingBuffer.Destroy(device);
//
//	m_imageView = Element::VkFunctions::createImageView(device, m_image.m_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
//
//	createSampler(device);
//
//}

void Element::Texture::Load(std::string file, VkImageLayout imageLayout)
{
	Element::Resources::LoadTextureData(file, data);
	//Element::TextureData texture = Element::Resources::LoadTextureData("texture.jpg");

	VkDeviceSize imageSize = data.width * data.height * 4;

	mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(data.width, data.height)))) + 1;

	Buffer stagingBuffer(imageSize);

	//stagingBuffer.Create(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	stagingBuffer.MapCopyMemory(data.data);
	stbi_image_free(data.data);

	m_image.Create(data.width, data.height, mipLevels, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	m_image.transitionLayout(m_image.m_format, mipLevels,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		0,
		VK_ACCESS_TRANSFER_WRITE_BIT,
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT);

	m_image.CopyFromBuffer(stagingBuffer.m_buffer, data.width, data.height);

	//m_image.transitionLayout(m_image.m_format, mipLevels,
	//	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
	//	imageLayout,
	//	VK_ACCESS_TRANSFER_WRITE_BIT,
	//	VK_ACCESS_SHADER_READ_BIT,
	//	VK_PIPELINE_STAGE_TRANSFER_BIT,
	//	VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

	stagingBuffer.Destroy();

	m_image.generateMipmaps();

	m_imageView = Element::VkFunctions::createImageView(Device::getVkDevice(), m_image.m_vkImage, m_image.m_mipLevels, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	m_imageLayout = imageLayout;
	createSampler();
	updateImageDescriptor();
}


void Element::Texture::destroy()
{
	auto logicalDevice = Device::getVkDevice();
	vkDestroySampler(logicalDevice, m_sampler, nullptr);
	vkDestroyImageView(logicalDevice, m_imageView, nullptr);
	m_image.Destroy();

}

void Element::Texture::createSampler() {

	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16.0f;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.minLod = 0.0f; // Optional
	samplerInfo.maxLod = static_cast<float>(m_image.m_mipLevels);
	samplerInfo.mipLodBias = 0.0f; // Optional

	if (vkCreateSampler(Device::getVkDevice(), &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}
}

void Element::Texture::updateImageDescriptor()
{
	m_image.m_descriptorInfo.imageView = m_imageView;
	m_image.m_descriptorInfo.sampler = m_sampler;
	m_image.m_descriptorInfo.imageLayout = m_imageLayout;
}
