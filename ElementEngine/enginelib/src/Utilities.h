#pragma once

#include <element/Maths/Vec3.h>
#include <element/Maths/Vec2.h>

#include <glm/glm.hpp>

#include <string>

namespace Element {

	namespace Utilities
	{
		const std::string extractName(const std::string& name);

		Vec3 glmvec3RefToVec3(const glm::vec3& vec3);

        Vec2 glmvec2RefToVec2(const glm::vec2& vec2);

        Vec3 glmvec3ToVec3(glm::vec3 vec3);

        Vec2 glmvec2ToVec2(glm::vec2 vec2);

        glm::vec3 vec3RefToGlmvec3(const Vec3& vec3);

        glm::vec2 vec2RefToGlmvec2(const Vec2& vec2);

        glm::vec3 vec3toGlmvec3(Vec3 vec3);

        glm::vec2 vec2ToGlmvec2(Vec2 vec2);


	};
}
