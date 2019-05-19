#pragma once

#ifndef __CAMERA_COMPONENT_H__
#define __CAMERA_COMPONENT_H__

#include "ECS.h"
#include "TransformComponent.h"

class CameraComponent : public Component
{
public:

	void init() override;

	void update() override;
	void gui() override;

	//set camera matrix
	void setCameraMatrix(glm::mat4& aCameraMatrix);
	//set projection matrix
	void setProjectionMatrix(glm::mat4 aProjectionMatrx);

	void setCameraPropertiesPre(float aFOV, float aAspect, float aNearPlane, float aFarPlane);
	void setCameraPropertiesOrt(float aFOV, float aWidth, float aHeight, float aNearPlane, float aFarPlane);

	void invertPitch();

	//get camera matrix
	glm::mat4* getCameraMatrix();
	//get projection matrix
	glm::mat4* getProjectionMatrix();
	//get projection view matrix
	glm::mat4* getProjectionViewMatrix();
	//get view matrix
	glm::mat4 getViewMatrix();

	//print matrix
	void printCameraMatrix();

private:
	//camera Matrix (pos, rot)
	TransformComponent* mTransoform;
	//projection Matrix
	glm::mat4 mPorjectionMatrix;
	glm::mat4 mPorjectionViewMatrix;

	float mFOV;
	float mAspect;
	glm::vec2 mFrustrumPlanes;
};
#endif // __CAMERA_COMPONENT_H__