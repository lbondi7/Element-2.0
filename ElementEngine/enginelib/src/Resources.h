#pragma once

#include "VknConstants.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

#include <string>
#include <map>
#include <memory>

namespace Element {

	class Resources {
    public:
        enum class State : int{
            STATIC = 0,
            DYNAMIC= 1
        };

        Resources() = default;
        ~Resources();

        Element::Mesh* mesh(const std::string& name, State state = State::STATIC);

        Element::Texture *texture(const std::string &name, State state = State::STATIC);

        Element::Shader *shader(const std::string &name, ShaderType type);

        Element::Material *material(const std::string &name, State state = State::STATIC);

        void unbindMeshes();

        void init();

        void destroy();


	private:

        std::map<std::string, std::unique_ptr<Shader>> vertex_shaders;
        std::map<std::string, std::unique_ptr<Shader>> fragment_shaders;
        std::map<std::string, std::unique_ptr<Shader>> geometry_shaders;

        std::map<std::string, Mesh> static_meshes;
        //std::map<std::string, Mesh*> static_meshes;
        std::map<std::string, Mesh> dynamic_meshes;

        std::map<std::string, std::unique_ptr<Texture>> static_textures;
        std::map<std::string, std::unique_ptr<Texture>> dynamic_textures;

        std::map<std::string, std::unique_ptr<Material>> static_materials;
        std::map<std::string, std::unique_ptr<Material>> dynamic_materials;


        void LoadTextureData(const std::string& file, TextureData& textureData);
        void LoadTextureData(const char* file, TextureData& textureData);
//        void LoadMeshData(const std::string& file, MeshData& meshData);
//        void LoadMeshData(const char* file, MeshData& meshData);

        void LoadMaterialData(const char* file, MaterialData& materialData);

        Shader* getShader(const std::string& name, Element::ShaderType type,
                          std::map<std::string, std::unique_ptr<Shader>>& shader_map);

	};
}