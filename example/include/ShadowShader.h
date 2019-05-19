#pragma once

#ifndef __SHADOW_SHADER_H__
#define __SHADOW_SHADER_H__

#include <vector>
#include "Shader.h"

class LightComponent;

class ShadowShader : public Shader
{
public:
	ShadowShader();
	ShadowShader(const char* aVertexPath, const char* aFragmentPath);
	ShadowShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
		const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount = 3,
		const char* aInputs[] = nullptr, unsigned int aOutputCount = 1, const char* aOutputs[] = nullptr);
	~ShadowShader();

	void useShader(bool aClear = false) override;

	void addShadow(LightComponent* aLight);

private:
	unsigned int mFSShadowShader;
	std::vector<LightComponent*> mShadowLights;

	glm::mat4 testMatrix;
};

#endif // __SHADOW_SHADER_H__
