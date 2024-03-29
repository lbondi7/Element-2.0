#pragma once

#include "VknConstants.h"
#include "Buffer.h"
#include "Device.h"

#include <memory>
#include <string>

namespace Element {

    class Mesh {
    public:
        Mesh() = default;

        ~Mesh() = default;

        //MeshData data;
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};
        Buffer m_vertexBuffer;
        Buffer m_indexBuffer;

        bool bound = false;
        bool hasIndices = false;

        void Load();

        void generateFromVertices(const std::vector<Vertex> &vertices);

        void generateFromVertices(Vertex *vertices, int verticesCount);

        void createVertexBuffer();

        void createIndexBuffer();

        void destroy();

        void bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize *offsets);

        void draw(VkCommandBuffer vkCommandBuffer);
    };

}
namespace Element {

    struct MeshData {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        MeshData& operator=(const MeshData& other) = default;

    };

}
