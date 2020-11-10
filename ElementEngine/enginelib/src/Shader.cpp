#include "Shader.h"

#include "VkInitializers.h"
#include "VkFunctions.h"
#include "Device.h"
#include "Utilities.h"

#include <stdexcept>
#include <algorithm>

Element::Shader::Shader(ShaderType type, std::string file) : m_shaderType(type)
{
    Load(type, file);
}

Element::Shader::~Shader()
{
    //destroy();
}

void Element::Shader::Load(ShaderType type, std::string file)
{
    m_shaderType = type;
    name = Utilities::extractName(file);
    auto shaderCode = Element::VkFunctions::readFile("resources/shaders/" + file + GetFileExtension());
    VkShaderModuleCreateInfo createInfo = Element::VkInitializers::shaderModuleCreateInfo(shaderCode);

    if (vkCreateShaderModule(Device::getVkDevice(), &createInfo, nullptr, &m_module) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
}

void Element::Shader::destroy() {

    if (m_module)
    {
        vkDestroyShaderModule(Device::getVkDevice(), m_module, nullptr);
    }
}

void Element::Shader::SetShaderType(ShaderType type)
{
    m_shaderType = type;
}

Element::ShaderType Element::Shader::GetShaderType() const
{
    return m_shaderType;
}

VkShaderModule Element::Shader::GetVkShaderModule() const
{
    return m_module;
}

VkShaderStageFlagBits Element::Shader::GetVkShaderStageFlag() const
{
    switch (m_shaderType) {

    case ShaderType::VERTEX:
    {
        return VK_SHADER_STAGE_VERTEX_BIT;
    }
    case ShaderType::FRAGMENT:
    {
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    case ShaderType::GEOMETRY:
    {
        return VK_SHADER_STAGE_GEOMETRY_BIT;
    }
    }
    return VK_SHADER_STAGE_VERTEX_BIT;
}

VkShaderStageFlagBits Element::Shader::GetVkShaderStageFlag(ShaderType type)
{
    switch (type) {

    case ShaderType::VERTEX:
    {
        return VK_SHADER_STAGE_VERTEX_BIT;
    }
    case ShaderType::FRAGMENT:
    {
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    case ShaderType::GEOMETRY:
    {
        return VK_SHADER_STAGE_GEOMETRY_BIT;
    }
    }

    return VK_SHADER_STAGE_VERTEX_BIT;
}

std::string Element::Shader::GetFileExtension()
{
    std::string extension = "";
    switch (m_shaderType)
    {
    case Element::ShaderType::VERTEX:
        extension += ".vert";
        break;
    case Element::ShaderType::FRAGMENT:
        extension += ".frag";
        break;
    case Element::ShaderType::GEOMETRY:
        extension += ".geom";
        break;
    }
    extension += ".spv";
    return extension;
}

