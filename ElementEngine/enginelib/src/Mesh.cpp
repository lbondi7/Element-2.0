#include "Mesh.h"

void Element::Mesh::LoadMesh(std::string mesh)
{
	Resources::LoadMeshData(mesh, data);
    hasIndices = true;
	createVertexBuffer();
	createIndexBuffer();
}

void Element::Mesh::generateFromVertices(const std::vector<Vertex>& vertices)
{
    data.vertices = vertices;
    hasIndices = false;
    createVertexBuffer();
}

void Element::Mesh::generateFromVertices(Vertex* vertices, int verticesCount)
{
    data.vertices.resize(verticesCount);

    for (size_t i = 0; i < verticesCount; i++)
    {
        data.vertices[i] = vertices[i];
    }
    hasIndices = false;
    createVertexBuffer();
}

void Element::Mesh::createVertexBuffer()
{
	VkDeviceSize bufferSize = sizeof(data.vertices[0]) * data.vertices.size();

	Buffer stagingBuffer(bufferSize);
	stagingBuffer.MapCopyMemory(data.vertices.data());

	m_vertexBuffer.Create(bufferSize, 0, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	m_vertexBuffer.CopyFromBuffer(stagingBuffer);
    stagingBuffer.Destroy();

}

void Element::Mesh::createIndexBuffer()
{
	VkDeviceSize bufferSize = sizeof(data.indices[0]) * data.indices.size();

	Buffer stagingBuffer(bufferSize);
	stagingBuffer.MapCopyMemory(data.indices.data());

	m_indexBuffer.Create(bufferSize, 0, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	m_indexBuffer.CopyFromBuffer(stagingBuffer);
	stagingBuffer.Destroy();
}

void Element::Mesh::Destroy()
{
	m_indexBuffer.Destroy();
	m_vertexBuffer.Destroy();
}

void Element::Mesh::bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize* offsets)
{
    vkCmdBindVertexBuffers(vkCommandBuffer, 0, 1, &m_vertexBuffer.m_buffer, offsets);

    if(hasIndices)
        vkCmdBindIndexBuffer(vkCommandBuffer, m_indexBuffer.m_buffer, 0, VK_INDEX_TYPE_UINT32);
}

void Element::Mesh::draw(VkCommandBuffer vkCommandBuffer)
{
    if (hasIndices)
        vkCmdDrawIndexed(vkCommandBuffer, static_cast<uint32_t>(data.indices.size()), 1, 0, 0, 0);
    else
        vkCmdDraw(vkCommandBuffer, static_cast<uint32_t>(data.vertices.size()), 1, 0, 0);
}
