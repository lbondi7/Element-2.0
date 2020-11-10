#include <element/Sprite.h>

void Element::Sprite::SetTransform(const Transform2D& other)
{
	transform = other;
}

const Element::Transform2D& Element::Sprite::GetTransform() const noexcept
{
	return transform;
}

Element::Transform2D& Element::Sprite::GetTransform() noexcept
{
	return transform;
}

uint32_t Element::Sprite::getId() const
{
	return id;
}

Element::Sprite::Sprite(Element::VknPipeline *pipeline, Element::Mesh *mesh, uint32_t imageCount) : m_pipeline
(pipeline), m_mesh(mesh){

}

Element::Sprite::Sprite() {

}
