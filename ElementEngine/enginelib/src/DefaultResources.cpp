#include "DefaultResources.h"
#include <element/Utilities.h>


std::unique_ptr<Element::Shader> Element::DefaultResources::vertexShader;
std::unique_ptr<Element::Shader> Element::DefaultResources::fragmentShader;
std::unique_ptr<Element::Texture> Element::DefaultResources::texture;
std::unique_ptr<Element::Mesh> Element::DefaultResources::mesh;


void Element::DefaultResources::init()
{
	texture = std::make_unique<Texture>();
	texture->Load("default.jpg");

	mesh = std::make_unique<Mesh>();
	mesh->LoadMesh("error.obj");
	
	vertexShader = std::make_unique<Shader>(Shader::ShaderType::VERTEX, "shader");
	fragmentShader = std::make_unique<Shader>(Shader::ShaderType::FRAGMENT, "shader");
}

void Element::DefaultResources::deinit()
{
	vertexShader.reset();
	fragmentShader.reset();
	texture->destroy();
	texture.reset();
	mesh->Destroy();
	mesh.reset();
}

Element::Shader* Element::DefaultResources::GetVertexShader()
{
	return vertexShader.get();
}

Element::Shader* Element::DefaultResources::GetFragmentShader()
{
	return fragmentShader.get();
}

Element::Texture* Element::DefaultResources::GetTexture()
{
	return texture.get();
}

Element::Mesh* Element::DefaultResources::GetMesh()
{
	return mesh.get();
}
