#include "CommandBuffer.h"

#include <stdexcept>
#include <limits>

Element::CommandBuffer::CommandBuffer(Element::CommandPool* commandPool, bool _begin,  VkQueueFlagBits queueType, VkCommandBufferLevel bufferLevel)
: queueType(queueType), m_commandPool(commandPool){

	const auto& logicalDevice = Device::GetLogicalDevice();
	if (queueType == VK_QUEUE_GRAPHICS_BIT)
		queue = logicalDevice->GetGraphicsQueue();
	if (queueType == VK_QUEUE_COMPUTE_BIT)
		queue = logicalDevice->GetComputeQueue();

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = m_commandPool->GetVkCommandPool();
	commandBufferAllocateInfo.level = bufferLevel;
	commandBufferAllocateInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(logicalDevice->GetVkDevice(), &commandBufferAllocateInfo, &m_vkCommandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	if (_begin)
		begin();
}

Element::CommandBuffer::~CommandBuffer()
{
	destroy();
}

void Element::CommandBuffer::destroy()
{
	if(m_vkCommandBuffer != VK_NULL_HANDLE)
		vkFreeCommandBuffers(Device::getVkDevice(), m_commandPool->GetVkCommandPool(), 1, &m_vkCommandBuffer);

	m_vkCommandBuffer = VK_NULL_HANDLE;
}

void Element::CommandBuffer::begin(VkCommandBufferUsageFlags usage) {
	if (m_running)
		return;

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = usage;
	if (vkBeginCommandBuffer(m_vkCommandBuffer, &beginInfo) != VK_SUCCESS) {
	    throw std::runtime_error("failed to begin recording command buffer!");
	}
	m_running = true;
}

void Element::CommandBuffer::end() {
	if (!m_running) return;

	if (vkEndCommandBuffer(m_vkCommandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
	m_running = false;
}

void Element::CommandBuffer::Submit(const VkSemaphore& waitSemaphore, const VkSemaphore& signalSemaphore, VkFence fence)
{

	if (m_running)
		end();

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_vkCommandBuffer;

	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &waitSemaphore;

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &signalSemaphore;

	if(fence != VK_NULL_HANDLE)
		vkResetFences(Device::getVkDevice(), 1, &fence);

	if (vkQueueSubmit(getQueue(), 1, &submitInfo, fence) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}
}

void Element::CommandBuffer::SubmitIdle() {
	const auto& queueSelected = getQueue();
	const auto& logicalDevice = Device::getVkDevice();

	if (m_running)
		end();

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_vkCommandBuffer;

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

	VkFence fence;
	vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, &fence);

	vkResetFences(logicalDevice, 1, &fence);

	vkQueueSubmit(queue, 1, &submitInfo, fence);
	vkQueueWaitIdle(queue);
	vkWaitForFences(logicalDevice, 1, &fence, VK_TRUE, std::numeric_limits<uint64_t>::max());

	vkDestroyFence(logicalDevice, fence, nullptr);
}

VkCommandBuffer Element::CommandBuffer::GetVkCommandBuffer()
{
	return m_vkCommandBuffer;
}

VkQueue Element::CommandBuffer::getQueue()
{
	return queue;
}
