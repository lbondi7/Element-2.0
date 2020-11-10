#include "Resources.h"

#include "Utilities.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <stdexcept>
#include <unordered_map>
#include <fstream>

template<> struct std::hash<Vertex> {
    size_t operator()(Vertex const& vertex) const {
        return ((std::hash<glm::vec3>()(vertex.pos) ^
            (std::hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
               (std::hash<glm::vec2>()(vertex.texCoord) << 1);
    }
};

void Element::Resources::LoadTextureData(const char* file, TextureData& textureData)
{
    std::string filename = "resources/textures/";
    filename += file;
    textureData.data = stbi_load(filename.c_str(), &textureData.width, &textureData.height, &textureData.channels, STBI_rgb_alpha);

    if (!textureData.data) {
        throw std::runtime_error("failed to load texture image!");
    }
}

void LoadMeshData(const char* file, Element::MeshData& meshData)
{
    std::string filename = "resources/models/";
    filename += file;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str())) {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};
    int i = 0;
    for (const auto& shape : shapes) {
        Vertex vertex{};
        for (const auto& index : shape.mesh.indices) {

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.normal = {attrib.normals[3 * index.normal_index + 0],
                             attrib.normals[3 * index.normal_index + 1],
                             attrib.normals[3 * index.normal_index + 2]
            };

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(meshData.vertices.size());
                meshData.vertices.push_back(vertex);
            }

            meshData.indices.push_back(uniqueVertices[vertex]);
            i++;
        }
    }
}

void Element::Resources::LoadTextureData(const std::string& file, TextureData& textureData)
{
    LoadTextureData(file.c_str(), textureData);
}

//void Element::Resources::LoadMeshData(const std::string& file, MeshData& meshData)
//{
//    LoadMeshData(file.c_str(), meshData);
//}

void Element::Resources::LoadMaterialData(const char *file, MaterialData& materialData) {

    std::string filename = "resources/materials/";
    filename += file;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    std::map<std::string, int> material_map;

    std::ifstream is(filename);

    if(is.is_open())
        tinyobj::LoadMtl(&material_map, &materials, &is, &warn, &err);


    materialData.ambient = Vec3(materials[0].ambient);
    materialData.diffuse = Vec3(materials[0].diffuse);
    materialData.specular = Vec3(materials[0].specular);
    materialData.transmittance = Vec3(materials[0].transmittance);
    materialData.shininess = materials[0].shininess;
    materialData.dissolve = materials[0].dissolve;
    materialData.refractionIndex = materials[0].ior;
    materialData.illum = materials[0].illum;

    /// PBR values;
    materialData.roughness = materials[0].roughness;
    materialData.anisotropy = materials[0].anisotropy;
    materialData.metallic = materials[0].metallic;
    materialData.sheen = materials[0].sheen;

//    tinyobj::MaterialFileReader matFileReader(filename);
//    matFileReader.operator()("plane.mtl", &materials, &material_map, &warn, &err);
}

Element::Mesh *Element::Resources::mesh(const std::string &name, State state) {

    auto tmp_name = (state == State::STATIC ? "s_" : "d_") + Utilities::extractName(name);


    for (auto& m : static_meshes)
    {
        if(tmp_name == m.first)
            return &m.second;
    }
    for (auto& m : dynamic_meshes)
    {
        if(tmp_name == m.first)
            return &m.second;
    }

    auto& mesh = state == State::STATIC ? static_meshes[tmp_name] : dynamic_meshes[tmp_name];

    MeshData data;
    LoadMeshData(name.c_str(), data);
    mesh.vertices = data.vertices;
    mesh.indices = data.indices;
    mesh.Load();
    return &mesh;
}

Element::Texture *Element::Resources::texture(const std::string &name, State state) {

    auto tmp_name = (state == State::STATIC ? "s_" : "d_") + Utilities::extractName(name);

    auto &texture = state == State::STATIC ? static_textures[tmp_name] : dynamic_textures[tmp_name];
    if (texture)
        return texture.get();

    texture = std::make_unique<Texture>();
    LoadTextureData(name, texture->data);
    texture->Load();
    return texture.get();
}

Element::Shader *Element::Resources::shader(const std::string &name, Element::ShaderType type) {

    switch (type) {
        case ShaderType::VERTEX: {
                return getShader(name, type, vertex_shaders);
        }
        case ShaderType::FRAGMENT: {
            return getShader(name, type, fragment_shaders);
        }
        case ShaderType::GEOMETRY: {
            return getShader(name, type, geometry_shaders);
        }
    }

    return nullptr;
}

void Element::Resources::unbindMeshes() {

    for (auto& mesh: static_meshes)
        mesh.second.bound = false;

    for (auto& mesh: dynamic_meshes)
        mesh.second.bound = false;

}

void Element::Resources::init() {
    std::string meshNames[3]{ "cube.obj", "quad.obj", "error.obj" };
    for (const auto& name : meshNames)
        mesh(name);

    std::string texts[2]{ "default.jpg", "texture.jpg" };
    for (const auto& name : texts)
        texture(name);
}

void Element::Resources::destroy() {

    for (auto& mesh: static_meshes)
        mesh.second.destroy();

    for (auto& mesh: dynamic_meshes)
        mesh.second.destroy();

    for (auto& texture: static_textures)
        texture.second->destroy();

    for (auto& texture: dynamic_textures)
        texture.second->destroy();

    for (auto& shader: vertex_shaders)
        shader.second->destroy();

    for (auto& shader: fragment_shaders)
        shader.second->destroy();

    for (auto& shader: geometry_shaders)
        shader.second->destroy();

}

Element::Shader *Element::Resources::getShader(const std::string& name, Element::ShaderType type, std::map<std::string,
                                               std::unique_ptr<Element::Shader>> &shader_map) {

    auto &shader = shader_map[name];
    if (shader)
        return shader.get();

    shader = std::make_unique<Shader>();
    shader->Load(type, name);
    return shader.get();
}

Element::Resources::~Resources() {

}

Element::Material *Element::Resources::material(const std::string &name, Element::Resources::State state) {
    auto tmp_name = (state == State::STATIC ? "s_" : "d_") + Utilities::extractName(name);

    auto &material = state == State::STATIC ? static_materials[tmp_name] : dynamic_materials[tmp_name];
    if (material)
        return material.get();

    material = std::make_unique<Material>();
    LoadMaterialData(name.c_str(), material->data);
    return material.get();
}

