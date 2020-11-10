#include "ShaderManager.h"
#include "Device.h"

Element::ShaderManager::ShaderManager()
{
	GetShader(ShaderType::VERTEX, "shader");
	GetShader(ShaderType::FRAGMENT, "shader");
}

Element::ShaderManager::~ShaderManager()
{
	//m_vertexShaders.clear();
	//m_fragmentShaders.clear();
}



Element::Shader* Element::ShaderManager::GetShader(ShaderType type, std::string name)
{
	switch (type)
	{
	case ShaderType::VERTEX: {

		return CheckShaderCache(m_vertexShaders, type, name);
	}
	case ShaderType::FRAGMENT: {

	
		return CheckShaderCache(m_fragmentShaders, type, name);
	}
	case ShaderType::GEOMETRY: {


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

std::string Element::ShaderManager::GetFileExtension(ShaderType type)
{
	std::string extension = "";
	switch (type)
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

Element::Shader* Element::ShaderManager::CheckShaderCache(std::map<std::string, std::unique_ptr<Element::Shader>>& shaders, Element::ShaderType type, std::string& name) {
	for (auto& shader : shaders)
	{
		if (shader.first == name)
			return shaders[name].get();
	}

	auto newShader = std::make_unique<Shader>(type, name);
	shaders[name] = std::move(newShader);
	return shaders[name].get();
}