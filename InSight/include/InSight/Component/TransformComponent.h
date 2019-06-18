#pragma once

#include "Entity/ECS.h"
#include <glm/ext.hpp>

namespace InSight
{
	class TransformData : public ComponentData
	{
	public:
		float Matrix[16];
	};

	class TransformComponent : public Component
	{
	public:
		//overrides
		void init() override;
		void gui() override;

		virtual int Save() override;

		//transofmr matrix functions
		void setTransformMatrix(glm::mat4& aMatrix);
		void setPosition(glm::vec4 aPosition);
		void setScale(const glm::vec3& aScale);
		void setScale(const float& aScale);

		//invert pitch
		void invertPitch();

		//get transform matrix and position
		glm::mat4* getTransformMatrix();
		glm::vec4* getPosition();

		//has this component been updated
		bool& getHasChanged();
	private:
		//transform matrix
		glm::mat4 mTransformMatrix;
		//position
		glm::vec4 mPosition;
		//component updated
		bool mHasChanged;
	};
}