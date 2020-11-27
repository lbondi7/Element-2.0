#include "Resources.h"

#include <Misc/Utilities.h>

#include <element/Debugger.h>

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

bool Element::Resources::LoadTextureData(const char* file, TextureData& textureData)
{
    std::string filename = "resources/textures/";
    filename += file;
    textureData.data = stbi_load(filename.c_str(), &textureData.width, &textureData.height, &textureData.channels, STBI_rgb_alpha);

    if (!textureData.data) {

        Debugger::get().error("failed to load texture image!");
        textureData.data = nullptr;
        return false;
        throw std::runtime_error("failed to load texture image!");
    }
    return true;
}

bool Element::Resources::LoadMeshData(const char* file, Element::MeshData& meshData)
{
    std::string filename = "resources/models/";
    filename += file;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str())) {

        Debugger::get().error(warn + err);
        return false;
        //throw std::runtime_error(warn + err);
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
    return true;
}

bool Element::Resources::LoadTextureData(const std::string& file, TextureData& textureData)
{
    return LoadTextureData(file.c_str(), textureData);
}

//void Element::Resources::LoadMeshData(const std::string& file, MeshData& meshData)
//{
//    LoadMeshData(file.c_str(), meshData);
//}

bool Element::Resources::LoadMaterialData(const char *file, MaterialData& materialData) {

    std::string filename = "resources/materials/";
    filename += file;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    std::map<std::string, int> material_map;

    std::ifstream is(filename);

    if(!is.is_open())
    {
        Debugger::get().error("failed to load material!");
        return false;
    }
    tinyobj::LoadMtl(&material_map, &materials, &is, &warn, &err);


    materialData.ambient = Vec3(materials[0].ambient);
    materialData.diffuse = Vec3(materials[0].diffuse);
    materialData.specular = Vec3(materials[0].specular);
    materialData.emission = Vec3(materials[0].emission);
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

    return true;

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
    if(!LoadMeshData(name.c_str(), data)){
        if(state == State::STATIC)
            static_meshes.erase(tmp_name);
        if(state == State::DYNAMIC)
            dynamic_meshes.erase(tmp_name);

        return &static_meshes["s_error"];
    }
    mesh.vertices = data.vertices;
    mesh.indices = data.indices;

    Debugger::get().log(name, Colour::BRIGHT_GREEN);
    mesh.Load();
    return &mesh;
}

Element::Texture *Element::Resources::texture(const std::string &name, State state) {

    auto tmp_name = (state == State::STATIC ? "s_" : "d_") + Utilities::extractName(name);

    auto &texture = state == State::STATIC ? static_textures[tmp_name] : dynamic_textures[tmp_name];
    if (texture)
        return texture.get();

    texture = std::make_unique<Texture>();
    if(!LoadTextureData(name, texture->data)) {
        if(state == State::STATIC)
            static_textures.erase(tmp_name);
        if(state == State::DYNAMIC)
            dynamic_textures.erase(tmp_name);

        return static_textures["s_default"].get();
    }

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
    std::vector<std::string> names{ "cube.obj", "quad.obj", "error.obj" };
    for (const auto& name : names)
        mesh(name);

    names = { "default.jpg", "texture.jpg" };
    for (const auto& name : names)
        texture(name);

    names = { "test.mtl"};
    for (const auto& name : names)
        material(name);
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

    for (auto& material: static_materials)
        material.second->destroy();

    for (auto& material: dynamic_materials)
        material.second->destroy();

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
    if(!LoadMaterialData(name.c_str(), material->data)){
        if(state == State::STATIC)
            static_materials.erase(tmp_name);
        if(state == State::DYNAMIC)
            dynamic_materials.erase(tmp_name);

        return static_materials["s_test"].get();
    }
    material->load();
    return material.get();
}

