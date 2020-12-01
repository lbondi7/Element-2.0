#include "VknSprite.h"

#include <Misc/Utilities.h>
#include <Resource/Resources.h>
#include <Misc/Locator.h>

#include <element/IdHandler.h>
#include <element/Debugger.h>

#include <glm/gtc/matrix_transform.hpp>

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
		transform.setSize(Vec2(static_cast<float>(texture->data.width),
                               static_cast<float>(texture->data.height)));

    if(descriptorSet)
        descriptorSet->updateImageInfo(m_texture, texture ? texture : Locator::getResource()->texture("default"));
	if(dirty == DirtyFlags::CLEAN)
		dirty = DirtyFlags::DIRTY;
}

void Element::VknSprite::SetPipeline(VknPipeline* pipeline)
{
    if (m_pipeline == pipeline)
        return;

    m_pipeline = pipeline;
	//descriptorSet->flush();
	//descriptorSet->init(m_pipeline, descriptorSet->getCount());
	dirty = DirtyFlags::PIPELINE_DIRTY;

    descriptorSet->init(pipeline, descriptorSet->getCount(), 2);
}

const Element::VknPipeline* Element::VknSprite::GetOldPipeline() const noexcept
{
	return oldPipeline;
}

Element::DirtyFlags Element::VknSprite::isDirty()
{
	return dirty;
}

void Element::VknSprite::setDirty(Element::DirtyFlags flag)
{
	dirty = flag;
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
    if(destroyed)
        return;

    destroyed = true;

	descriptorSet->flush();
	m_mesh = nullptr;
	m_texture = nullptr;
	m_pipeline = nullptr;
	oldPipeline = nullptr;

	for (auto& buffer : uniformBuffers)
	{
        buffer.Destroy2();
	}
}

void Element::VknSprite::init(VknPipeline* pipeline, Mesh* mesh, uint32_t imageCount) {
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	uniformBuffers.resize(imageCount);
	for (auto& buffer : uniformBuffers)
	{
		buffer.CreateAlloc(bufferSize, 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
	}
	entityState = EntityState::NOT_RENDERED;
	prevEntityState = EntityState::NOT_RENDERED;
	m_pipeline = pipeline;
	m_mesh = mesh;
	//descriptorSet = std::make_unique<DescriptorSet>();
	//descriptorSet->init(m_pipeline, imageCount);
	SetTexture(Locator::getResource()->texture("default"), false);
    descriptorSet = std::make_unique<VknDescriptorSet>(pipeline, imageCount, 2);
    descriptorSet->addData(uniformBuffers.data(), 0);
    descriptorSet->addData(m_texture, 1);
    descriptorSet->addData(Locator::getResource()->material("test")->uniformBuffers.data(), 2);
    descriptorSet->createDescWritesAndUpdate();
}

void Element::VknSprite::reInit(uint32_t imageCount) {
	entityState = EntityState::NOT_RENDERED;
	prevEntityState = EntityState::NOT_RENDERED;
    descriptorSet->init(descriptorSet->getPipeline(), imageCount, descriptorSet->getId());
    descriptorSet->createDescWritesAndUpdate();

	dirty = DirtyFlags::DIRTY;
}

void Element::VknSprite::updateUniformBuffers(uint32_t imageIndex) {
    if (!transform.isUpdated())
        return;

    const auto& scalar = glm::vec3(transform.getSize().x * transform.getScale().x,
                                   transform.getSize().y * transform.getScale().y, 1.0f);
    auto pos = Utilities::vec3RefToGlmvec3(transform.getPosition());
    pos.y *= -1;
    transformMatrix = glm::translate(glm::identity<glm::mat4>(), pos + scalar);
    transformMatrix = glm::scale(transformMatrix, scalar);
    transformMatrix = glm::rotate(transformMatrix, glm::radians(transform.getRotationZ()), glm::vec3(0.0f, 0.0f, 1.0f));

    uniformBuffers[imageIndex].MapCopyMemory2(&transformMatrix);
}

Element::VknDescriptorSet *Element::VknSprite::getDescriptorSet() const {
    return descriptorSet.get();
}

const glm::mat4 &Element::VknSprite::getTransformMatrix() const {
    return transformMatrix;
}
