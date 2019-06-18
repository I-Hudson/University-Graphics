#pragma once

#include <glm/glm.hpp>
#include "Component/LightComponent.h"

namespace InSight
{
	class DirectioanlLightData : public ComponentData
	{
	public:

	};

	class DirectionalLight : public LightComponent
	{
	public:
		//constructor
		DirectionalLight(const glm::vec4& aTargetPosition = glm::vec4(0, 0, 0, 1));
		//destructor
		~DirectionalLight();

		//overrides
		void			init() override;
		void			draw(BaseShader* aShader, const bool& aBindTextures = true) override;
		void			update() override;
		void			gui() override;

		virtual int Save() override;

		//setter
		void			setTargetPosition(const glm::vec4& aTargetPosition);

	private:
		//create a quad mesh
		void createQuadMesh();
		//set uniforms 
		void setUniforms(BaseShader* aShader);
	};
}