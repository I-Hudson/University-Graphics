#pragma once

#include "Shader/BaseShader.h"
#include "GBuffer.h"

class LightComponent;

class ShadowMapShader : public BaseShader
{
public:
	//Constructor
	ShadowMapShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
		const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount = 3,
		const char* aInputs[] = nullptr, unsigned int aOutputCount = 1, const char* aOutputs[] = nullptr);
	//Destructor
	~ShadowMapShader();

	//init buffers
	void initBuffers();
	//destroy, overrides
	void			destroy() override;
	void useShader(bool aClear = false) override;

	//get shadow map texture
	unsigned int* getShadowTexture();

private:
	//shaodw map buffer
	GBuffer* mShadowMapBuffer;
};