#include "VknModel.h"
#include "Utilities.h"
#include "Resources.h"
#include "Locator.h"
#include "VknResources.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

Element::VknModel::VknModel()
{

}

Element::VknModel::VknModel(VknPipeline* _pipeline)
{
	pipeline = _pipeline;
}

Element::VknModel::VknModel(VknPipeline* _pipeline, uint32_t imageCount)
{
	init(_pipeline, imageCount);
}

Element::VknModel::~VknModel()
{

}

Element::Mesh* Element::VknModel::GetMesh()
{
	return mesh;
}

Element::Texture* Element::VknModel::GetTexture()
{
	return texture;
}

Element::VknPipeline* Element::VknModel::GetPipeline()
{
	return pipeline;
}


void Element::VknModel::SetMesh(Mesh* _mesh)
{
	if (mesh == _mesh)
		return;

	if (dirty == DirtyFlags::CLEAN)
		dirty = DirtyFlags::DIRTY;

	mesh = _mesh;
}

void Element::VknModel::SetTexture(Texture* _texture)
{
	if (texture == _texture)
		return;

	//descriptorSet->createDescWritesAndUpdate(uniformBuffers, texture);
	if(dirty == DirtyFlags::CLEAN)
		dirty = DirtyFlags::DIRTY;

	texture = _texture;
	descriptorSet->updateImageInfo(texture ? &texture->m_image.m_descriptorInfo :
                                &Locator::getResource()->texture("default")->m_image.m_descriptorInfo, 1);
}

void Element::VknModel::SetPipeline(VknPipeline* _pipeline)
{
	if (pipeline == _pipeline)
		return;

	pipeline = _pipeline;
	//descriptorSet->flush();
	//descriptorSet->init(pipeline, descriptorSet->getCount());
	dirty = DirtyFlags::PIPELINE_DIRTY;
}

//const Element::Pipeline* Element::VknModel::GetOldPipeline() const
//{
//	return oldPipeline;
//}

const std::vector<Element::Buffer>& Element::VknModel::GetUniformBuffers() const noexcept
{
	return uniformBuffers;
}

std::vector<Element::Buffer>& Element::VknModel::GetUniformBuffers() noexcept
{
	return uniformBuffers;
}

DirtyFlags Element::VknModel::isDirty()
{
	return dirty;
}

void Element::VknModel::setDirty(DirtyFlags flag)
{
	dirty = flag;
}

void Element::VknModel::updateUniformBuffers(bool cameraChanged, const glm::mat4& viewMatrix, const glm::mat4& projMatrix,uint32_t imageIndex)
{

	if (!transform.isUpdated() && !cameraChanged)
		return;

	UniformBufferObject ubo{};
	auto pos = Utilities::vec3RefToGlmvec3(transform.getPosition());
	pos.y *= -1;
	ubo.model = glm::translate(ubo.model, pos);
	ubo.model *= glm::yawPitchRoll(transform.getRotation().y, transform.getRotation().x, transform.getRotation().z);
	ubo.model = glm::scale(ubo.model, Utilities::vec3RefToGlmvec3(transform.getScale()));
//	ubo.view = viewMatrix;
//	ubo.proj = projMatrix;

	uniformBuffers[imageIndex].CopyMemory(&ubo, sizeof(ubo));
}

void Element::VknModel::setEntityState(EntityState state)
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

Element::EntityState Element::VknModel::getEntityState()
{
	return entityState;
}

Element::EntityState Element::VknModel::getPrevEntityState()
{
	return prevEntityState;
}

void Element::VknModel::destroy()
{
	//descriptorSet->flush();
	mesh = nullptr;
	texture = nullptr;
	pipeline = nullptr;
	oldPipeline = nullptr;

	for (auto& buffer : uniformBuffers)
	{
        buffer.Destroy();
	}
}

void Element::VknModel::init(VknPipeline* _pipeline, uint32_t imageCount)
{
	entityState = EntityState::NOT_RENDERED;
	prevEntityState = EntityState::NOT_RENDERED;
	pipeline = _pipeline;
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	uniformBuffers.resize(imageCount);
	for (auto& buffer : uniformBuffers)
	{
		buffer.Create(bufferSize, 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
		buffer.Map();
	}
	//descriptorSet = std::make_unique<DescriptorSet>();
	descriptorSet = Locator::getVknResource()->allocateDescriptorSet();
	descriptorSet->init(pipeline, imageCount, 2);
	SetTexture(Locator::getResource()->texture("default"));
}

void Element::VknModel::reInit(uint32_t imageCount)
{
	entityState = EntityState::NOT_RENDERED;
	prevEntityState = EntityState::NOT_RENDERED;
	//descriptorSet = std::make_unique<DescriptorSet>();
	descriptorSet->flush();
	descriptorSet->init(pipeline, imageCount, 2);
    //std::vector<void*>data{uniformBuffers.data(), texture};
	descriptorSet->createDescWritesAndUpdate();
	dirty = DirtyFlags::DIRTY;
}

Element::VknModel::VknModel(Element::VknPipeline *pipeline, uint32_t imageCount, std::vector<Buffer> &cameraBuffers) {

    init(pipeline, imageCount, cameraBuffers);
}

void Element::VknModel::init(VknPipeline* _pipeline, uint32_t imageCount, std::vector<Buffer> &cameraBuffers)
{
    entityState = EntityState::NOT_RENDERED;
    prevEntityState = EntityState::NOT_RENDERED;
    pipeline = _pipeline;
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    uniformBuffers.resize(imageCount);
    for (auto& buffer : uniformBuffers)
    {
        buffer.Create(bufferSize, 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        buffer.Map();
    }

    descriptorSet->init(pipeline, imageCount);
    SetTexture(Locator::getResource()->texture("default"));
}

void Element::VknModel::updateUniformBuffers(bool cameraChanged, UniformBufferObject &ubo, uint32_t
imageIndex) {

    if (!transform.isUpdated() && !cameraChanged)
        return;

    auto pos = Utilities::vec3RefToGlmvec3(transform.getPosition());
    pos.y *= -1;
    ubo.model = glm::translate(ubo.model, pos);
    ubo.model *= glm::yawPitchRoll(transform.getRotation().y, transform.getRotation().x, transform.getRotation().z);
    ubo.model = glm::scale(ubo.model, Utilities::vec3RefToGlmvec3(transform.getScale()));

    uniformBuffers[imageIndex].CopyMemory(&ubo, sizeof(ubo));
}
