#pragma once

#include "Shader/BaseShader.h"
#include "Entity/Entity.h"
#include "Component/TransformComponent.h"
#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"

class WaterShader : public BaseShader
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
	InSight::Entity* mMainCamera;
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