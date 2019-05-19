#pragma once

#ifndef __DIRECTIONAL_LIGHT__
#define __DIRECTIONAL_LIGHT__

#include <glm/glm.hpp>
#include "LightComponent.h"

class DirectionalLight : public LightComponent
{
public:
	DirectionalLight();
	DirectionalLight(const glm::vec4& aTargetPosition);
	~DirectionalLight();

	void			init() override;
	void			draw(Shader* aShader, const bool& aBindTextures = true) override;
	void			update() override;
	void			gui() override;

	//setter
	void			setTargetPosition(const glm::vec4& aTargetPosition);
	
private:
	void createQuadMesh();
	void setUniforms(Shader* aShader);
};


#endif // __DIRECTIONAL_LIGHT__