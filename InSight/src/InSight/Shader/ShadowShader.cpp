#include "Shader/ShadowShader.h"
#include "BaseRenderer.h"
#include "Entity/EntityManager.h"
#include "Component/DirectionalLight.h"
#include "Component/MeshComponent.h"

#include "InSight/Entity/EntityManager.h"

using namespace InSight;

ShadowShader::ShadowShader()
{
}

ShadowShader::ShadowShader(const char* aVertexPath, const char* aFragmentPath)
{
}

ShadowShader::ShadowShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath, 
	const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount, 
	const char* aInputs[], unsigned int aOutputCount, const char* aOutputs[]) : 
	BaseShader(aVertexPath, aControlpath, aEvaluationpath, aGeometryPath, aFragmentPath, aInputCount, aInputs, aOutputCount, aOutputs)
{
}

ShadowShader::~ShadowShader()
{
}

void ShadowShader::destroy()
{
}

void ShadowShader::useShader(bool aClear)
{
	//get all lights
	std::vector<DirectionalLight*> lights = EntityManager::Get().getAllEntityComponents<DirectionalLight>();
	//get all meshes
	std::vector<MeshComponent*> allMeshes = EntityManager::Get().getAllEntityComponents<MeshComponent>();

	//bind BaseShader
	glUseProgram(mShaderID);

	//loop though all lights and render their depth
	for (auto& light : lights)
	{
		light->bindShadowFB();

		light->bindShadowProViewMatrix(mShaderID);

		setUniforms();
		drawMeshes(allMeshes);

		light->unbindShadowFB();
	}

	glUseProgram(0);
}
