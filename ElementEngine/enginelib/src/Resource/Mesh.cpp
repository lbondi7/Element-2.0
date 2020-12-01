#include "Mesh.h"
#include <element/Debugger.h>

void Element::Mesh::Load()
{
    hasIndices = true;
	createVertexBuffer();
	createIndexBuffer();
}

void Element::Mesh::generateFromVertices(const std::vector<Vertex>& _vertices)
{
    vertices = _vertices;
    hasIndices = false;
    createVertexBuffer();
}

void Element::Mesh::generateFromVertices(Vertex* _vertices, int verticesCount)
{
    vertices.resize(verticesCount);

    for (size_t i = 0; i < verticesCount; i++)
    {
        vertices[i] = _vertices[i];
    }
    hasIndices = false;
    createVertexBuffer();
}

void Element::Mesh::createVertexBuffer()
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    Debugger::get().log("Vertex Staging Buffer", Colour::BRIGHT_CYAN);
	VknBuffer stagingBuffer(bufferSize);
	stagingBuffer.MapCopyMemory(vertices.data());

    Debugger::get().log("Vertex Buffer", Colour::BRIGHT_CYAN);
	m_vertexBuffer.CreateAlloc(bufferSize, 0, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	m_vertexBuffer.CopyFromBuffer2(stagingBuffer);
    stagingBuffer.Destroy();

}

void Element::Mesh::createIndexBuffer()
{
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    Debugger::get().log("Index Staging Buffer", Colour::BRIGHT_CYAN);
	VknBuffer stagingBuffer(bufferSize);
	stagingBuffer.MapCopyMemory(indices.data());

    Debugger::get().log("Index Buffer", Colour::BRIGHT_CYAN);
	m_indexBuffer.CreateAlloc(bufferSize, 0, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	m_indexBuffer.CopyFromBuffer2(stagingBuffer);
	stagingBuffer.Destroy();
}

void Element::Mesh::destroy()
{
	m_indexBuffer.Destroy2();
	m_vertexBuffer.Destroy2();
}

void Element::Mesh::bind(VkCommandBuffer vkCommandBuffer) {
    vkCmdBindVertexBuffers(vkCommandBuffer, 0, 1, &m_vertexBuffer.m_descriptorInfo.buffer,
                           &m_vertexBuffer.m_offset);

    if (hasIndices) {
        vkCmdBindIndexBuffer(vkCommandBuffer, m_indexBuffer.m_descriptorInfo.buffer,
                             m_indexBuffer.m_offset, VK_INDEX_TYPE_UINT32);
        //vkCmdBindIndexBuffer(vkCommandBuffer, m_indexBuffer.m_buffer, 0, VK_INDEX_TYPE_UINT32);
    }
}

void Element::Mesh::draw(VkCommandBuffer vkCommandBuffer)
{
    if (hasIndices)
        vkCmdDrawIndexed(vkCommandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
    else
        vkCmdDraw(vkCommandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);
}
