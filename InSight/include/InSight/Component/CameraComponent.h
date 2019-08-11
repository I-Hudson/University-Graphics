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

		void setPosition(const glm::vec3& a_position);
		void setRotation(float a_angle);

		//set camera props
		void setCameraPropertiesPre(float aFOV, float aAspect, float aNearPlane, float aFarPlane);
		void setCameraPropertiesOrt(float a_left, float a_right, float a_bottom, float a_top);

		void recalculateViewMatrix();

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
		glm::mat4 mProjectionMatrix;
		glm::mat4 mProjectionViewMatrix;
		glm::mat4 mViewMatrix;
		glm::mat4 mInvProjection;
		glm::mat4 mInvView;

		//camera props
		float mFOV;
		float mAspect;
		glm::vec2 mFrustrumPlanes;
	};
}