#pragma once

#ifndef __TRANSFORM_COMPONENT_H__
#define __TRANSFORM_COMPONENT_H__

#include "ECS.h"
#include <glm/ext.hpp>

class TransformComponent : public Component
{
public:
	//overrides
	void init() override;
	void gui() override;

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

#endif // !__TRANSFORM_COMPONENT_H__