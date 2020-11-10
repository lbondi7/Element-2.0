#include <element/Model.h>


void Element::Model::SetTransform(const Transform& _transform)
{
	transform = _transform;
}

const Element::Transform& Element::Model::GetTransform() const noexcept
{
	return transform;
}

Element::Transform& Element::Model::GetTransform() noexcept
{
	return transform;
}

const Vec3& Element::Model::Position() const noexcept
{
	return transform.getPosition();
}

const Vec3& Element::Model::Rotation() const noexcept
{
	return transform.getRotation();
}

const Vec3& Element::Model::Scale() const noexcept
{
	return transform.getScale();
}