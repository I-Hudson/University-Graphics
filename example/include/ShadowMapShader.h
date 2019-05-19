#pragma once

#ifndef __SHADOW_MAP_SHADER_H__
#define __SHADOW_MAP_SHADER_H__

#include "Shader.h"

class LightComponent;

class ShadowMapShader : public Shader
{
public:
	ShadowMapShader(const char* aVertexPath, const char* aFragmentPath);
	ShadowMapShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
		const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount = 3,
		const char* aInputs[] = nullptr, unsigned int aOutputCount = 1, const char* aOutputs[] = nullptr);
	~ShadowMapShader();

	void useShader(bool aClear = false) override;
	void addShadow(LightComponent* aLight);

private:
	std::vector<LightComponent*> mShadowLights;
	unsigned int tex1;
	unsigned int tex2;
};

#endif // __SHADOW_MAP_SHADER_H__
