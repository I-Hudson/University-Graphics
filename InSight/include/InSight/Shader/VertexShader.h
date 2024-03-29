#pragma once

#include "Shader/BaseShader.h"

class VertexShader : public BaseShader
{
private:

public:
	//Constructors
	VertexShader();
	VertexShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
		const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount = 3,
		const char* aInputs[] = nullptr, unsigned int aOutputCount = 1, const char* aOutputs[] = nullptr);
	//Destructor
	~VertexShader();

	//override
	void useShader(bool aClear = false) override;
};