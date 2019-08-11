#include "InSight/Renderer/Renderer.h"

namespace InSight
{
	Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData();

	void Renderer::BegineScene(CameraComponent& a_camera)
	{
		m_sceneData->ProjectionViewMatrix = *a_camera.getProjectionViewMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader> a_shader, const std::shared_ptr<VertexArray>& aVertexArrray)
	{
		a_shader->Bind();
		a_shader->UploadUniformMat4("u_ProjectionView", m_sceneData->ProjectionViewMatrix);

		aVertexArrray->Bind();
		RenderCommand::DrawIndexed(aVertexArrray);
	}
}