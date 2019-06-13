#include "Component/TransformComponent.h"

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

void TransformComponent::Save(std::ofstream& aFile)
{
	aFile << "	TransformComponent:\n";
	aFile << "		MatrixX:" << mTransformMatrix[0].x << "," << mTransformMatrix[0].y << "," << mTransformMatrix[0].z << "," << mTransformMatrix[0].w << "\n";
	aFile << "		MatrixY:" << mTransformMatrix[1].x << "," << mTransformMatrix[1].y << "," << mTransformMatrix[1].z << "," << mTransformMatrix[1].w << "\n";
	aFile << "		MatrixZ:" << mTransformMatrix[2].x << "," << mTransformMatrix[2].y << "," << mTransformMatrix[2].z << "," << mTransformMatrix[2].w << "\n";
	aFile << "		MatrixW:" << mTransformMatrix[3].x << "," << mTransformMatrix[3].y << "," << mTransformMatrix[3].z << "," << mTransformMatrix[3].w << "\n";
	aFile << "!\n";
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
