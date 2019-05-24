#pragma once

#ifndef __DEFERRED_SHADER_H__
#define __DEFERRED_SHADER_H__

#include <map>

#include "Shader.h"

class DeferredShader : public Shader
{
public:
	//Constructors
	DeferredShader();
	DeferredShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
		const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount = 3,
		const char* aInputs[] = nullptr, unsigned int aOutputCount = 1, const char* aOutputs[] = nullptr);
	//Destructor
	~DeferredShader();

	//get framebuffer
	unsigned int*	getFrameBuffer() { return &mFrameBuffer; }

	//overrides
	void			useShader(bool aClear = false) override;
	void			gui() override;

private:
	//render this shader in wireframe
	bool mRenderWireFrame;
};
#endif // __DEFERRED_SHADER_H__