#pragma once

#ifndef __TRANSFORM_COMPONENT_H__
#define __TRANSFORM_COMPONENT_H__

#include "ECS.h"
#include <glm/ext.hpp>

class TransformComponent : public Component
{
public:
	void init() override;
	void gui() override;

	void setTransformMatrix(glm::mat4& aMatrix);
	void setPosition(glm::vec4 aPosition);
	void setScale(const glm::vec3& aScale);
	void setScale(const float& aScale);

	void invertPitch();

	glm::mat4* getTransformMatrix();
	glm::vec4* getPosition();

	bool& getHasChanged();
private:
	glm::mat4 mTransformMatrix;
	glm::vec4 mPosition;

	bool mHasChanged;
};

#endif // !__TRANSFORM_COMPONENT_H__