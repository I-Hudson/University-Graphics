#pragma once

#ifndef __SSAO_H__
#define __SSAO_H__

#include "Shader/Shader.h"
#include "GBuffer.h"

class SSAOShader : public Shader
{
public:
	//Constructors
	SSAOShader();
	SSAOShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
		const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount = 3,
		const char* aInputs[] = nullptr, unsigned int aOutputCount = 1, const char* aOutputs[] = nullptr);
	//Destructor
	~SSAOShader();

	//override
	void destroy() override;
	void useShader(bool aClear) override;

	//render fullscreen quad
	void renderQuad();
	//generate fullscreen quad
	void generateQuad();

private:

	//buffer for ssao pass
	GBuffer mViewNormalBuffer;

	//blur shader
	Shader* mViewNormalShader;

	//buffer for ssao pass
	GBuffer mSSAOBuffer;

	//blur shader
	Shader* mBlurShader;

	//kernal array
	std::vector<glm::vec3> mKernel;

	//noise texture
	unsigned int mNoiseTexture;

	//sample radius
	float mSampleRad;

	//Buffers for the full screen quad
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mIBO;

	//struct for the full screen quad
	struct FSVertex
	{
		glm::vec4 mPostion;
		glm::vec2 mTexCoord1;

		FSVertex() {}
		FSVertex(const glm::vec4& aPosition, const glm::vec2& aTexCoord1)
		{
			mPostion = aPosition;
			mTexCoord1 = aTexCoord1;
		}
	};

};

#endif // __SSAO_H__
