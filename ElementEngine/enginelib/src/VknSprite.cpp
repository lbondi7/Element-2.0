#include "VknSprite.h"

#include "DefaultResources.h"
#include <element/IdHandler.h>

#include <glm/gtc/matrix_transform.hpp>

Element::VknSprite::VknSprite()
{

}

Element::VknSprite::VknSprite(VknPipeline* pipeline, Mesh* mesh, uint32_t imageCount)
{
	init(pipeline, mesh, imageCount);
}

Element::VknSprite::~VknSprite()
{

}

Element::Mesh* Element::VknSprite::GetMesh()
{
	return m_mesh;
}

Element::Texture* Element::VknSprite::GetTexture() noexcept
{
	return m_texture;
}

Element::VknPipeline* Element::VknSprite::GetPipeline() noexcept
{
	return m_pipeline;
}

const Element::Mesh* Element::VknSprite::GetMesh() const
{
	return m_mesh;
}

const Element::Texture* Element::VknSprite::GetTexture() const noexcept
{
	return m_texture;
}

const Element::VknPipeline* Element::VknSprite::GetPipeline() const noexcept
{
	return m_pipeline;
}

void Element::VknSprite::SetTexture(Texture* texture, bool keepSize)
{
	m_texture = texture;

	if(!keepSize)
		transform.setSize(glm::vec2(texture->data.width, texture->data.height));

	descriptorSet->update(uniformBuffers, m_texture);
	if(dirty == DirtyFlags::CLEAN)
		dirty = DirtyFlags::DIRTY;
}

void Element::VknSprite::SetPipeline(VknPipeline* pipeline)
{
	m_pipeline = pipeline;
	//descriptorSet->flush();
	descriptorSet->init(m_pipeline, descriptorSet->getCount());
	dirty = DirtyFlags::PIPELINE_DIRTY;
}

const Element::VknPipeline* Element::VknSprite::GetOldPipeline() const noexcept
{
	return oldPipeline;
}

DirtyFlags Element::VknSprite::isDirty()
{
	return dirty;
}

void Element::VknSprite::setDirty(DirtyFlags flag)
{
	dirty = flag;
}

void Element::VknSprite::updateUniformBuffers(bool cameraChanged, const glm::mat4& viewMatrix, const glm::mat4& projMatrix, uint32_t imageIndex)
{
	if (!transform.isUpdated() && !cameraChanged)
		return;

	UniformBufferObject ubo{};
	const auto& scalar = glm::vec3(transform.getSize().x * transform.getScale().x,
		transform.getSize().y * transform.getScale().y, 1.0f);
	glm::vec3 pos = transform.getPosition();
	pos.y *= -1;
	ubo.model = glm::translate(ubo.model, pos + scalar);
	ubo.model = glm::scale(ubo.model, scalar);
	ubo.model = glm::rotate(ubo.model, glm::radians(transform.getRotationZ()), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = viewMatrix;
	ubo.proj = projMatrix;

	uniformBuffers[imageIndex].CopyMemory(&ubo, sizeof(ubo));
}

void Element::VknSprite::setEntityState(EntityState state)
{
	if (state == EntityState::DESTROY)
	{
		entityState = state;
		return;
	}

	if (state != EntityState::RENDERED)
	{
		prevEntityState = entityState;
	}
	entityState = state;
}

Element::EntityState Element::VknSprite::getEntityState()
{
	return entityState;
}

Element::EntityState Element::VknSprite::getPrevEntityState()
{
	return prevEntityState;
}

void Element::VknSprite::destroy()
{
	//descriptorSet->flush();
	m_mesh = nullptr;
	m_texture = nullptr;
	m_pipeline = nullptr;
	oldPipeline = nullptr;

	for (auto& buffer : uniformBuffers)
	{
		buffer.Destroy();
	}
}

void Element::VknSprite::init(VknPipeline* pipeline, Mesh* mesh, uint32_t imageCount) {
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	uniformBuffers.resize(imageCount);
	for (auto& buffer : uniformBuffers)
	{
		buffer.Create(bufferSize, 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
		buffer.Map();
	}
	entityState = EntityState::NOT_RENDERED;
	prevEntityState = EntityState::NOT_RENDERED;
	m_pipeline = pipeline;
	m_mesh = mesh;
	descriptorSet = std::make_unique<DescriptorSet>();
	descriptorSet->init(m_pipeline, imageCount);
	SetTexture(DefaultResources::GetTexture(), false);
	//descriptorSet->update(uniformBuffers, m_texture);
	id = IdHandler::GetID();
}

void Element::VknSprite::reInit(uint32_t imageCount) {
	entityState = EntityState::NOT_RENDERED;
	prevEntityState = EntityState::NOT_RENDERED;
	descriptorSet = std::make_unique<DescriptorSet>();
	descriptorSet->init(m_pipeline, imageCount);
	descriptorSet->update(uniformBuffers, m_texture);
	dirty = DirtyFlags::DIRTY;
}