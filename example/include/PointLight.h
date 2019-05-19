#pragma once

#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__

#include <glm/ext.hpp>

#include "LightComponent.h"

class PointLight : public LightComponent
{
public:

	//Constructor
	PointLight();
	PointLight(glm::vec4 aAmbient, const float aConstant, 
			   const float aLinear, const float aQuadrati, const float aRadius);
	//Destructor
	~PointLight();

	void			init() override;
	void			update()override;
	//Draw the light volume
	void			draw(Shader* aShader, const bool& aBindTextures = true) override;
	void			gui()override;

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
};
#endif // __POINT_LIGHT_H__