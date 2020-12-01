#pragma once

#include <Vulkan/VknConstants.h>
#include <Vulkan/VknBuffer.h>

#include <memory>
#include <string>

namespace Element {

    struct MeshData {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        MeshData &operator=(const MeshData &other) = default;

    };

    class Mesh {
    public:
        Mesh() = default;

        ~Mesh() = default;

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        VknBuffer m_vertexBuffer;
        VknBuffer m_indexBuffer;

        glm::vec3 min = glm::vec3(FLT_MAX);
        glm::vec3 max = glm::vec3(-FLT_MAX);

        bool bound = false;
        bool hasIndices = false;

        void Load();

        void generateFromVertices(const std::vector<Vertex> &vertices);

        void generateFromVertices(Vertex *vertices, int verticesCount);

        void createVertexBuffer();

        void createIndexBuffer();

        void destroy();

        void bind(VkCommandBuffer vkCommandBuffer);

        void draw(VkCommandBuffer vkCommandBuffer);
    };
}
