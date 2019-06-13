#pragma once

#include "InSight/Renderer/Buffer.h"

namespace InSight
{
	class VulkanVertexBuffer : public VertexBuffer
	{
		VulkanVertexBuffer(float* aVertices, uint32_t aSize);
		virtual ~VulkanVertexBuffer();

		virtual void Bind()  const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const BufferLayout& aLayout) override { mLayout = aLayout; }
		virtual const BufferLayout& GetLayout() const override { return mLayout; }

	private:
		uint32_t mBaseRendererID;
		BufferLayout mLayout;
	};

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~VulkanIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return mCount; }
	private:
		uint32_t mRendererID;
		uint32_t mCount;
	};
}