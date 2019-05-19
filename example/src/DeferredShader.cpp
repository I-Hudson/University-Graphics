#include "DeferredShader.h"
#include "Gizmos.h"


DeferredShader::DeferredShader()
{
}

DeferredShader::DeferredShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath, 
							   const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount /*= 3*/, 
							   const char* aInputs[] /*= nullptr*/, unsigned int aOutputCount /*= 1*/, const char* aOutputs[] /*= nullptr*/) : 
	Shader(aVertexPath, aControlpath, aEvaluationpath, aGeometryPath, aFragmentPath, aInputCount, aInputs, aOutputCount, aOutputs)
{
}


DeferredShader::~DeferredShader()
{
}

void DeferredShader::useShader(bool aClear)
{	
	//if mRenderWireFrame is true 
	//then set the polygon mode
	if (mRenderWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	mGBuffer->bindBuffer();

	mGBuffer->setDrawBuffers();
	glViewport(0, 0, 1920, 1080);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	Shader::useShader(aClear);
	Gizmos::draw(*mMat4Uniforms["ProjectionView"]);
	
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, 1920, 1080);
	mGBuffer->unBindBuffer();

	//reset the polygon mode 
	if (mRenderWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void DeferredShader::gui()
{
	for (auto& tex : *getGBuffer()->getAllTextures())
	{
		ImGui::Text(tex.mName);
		ImTextureID texID = (void*)(intptr_t)tex.mTexture;
		ImGui::Image(texID, ImVec2(256, 128), ImVec2(0, 1), ImVec2(1, 0));
	}

	ImGui::Checkbox("WireFrame", &mRenderWireFrame);
}

//void DeferredShader::setStencilShader(VertexShader * aStencilShader)
//{
//	mStencilShader = aStencilShader;
//}
//
//void DeferredShader::setDeferredLightShader(DeferredLightPassShader * aLightShader)
//{
//	mDeferredLightShader = aLightShader;
//}

//VertexShader* DeferredShader::getStencilShader()
//{
//	return mStencilShader;
//}
//
//DeferredLightPassShader* DeferredShader::getLightShader()
//{
//	return mDeferredLightShader;
//}
