#include "Utilities.h"

const std::string Element::Utilities::extractName(const std::string& _name)
{
    std::string name = "";

    for (const auto& c : _name)
    {
        if (c == '/')
        {
            name.clear();
            continue;
        }
        else if (c == '.')
        {
            break;
        }
        name += c;
    }

    return name;
}

Vec3 Element::Utilities::glmvec3RefToVec3(const glm::vec3 &vec3) {
    return Vec3(vec3.x, vec3.y, vec3.z);
}

Vec2 Element::Utilities::glmvec2RefToVec2(const glm::vec2 &vec2) {
    return Vec2(vec2.x, vec2.y);
}

glm::vec3 Element::Utilities::vec3RefToGlmvec3(const Vec3 &vec3) {
    return glm::vec3(vec3.x, vec3.y, vec3.z);
}

glm::vec2 Element::Utilities::vec2RefToGlmvec2(const Vec2 &vec2) {
    return glm::vec2(vec2.x, vec2.y);
}

Vec3 Element::Utilities::glmvec3ToVec3(glm::vec3 vec3) {
    return Vec3(vec3.x, vec3.y, vec3.z);
}

Vec2 Element::Utilities::glmvec2ToVec2(glm::vec2 vec2) {
    return Vec2(vec2.x, vec2.y);
}

[[maybe_unused]] glm::vec3 Element::Utilities::vec3toGlmvec3(Vec3 vec3) {
    return glm::vec3(vec3.x, vec3.y, vec3.z);
}

glm::vec2 Element::Utilities::vec2ToGlmvec2(Vec2 vec2) {
    return glm::vec2(vec2.x, vec2.y);
}

