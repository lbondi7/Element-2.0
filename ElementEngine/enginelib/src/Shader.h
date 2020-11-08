#pragma once

#include "VknConstants.h"

#include <string>

namespace Element {

	class Shader
	{
	public:

		enum class ShaderType : int {
			VERTEX = 0,
			FRAGMENT = 1,
			GEOMETRY = 2,
		};

		Shader() = default;
		Shader(ShaderType type, std::string file);
		~Shader();

		bool operator==(const Shader& other)
		{
			return m_shaderType == other.m_shaderType &&
				name == other.name;
		}

		Shader& operator=(const Shader& other)
		{
			m_shaderType = other.m_shaderType;
			name = other.name;
			m_module = other.m_module;
			return *this;
		}


		void Load(ShaderType type, std::string file);
		void Destroy();

		void SetShaderType(ShaderType type);
		ShaderType GetShaderType();

		VkShaderModule GetVkShaderModule();
		VkShaderStageFlagBits GetVkShaderStageFlag();

		static VkShaderStageFlagBits GetVkShaderStageFlag(ShaderType type);

		std::string GetFileExtension();
	private:

		ShaderType m_shaderType = ShaderType::VERTEX;

		VkShaderModule m_module;
		std::string name;
	};

}
