#pragma once

#ifndef __TESS_SHADER_H__
#define __TESS_SHADER_H__

#include "Shader.h"

class TessShader : public Shader
{
public:
	//Constructors
	TessShader();
	TessShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
		const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount = 3,
		const char* aInputs[] = nullptr, unsigned int aOutputCount = 1, const char* aOutputs[] = nullptr);
	//Destructor
	~TessShader();

	//Use this shader to render
	void			useShader(bool aClear = false) override;
	//gui
	void			gui() override;

private:
	//Float values for the different detail settings the tessellation can have 
	float mTessHighDetailDistance;
	float mTessHighDetailValue;

	float mTessMediumDetailDistance;
	float mTessMediumDetailValue;

	float mTessLowDetailValue;

	//render this shader in wireframe
	bool mRenderWireFrame;
};

#endif // __TESS_SHADER_H__
