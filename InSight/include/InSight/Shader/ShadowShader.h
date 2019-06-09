#pragma once

#ifndef __SHADOW_SHADER_H__
#define __SHADOW_SHADER_H__

#include <vector>
#include "Shader/BaseShader.h"
#include "GBuffer.h"

class LightComponent;

class ShadowShader : public BaseShader
{
public:
	//Constructors
	ShadowShader();
	ShadowShader(const char* aVertexPath, const char* aFragmentPath);
	ShadowShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
		const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount = 3,
		const char* aInputs[] = nullptr, unsigned int aOutputCount = 1, const char* aOutputs[] = nullptr);
	//Destructor
	~ShadowShader();

	//overrides
	void destroy() override;
	void useShader(bool aClear = false) override;

private:
};

#endif // __SHADOW_SHADER_H__