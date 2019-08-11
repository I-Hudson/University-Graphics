#pragma once

#include "Renderer/RenderCommand.h"

#include "InSight/Renderer/Shader.h"
#include "InSight/Component/CameraComponent.h"

namespace InSight
{

	class Renderer
	{
	public:
		static void BegineScene(CameraComponent& a_camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader> a_shader, const std::shared_ptr<VertexArray>& aVertexArrray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ProjectionViewMatrix;
		};

		static SceneData* m_sceneData;
	};

}