#include "Platform/Vulkan/VulkanVertexArray.h"

#include <vulkan/vulkan.h>

namespace InSight
{
	static int ShaderDataTypeToVulkanBaseType(ShaderDataType aType)
	{
		switch (aType)
		{
		case InSight::ShaderDataType::Float:	return VK_FORMAT_R32_SFLOAT;
		case InSight::ShaderDataType::Float2:	return VK_FORMAT_R32_SFLOAT;
		case InSight::ShaderDataType::Float3:	return VK_FORMAT_R32_SFLOAT;
		case InSight::ShaderDataType::Float4:	return VK_FORMAT_R32_SFLOAT;
		case InSight::ShaderDataType::Mat3:		return VK_FORMAT_R32_SFLOAT;
		case InSight::ShaderDataType::Mat4:		return VK_FORMAT_R32_SFLOAT;
		case InSight::ShaderDataType::Int:		return VK_FORMAT_R32_SINT;
		case InSight::ShaderDataType::Int2:		return VK_FORMAT_R32_SINT;
		case InSight::ShaderDataType::Int3:		return VK_FORMAT_R32_SINT;
		case InSight::ShaderDataType::Int4:		return VK_FORMAT_R32_SINT;
		case InSight::ShaderDataType::Bool:		return VK_FORMAT_R32_SINT;
		}
		EN_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	VulkanVertexArray::VulkanVertexArray()
	{
	}

	VulkanVertexArray::~VulkanVertexArray()
	{
	}

	void VulkanVertexArray::Bind() const
	{
	}

	void VulkanVertexArray::Unbind() const
	{
	}

	void VulkanVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& aVertexBuffer)
	{
		EN_CORE_ASSERT(aVertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");


	}

	void VulkanVertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& aIndexBuffer)
	{
	}
}