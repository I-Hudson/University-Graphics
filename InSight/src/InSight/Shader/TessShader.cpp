#include "Shader/TessShader.h"
#include "Renderer.h"

TessShader::TessShader()
{
}

TessShader::TessShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath, 
	const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount, 
	const char* aInputs[], unsigned int aOutputCount, const char* aOutputs[]) : 
	Shader(aVertexPath, aControlpath, aEvaluationpath, aGeometryPath, aFragmentPath, aInputCount, aInputs, aOutputCount, aOutputs)
{
	//set uniforms 
	setFloat("TessHighDetailDist", mTessHighDetailDistance);
	setFloat("TessHighDetailValue", mTessHighDetailValue);

	setFloat("TessMediumDetailDist", mTessMediumDetailDistance);
	setFloat("TessMediumDetailValue", mTessMediumDetailValue);

	setFloat("TesslowDetailValue", mTessLowDetailValue);

	mTessHighDetailDistance = 3.0f;
	mTessHighDetailValue = 10.0f;

	mTessMediumDetailDistance = 10.0f;
	mTessMediumDetailValue = 5.0f;

	mTessLowDetailValue = 1.0f;
}

TessShader::~TessShader()
{
}

void TessShader::useShader(bool aClear)
{
	//if mRenderWireFrame is true 
	//then set the polygon mode
	if (mRenderWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	//bind and set draw buffers if there is a GBuffer attached
	if (mGBuffer != nullptr)
	{
		setVec2("screenSize", *mGBuffer->getScreenSize());
		mGBuffer->bindBuffer();
		mGBuffer->setViewPort();
		mGBuffer->setDrawBuffers();
	}

	//enable depth writing and testing, disable blending
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	//draw objects
	Shader::useShader(aClear, GL_PATCHES);

	//disable depth writing and testing
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	//unbind GBuffer if was have one
	if (mGBuffer != nullptr)
	{
		mGBuffer->unBindBuffer();
		glViewport(0, 0, getRenderer()->getScreenSize().x, getRenderer()->getScreenSize().y);
	}

	//reset the polygon mode 
	if (mRenderWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void TessShader::gui()
{
	ImGui::SliderFloat("Tessilation High Detail Distance", &mTessHighDetailDistance, 1.0f, 10.0f);
	ImGui::SliderFloat("Tessilation High Detail Value", &mTessHighDetailValue, 10.0f, 15.0f);

	ImGui::SliderFloat("Tessilation Medium Detail Distance", &mTessMediumDetailDistance, mTessHighDetailDistance, 20.0f);
	ImGui::SliderFloat("Tessilation Medium Detail Value", &mTessMediumDetailValue, 5.0f, 10.0f);

	ImGui::SliderFloat("Tessilation", &mTessLowDetailValue, 1.0f, 3.0f);

	ImGui::Checkbox("WireFrame", &mRenderWireFrame);
}
