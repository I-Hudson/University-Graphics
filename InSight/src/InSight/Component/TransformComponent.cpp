#include "Component/TransformComponent.h"

namespace InSight
{
	void TransformComponent::init()
	{
		//set mTransformMatrix and mHasChanged
		mTransformMatrix = glm::mat4();
		mHasChanged = false;
	}

	void TransformComponent::gui()
	{
		if (ImGui::CollapsingHeader("Transofrm Component"))
		{
			mHasChanged = false;

			float tempPosition[3];
			tempPosition[0] = mTransformMatrix[3].x;
			tempPosition[1] = mTransformMatrix[3].y;
			tempPosition[2] = mTransformMatrix[3].z;
			if (ImGui::DragFloat3("Position", tempPosition))
			{
				mTransformMatrix[3].x = tempPosition[0];
				mTransformMatrix[3].y = tempPosition[1];
				mTransformMatrix[3].z = tempPosition[2];
				mHasChanged = true;
			}
		}
	}

	int TransformComponent::Save()
	{	
		TransformData data = {};
		data.Type = ComponentType::TRANSFORM;

		for (int i = 0; i < 4; i++)
		{
			data.Matrix[(4 * i) + 0] = mTransformMatrix[i].x;
			data.Matrix[(4 * i) + 1] = mTransformMatrix[i].y;
			data.Matrix[(4 * i) + 2] = mTransformMatrix[i].z;
			data.Matrix[(4 * i) + 3] = mTransformMatrix[i].w;
		}
		return GetUnqiueID();
	}

	//setters
	void TransformComponent::setTransformMatrix(glm::mat4& aMatrix)
	{
		mTransformMatrix = aMatrix;
	}

	void TransformComponent::setPosition(glm::vec4 aPosition)
	{
		mTransformMatrix[3] = aPosition;
	}

	void TransformComponent::setScale(const glm::vec3& aScale)
	{
		mTransformMatrix[0][0] = aScale.x;
		mTransformMatrix[1][1] = aScale.x;
		mTransformMatrix[2][2] = aScale.x;
	}

	void TransformComponent::setScale(const float & aScale)
	{
		mTransformMatrix[0][0] = aScale;
		mTransformMatrix[1][1] = aScale;
		mTransformMatrix[2][2] = aScale;
	}

	void TransformComponent::invertPitch()
	{
		//invert pitch
		mTransformMatrix[0] = -mTransformMatrix[0];
	}

	//getters
	glm::mat4* TransformComponent::getTransformMatrix()
	{
		return &mTransformMatrix;
	}

	glm::vec4* TransformComponent::getPosition()
	{
		return &mTransformMatrix[3];
	}

	bool& TransformComponent::getHasChanged()
	{
		return mHasChanged;
	}
}