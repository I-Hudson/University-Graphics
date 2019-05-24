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
	//Constructors
	WaterShader();
	WaterShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
		const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount = 3,
		const char* aInputs[] = nullptr, unsigned int aOutputCount = 1, const char* aOutputs[] = nullptr);
	//Destructor
	~WaterShader();

	//init buffers
	void initBuffers();

	//override
	void	destroy() override;
	void useShader(bool aClear = false) override;

	//get framebuffer
	unsigned int* getFrameBuffer() { return &mFrameBuffer; }

	//reflection buffer
	GBuffer* mReflectionBuffer;
	//refraction buffer
	GBuffer* mRefractionBuffer;

	//main camera pointer
	Entity* mMainCamera;
private:
	//clip plane program
	unsigned int mClipPlaneProgram;

	//dudvmap
	unsigned int mDUDVMap;
	//normal map
	unsigned int mNormalMap;
	//anim move factor
	float mMoveFactor;

	//clip plane
	glm::vec4 mClipPlane;
	//wave data
	glm::vec4* waveData;
};

#endif //__WATER_SHADER_H__
