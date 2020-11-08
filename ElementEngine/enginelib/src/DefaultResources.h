#pragma once

#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "VknPipeline.h"
#include <memory>

namespace Element {
	class DefaultResources
	{
	public:

		static void init();
		static void deinit();
		static Element::Shader* GetVertexShader();
		static Element::Shader* GetFragmentShader();
		static Element::Texture* GetTexture();
		static Mesh* GetMesh();

	private:
		static std::unique_ptr<Shader> vertexShader;
		static std::unique_ptr<Shader> fragmentShader;
		static std::unique_ptr<Texture> texture;
		static std::unique_ptr<Mesh> mesh;
	};
}
