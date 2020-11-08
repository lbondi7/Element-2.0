#include "Resources.h"

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
            (std::hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
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

void Element::Resources::LoadMeshData(const char* file, MeshData& meshData)
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
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = { 1.0f, 1.0f, 1.0f };


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

void Element::Resources::LoadMeshData(const std::string& file, MeshData& meshData)
{
    LoadMeshData(file.c_str(), meshData);
}

void Element::Resources::LoadMaterialData(const char *file, MaterialData& materialData) {

    std::string filename = "resources/materials/";
    filename += file;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    std::map<std::string, int> material_map;

    std::ifstream is(filename);

    if(is.is_open())
        tinyobj::LoadMtl(&material_map, &materials, &is, &warn, &err);

    materialData.ambient = glm::vec3(materials[0].ambient[0], materials[0].ambient[1], materials[0].ambient[2]);
    materialData.diffuse = glm::vec3(materials[0].diffuse[0], materials[0].diffuse[1], materials[0].diffuse[2]);
    materialData.specular = glm::vec3(materials[0].specular[0], materials[0].specular[1], materials[0].specular[2]);

//    tinyobj::MaterialFileReader matFileReader(filename);
//    matFileReader.operator()("plane.mtl", &materials, &material_map, &warn, &err);


}
