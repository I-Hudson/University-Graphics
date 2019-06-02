#pragma once

#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__

#include <glm/ext.hpp>

#include "Component/LightComponent.h"

class PointLight : public LightComponent
{
public:

	//Constructor
	PointLight(glm::vec4 aAmbient = glm::vec4(0.015f, 0.015f, 0.015f, 1), const float aConstant = 1.0f, 
			   const float aLinear = 0.14f, const float aQuadratic = 0.014f, const float aRadius = 7.0f);
	//Destructor
	~PointLight();

	//init
	void			init() override;
	//update
	void			update()override;
	//Draw the light volume
	void			draw(Shader* aShader, const bool& aBindTextures = true) override;
	void			gui()override;

	//call when the light pass has finished
	void			PostLight(Shader* aShader) override;

	//Setters
	void			setConstant(const float& aConstant);
	void			setlinear(const float& aLinear);
	void			setQuadratic(const float& aQuadratic);
	void			setRadius(const float& aRadius);
	void			setAmbient(const glm::vec4& aAmbient);

	//Getters
	float*			getConstant();
	float*			getLinear();
	float*			getQuadratic();
	float*			getRadius();
	glm::vec4*		getAmbient();

private:
	//Constatnt for attenuation
	float mConstant;
	//Linear for attenuation 
	float mLinear;
	//Quadratic for attenuation 
	float mQuadratic;
	//Radius for light volume
	float mRadius;
	//Ambient for light volume
	glm::vec4 mAmbient;

	void createSphereMesh();
	void setUniforms(Shader* aShader);

	//Cube VAO, VBO, IBO
	unsigned int mCubeVAO;
	unsigned int mCubeVBO;
	unsigned int mCubeIBO;
};
#endif // __POINT_LIGHT_H__