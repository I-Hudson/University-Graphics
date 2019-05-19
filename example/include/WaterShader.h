#pragma once

#ifndef __WATER_SHADER_H__
#define __WATER_SHADER_H__

#include "Shader.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "MeshComponent.h"

class WaterShader : public Shader
{
public:
	WaterShader();
	WaterShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
		const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount = 3,
		const char* aInputs[] = nullptr, unsigned int aOutputCount = 1, const char* aOutputs[] = nullptr);
	~WaterShader();

	unsigned int* getFrameBuffer() { return &mFrameBuffer; }
	void useShader(bool aClear = false) override;

	unsigned int mReflection;
	unsigned int mRefraction;
	unsigned int mDepth;

	GBuffer mReflectionBuffer;
	GBuffer mRefractionBuffer;

	Entity* mMainCamera;
private:
	unsigned int mClipPlaneProgram;

	unsigned int mDUDVMap;
	unsigned int mNormalMap;
	float mMoveFactor;

	glm::vec4 mClipPlane;
	glm::vec4 waveData[3];
};

#endif //__WATER_SHADER_H__
