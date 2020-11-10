#pragma once

#include "Shader.h"
#include <map>
#include <vector>
#include <memory>
namespace Element {
	class ShaderManager
	{
	public:
		ShaderManager();
		~ShaderManager();


		Shader* GetShader(ShaderType type, std::string name);

		void Destroy();

	private:

		std::string GetFileExtension(ShaderType type);

		Shader* CheckShaderCache(std::map<std::string, std::unique_ptr<Shader>>& shaders, ShaderType type, std::string& name);

		std::map<std::string, std::unique_ptr<Shader>> m_vertexShaders;
		std::map<std::string, std::unique_ptr<Shader>> m_fragmentShaders;
	};
}
