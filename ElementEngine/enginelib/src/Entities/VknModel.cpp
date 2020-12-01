#include "VknModel.h"
#include <Misc/Utilities.h>
#include <Resource/Resources.h>
#include <Misc/Locator.h>

#include <element/Debugger.h>

#include <Physics/Physics.h>

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


	if(descriptorSet)
	    descriptorSet->updateImageInfo(texture, _texture ? _texture : Locator::getResource()->texture("default"));
    texture = _texture;
}

void Element::VknModel::SetPipeline(VknPipeline* _pipeline)
{
	if (pipeline == _pipeline)
		return;

	pipeline = _pipeline;

	descriptorSet->init(_pipeline, descriptorSet->getCount(), 2);

	//descriptorSet->flush();
	//descriptorSet->init(pipeline, descriptorSet->getCount());
	dirty = DirtyFlags::PIPELINE_DIRTY;
}

const std::vector<Element::VknBuffer>& Element::VknModel::GetUniformBuffers() const noexcept
{
	return uniformBuffers;
}

std::vector<Element::VknBuffer>& Element::VknModel::GetUniformBuffers() noexcept
{
	return uniformBuffers;
}

Element::DirtyFlags Element::VknModel::isDirty()
{
	return dirty;
}

void Element::VknModel::setDirty(Element::DirtyFlags flag)
{
	dirty = flag;
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
    if(destroyed)
        return;

    destroyed = true;

	descriptorSet->flush();
	mesh = nullptr;
	texture = nullptr;
	pipeline = nullptr;
	oldPipeline = nullptr;

	for (auto& buffer : uniformBuffers)
	{
        buffer.Destroy2();
	}
}

void Element::VknModel::init(VknPipeline* _pipeline, uint32_t imageCount)
{
	entityState = EntityState::NOT_RENDERED;
	prevEntityState = EntityState::NOT_RENDERED;
	pipeline = _pipeline;
	VkDeviceSize bufferSize = sizeof(glm::mat4);
	uniformBuffers.resize(imageCount);
	for (auto& buffer : uniformBuffers)
	{
        //Debugger::get().log("Model Buffer", Colour::BRIGHT_CYAN);
		buffer.CreateAlloc(bufferSize, 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		//buffer.Map();
	}
	//descriptorSet = std::make_unique<DescriptorSet>();

    SetTexture(Locator::getResource()->texture("default"));
	descriptorSet = std::make_unique<VknDescriptorSet>(pipeline, imageCount, 2);
	descriptorSet->addData(uniformBuffers.data(), 0);
	descriptorSet->addData(texture, 1);
	descriptorSet->addData(Locator::getResource()->material("test")->uniformBuffers.data(), 2);
	descriptorSet->createDescWritesAndUpdate();

	collider = Physics::get().getCollider(&transformMatrix);
	collider->transform.setPositionX(-2.f);

}

void Element::VknModel::reInit(uint32_t imageCount)
{
	entityState = EntityState::NOT_RENDERED;
	prevEntityState = EntityState::NOT_RENDERED;
    descriptorSet->init(descriptorSet->getPipeline(), imageCount, descriptorSet->getId());
    descriptorSet->createDescWritesAndUpdate();

	dirty = DirtyFlags::DIRTY;
}

void Element::VknModel::updateUniformBuffers(uint32_t imageIndex) {

    if (!transform.isUpdated())
        return;

    auto pos = Utilities::vec3RefToGlmvec3(transform.getPosition());
    pos.y *= -1;
    transformMatrix = glm::translate(glm::identity<glm::mat4>(), pos);
    transformMatrix *= glm::yawPitchRoll(transform.getRotation().y, transform.getRotation().x, transform.getRotation().z);
    transformMatrix = glm::scale(transformMatrix, Utilities::vec3RefToGlmvec3(transform.getScale()));
    uniformBuffers[imageIndex].MapCopyMemory2(&transformMatrix);

    if(collider)
        collider->update(imageIndex);
}

Element::VknDescriptorSet *Element::VknModel::getDescriptorSet() const {
    return descriptorSet.get();
}

const glm::mat4 &Element::VknModel::getTransformMatrix() const {
    return transformMatrix;
}
