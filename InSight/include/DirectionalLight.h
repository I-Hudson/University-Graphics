#pragma once

#ifndef __DIRECTIONAL_LIGHT__
#define __DIRECTIONAL_LIGHT__

#include <glm/glm.hpp>
#include "LightComponent.h"

class DirectionalLight : public LightComponent
{
public:
	//constructor
	DirectionalLight(const glm::vec4& aTargetPosition = glm::vec4(0,0,0,1));
	//destructor
	~DirectionalLight();

	//overrides
	void			init() override;
	void			draw(Shader* aShader, const bool& aBindTextures = true) override;
	void			update() override;
	void			gui() override;

	//setter
	void			setTargetPosition(const glm::vec4& aTargetPosition);
	
private:
	//create a quad mesh
	void createQuadMesh();
	//set uniforms 
	void setUniforms(Shader* aShader);
};


#endif // __DIRECTIONAL_LIGHT__