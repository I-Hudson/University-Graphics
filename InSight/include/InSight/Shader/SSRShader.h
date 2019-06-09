#pragma once

#ifndef __SSR_SHADRE_H__
#define __SSR_SHADRE_H__

#include "Shader/BaseShader.h"
#include "GBuffer.h"


class SSRShader : public BaseShader
{
public:
	//Constructors
	SSRShader();
	SSRShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
		const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount = 3,
		const char* aInputs[] = nullptr, unsigned int aOutputCount = 1, const char* aOutputs[] = nullptr);
	//Destructor
	~SSRShader();

	//use this BaseShader
	void useShader(bool aClear) override;
	//destroy override
	void destroy() override;

	//render the fullscreen quad
	void						renderQuad();

	//screen space buffer
	GBuffer* mSSRBuffer;

private:

	//generate the buffers needed to render the quad
	void						generateBuffers();



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

#endif // __SSR_SHADRE_H__
