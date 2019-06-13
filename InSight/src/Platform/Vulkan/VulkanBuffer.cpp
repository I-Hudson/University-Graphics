#include "Platform\Vulkan\VulkanBuffer.h"

#include <vulkan/vulkan.h>

namespace InSight
{
	/////////////////////////////////////////////////////////////////////////
	// VERTEX BUFFER
	/////////////////////////////////////////////////////////////////////////
	VulkanVertexBuffer::VulkanVertexBuffer(float* aVertices, uint32_t aSize)
	{
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