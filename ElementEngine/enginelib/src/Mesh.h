#pragma once

#include "VknConstants.h"
#include "Buffer.h"
#include "Device.h"
#include "Resources.h"

#include <map>
#include <memory>
#include <string>

namespace Element {

	struct Mesh {

		MeshData data;
		Buffer m_vertexBuffer;
		Buffer m_indexBuffer;

		bool bound = false;
		bool hasIndices = false;

		void LoadMesh(std::string mesh);
		void generateFromVertices(const std::vector<Vertex>& vertices);
		void generateFromVertices(Vertex* vertices, int verticesCount);
		void createVertexBuffer();
		void createIndexBuffer();
		void Destroy();

		void bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize* offsets);
		void draw(VkCommandBuffer vkCommandBuffer);
	};

}
