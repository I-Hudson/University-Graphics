#pragma once

#include "Entity/ECS.h"
#include "Component/TransformComponent.h"

namespace InSight
{
	class CameraComponentData : public ComponentData
	{
	public:

	};

	class CameraComponent : public Component
	{
	public:

		//override functions
		void init() override;
		void update() override;
		void gui() override;

		//set camera matrix
		void setCameraMatrix(glm::mat4& aCameraMatrix);
		//set projection matrix
		void setProjectionMatrix(glm::mat4 aProjectionMatrx);

		//set camera props
		void setCameraPropertiesPre(float aFOV, float aAspect, float aNearPlane, float aFarPlane);
		void setCameraPropertiesOrt(float aFOV, float aWidth, float aHeight, float aNearPlane, float aFarPlane);

		//invert pitch
		void invertPitch();

		//get camera matrix
		glm::mat4* getCameraMatrix();
		//get projection matrix
		glm::mat4* getProjectionMatrix();
		//get projection view matrix
		glm::mat4* getProjectionViewMatrix();
		//get view matrix
		glm::mat4* getViewMatrix();

		glm::mat4* getInvprojectionMatrix();
		glm::mat4* getInvViewMatrix();

		//print matrix
		void printCameraMatrix();

		virtual int Save() override;

	private:
		//camera Matrix (pos, rot)
		TransformComponent* mTransoform;
		//camera matrixs
		glm::mat4 mPorjectionMatrix;
		glm::mat4 mPorjectionViewMatrix;
		glm::mat4 mViewMatrix;
		glm::mat4 mInvProjection;
		glm::mat4 mInvView;

		//camera props
		float mFOV;
		float mAspect;
		glm::vec2 mFrustrumPlanes;
	};
}