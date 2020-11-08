#include "ShaderManager.h"
#include "Device.h"

Element::ShaderManager::ShaderManager()
{
	GetShader(Shader::ShaderType::VERTEX, "shader");
	GetShader(Shader::ShaderType::FRAGMENT, "shader");
}

Element::ShaderManager::~ShaderManager()
{
	//m_vertexShaders.clear();
	//m_fragmentShaders.clear();
}



Element::Shader* Element::ShaderManager::GetShader(Shader::ShaderType type, std::string name)
{
	switch (type)
	{
	case Shader::ShaderType::VERTEX: {

		return CheckShaderCache(m_vertexShaders, type, name);
	}
	case Shader::ShaderType::FRAGMENT: {

	
		return CheckShaderCache(m_fragmentShaders, type, name);
	}
	case Shader::ShaderType::GEOMETRY: {


		break;
	}
	}

	return nullptr;
}

void Element::ShaderManager::Destroy()
{
	m_vertexShaders.clear();
	m_fragmentShaders.clear();
}

std::string Element::ShaderManager::GetFileExtension(Shader::ShaderType type)
{
	std::string extension = "";
	switch (type)
	{
	case Element::Shader::ShaderType::VERTEX:
		extension += ".vert";
		break;
	case Element::Shader::ShaderType::FRAGMENT:
		extension += ".frag";
		break;
	case Element::Shader::ShaderType::GEOMETRY:
		extension += ".geom";
		break;
	}
	extension += ".spv";
	return extension;
}

Element::Shader* Element::ShaderManager::CheckShaderCache(std::map<std::string, std::unique_ptr<Element::Shader>>& shaders, Element::Shader::ShaderType type, std::string& name) {
	for (auto& shader : shaders)
	{
		if (shader.first == name)
			return shaders[name].get();
	}

	auto newShader = std::make_unique<Shader>(type, name);
	shaders[name] = std::move(newShader);
	return shaders[name].get();
}