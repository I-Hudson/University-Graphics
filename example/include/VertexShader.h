#pragma once

#include "Shader.h"

class VertexShader : public Shader
{
private:

public:

	VertexShader();
	VertexShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
		const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount = 3,
		const char* aInputs[] = nullptr, unsigned int aOutputCount = 1, const char* aOutputs[] = nullptr);
	~VertexShader();

	void useShader(bool aClear = false) override;
};