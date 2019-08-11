#include "Platform\Vulkan\VulkanBuffer.h"

#include <vulkan/vulkan.h>

namespace InSight
{
	/////////////////////////////////////////////////////////////////////////
	// VERTEX BUFFER
	/////////////////////////////////////////////////////////////////////////
	VulkanVertexBuffer::VulkanVertexBuffer(float* aVertices, uint32_t aSize)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = sizeof(aVertices);
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		//if(vkCreateBuffer())
	}
	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
	}
	void VulkanVertexBuffer::Bind() const
	{
	}
	void VulkanVertexBuffer::Unbind() const
	{
	}

	/////////////////////////////////////////////////////////////////////////
	// INDEX BUFFER
	/////////////////////////////////////////////////////////////////////////
	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* indices, uint32_t count)
		:mCount(count)
	{
	}
	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
	}
	void VulkanIndexBuffer::Bind() const
	{
	}
	void VulkanIndexBuffer::Unbind() const
	{
	}
}