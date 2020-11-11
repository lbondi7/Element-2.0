//
// Created by lbondi7 on 09/11/2020.
//

#include "Material.h"
#include <element/Debugger.h>

#include <type_traits>
#include <glm/glm.hpp>

struct MatData{
    alignas(16) Vec3 ambient = Vec3();
    alignas(16) Vec3 diff = Vec3();
    alignas(16) Vec3 spec = Vec3();
    alignas(4) float shininess;
};

void Element::Material::load() {

    MatData ubo{};

    ubo.ambient = data.ambient;
    ubo.diff = data.diffuse;
    ubo.spec = data.specular;
    ubo.shininess = data.shininess;;
    VkDeviceSize size = sizeof(MatData);

    uniformBuffers.resize(3);

    for (auto& buffer: uniformBuffers)
    {
        buffer.Create(size, 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        buffer.Map();
        buffer.CopyMemory(&ubo, size);
        buffer.Unmap();
    }

}

void Element::Material::destroy() {
    for (auto& buffer: uniformBuffers)
    {
        buffer.Destroy();
    }
}
