#pragma once

#include <glm/glm.hpp>
#include "Component/LightComponent.h"

namespace InSight
{
	class AmbientLightData : public ComponentData
	{
	public:

	};

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

		virtual int Save() override;

	private:
	};
}
