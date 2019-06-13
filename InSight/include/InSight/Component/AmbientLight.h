#pragma once

#ifndef __AMBIENT_LIGHT_H__
#define __AMBIENT_LIGHT_H__

#include <glm/glm.hpp>
#include "Component/LightComponent.h"

class AmbientLight : public LightComponent
{
public:
	//constructor
	AmbientLight();
	AmbientLight(const glm::vec4& aAmbientLight);
	//destructor
	~AmbientLight();

	//overrides
	void			init() override;
	void			gui() override;

	virtual void Save(std::ofstream& aFile) override;

private:
};

#endif // __AMBIENT_LIGHT_H__
