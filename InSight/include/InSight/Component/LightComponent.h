#pragma once

#ifndef __LIGHT_COMPONENT_H__
#define __LIGHT_COMPONENT_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Log.h"

#include "Entity/ECS.h"

class BaseShader;
class TransformComponent;

//type of light
enum LightType
{
	Point,
	Spot,
	Directional,
	Ambient
};

class LightComponent : public Component
{
public:

	//Constructor
	LightComponent();
	//Destructor
	~LightComponent();

	void					init() override;
	//Draw the light volume
	void					draw(BaseShader* aShader = nullptr, const bool& aBindTextures = true) override;

	//Draw all gui 
	void					gui()override;

	//Set the position of this light
	void					setPosition(const glm::vec4 aPosition);
	//Set the diffuse colour this light emits
	void					setDiffuse(const glm::vec4& aDiffuse);
	//Set the specular colour this light emits
	void					setSpecular(const glm::vec4& aSpecular);
	//Set the intensity of this light
	void					setIntensity(const float& aIntensity);

	//Get a pointer to the position data
	glm::vec4*				getPosition();
	//Get a pointer to the diffuse data
	glm::vec4*				getDiffuse();
	//Get a pointer to the specular data
	glm::vec4*				getSpecular();
	//Get a pointer to the intensity data
	float*					getIntensity();

	//get target position
	glm::vec4*				getTargetPosition();
	//get light direction
	glm::vec4*				getLightDirection();

	//add required buffers/textuer for a shadow map
	void					addShadowMap();
	//get the shadowmap/texture
	unsigned int*			getShadowMap();
	//bind the framebuffer for to check for shadows
	void					bindShadowFB();
	//unbind  the framebuffer for shadow
	void					unbindShadowFB();

	//get a pointer the the shadow projection view matrix
	glm::mat4*				getShadowProViewMatrix();
	//set the shadow projection view matrix
	void					setShadowProViewMatrix(const glm::mat4& aMat4);
	//bind the shadow projection view matrix
	void					bindShadowProViewMatrix(unsigned int& aProgram);

	//check if shadows have been added to this ligth
	bool&					shadowsAdded();

	//Set this light's type
	LightType				getLightType();

	//Check if this light's light volume has been generated
	const bool				meshGenerated()const;

	//reset tri count
	void					clearMesh();

	//call after the light pass
	virtual void			PostLight(BaseShader* aShader);

protected:

	//Add a triangle to the light volume's mesh
	void					addTri(glm::vec4 aP0, glm::vec4 aP1, glm::vec4 aP2);

	//Set this light's type
	void					setLightType(LightType aType);

	//Return all the vertices for this light volume
	std::vector<glm::vec4>* getVertices();

	//light direction
	glm::vec4 mLightDir;

	//light target position (where the light is pointting
	//defualt stright
	glm::vec4 mTargetPosition;

private:
	//Generate all the buffers needed for this
	//light volume
	void					generateBuffers();

	LightType mLightType;

	struct LightTri
	{
		glm::vec4 P0;
		glm::vec4 P1;
		glm::vec4 P2;
	};

	unsigned int mLightTriCount;
	LightTri* mLightTri;

	const unsigned int mBUFFER_SIZE = 32728;

	//Pointer to Transform Component
	TransformComponent* mTransform;
	//Diffuse colour this light emits
	glm::vec4 mDiffuse;
	//specluar colour
	glm::vec4 mSpecular;
	//light intensity
	float mIntensity;

	//All the vertices for this light volume
	std::vector<glm::vec4> mVertices;
	//All the indices for this light volume
	std::vector<unsigned int> mIndices;

	//Buffers for the light volumes
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mIBO;

	//buffers/textures for shadows
	bool mShadowsAdded;
	unsigned int mShadowFB;
	unsigned int mShadowTexture;
	glm::mat4 mShadowProViewMatrix;
};

#endif // __LIGHT_COMPONENT_H__