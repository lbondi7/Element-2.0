#pragma once

#include "VknConstants.h"

#include <element/ElementConstants.h>

#include <string>

namespace Element {

	class Shader
	{
	public:

		Shader() = default;
		Shader(ShaderType type, std::string file);
		~Shader();

		bool operator==(const Shader& other)
		{
			return m_shaderType == other.m_shaderType &&
				name == other.name &&
				m_module == other.m_module;
		}

		Shader& operator=(const Shader& other)
		{
			this->m_shaderType = other.m_shaderType;
			this->name = other.name;
			this->m_module = other.m_module;
			return *this;
		}


		void Load(ShaderType type, std::string file);
		void destroy();

		void SetShaderType(ShaderType type);
		[[nodiscard]] ShaderType GetShaderType() const;

		[[nodiscard]] VkShaderModule GetVkShaderModule() const;
		[[nodiscard]] VkShaderStageFlagBits GetVkShaderStageFlag() const;

		static VkShaderStageFlagBits GetVkShaderStageFlag(ShaderType type);

		std::string GetFileExtension();
	private:

		ShaderType m_shaderType = ShaderType::VERTEX;

		VkShaderModule m_module;
		std::string name;
	};

}
