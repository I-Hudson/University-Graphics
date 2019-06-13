#pragma once

#ifndef __SPOT_LIGHT_H__
#define __SPOT_LIGHT_H__

#include <glm/ext.hpp>
#include "Component/LightComponent.h"

class SpotLight : public LightComponent
{
public:
	//Constructor
	SpotLight(const glm::vec4& aLightTarget = glm::vec4(0, 0, 0, 1),
			 const float& aInnerCutOff = 7.f, const float& aOuterCutoff = 60.0f, const float& aLength = 10.0f);
	//Destructor
	~SpotLight();

	void		init()override;
	void		draw(BaseShader* aShader, const bool& aBindTextures = true) override;
	void		update() override;
	void		gui()override;

	virtual void Save(std::ofstream& aFile) override;

private:
	//create the cone mesh for the light volume
	void		createConeMesh();
	//set all the uniforms for this light
	void		setUniforms(BaseShader* aShader);

	//cutoff for the inner edge of the spot light
	float mInnerCutoff;
	//cutoff for the outer edge of the spot light
	float mOuterCutoff;
	//the length the spot light extends
	float mLength;

	//Constatnt for attenuation
	float mConstant;
	//Linear for attenuation 
	float mLinear;
	//Quadratic for attenuation 
	float mQuadratic;
	//Ambient for light volume
	glm::vec4 mAmbient;
};

#endif // __SPOT_LIGHT_H__
