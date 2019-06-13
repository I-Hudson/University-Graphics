#include "Component/AmbientLight.h"

AmbientLight::AmbientLight()
{
	//set light type
	setLightType(LightType::Ambient);
}

AmbientLight::AmbientLight(const glm::vec4 & aAmbientLight)
{
	//set poition, diffuse, light type
	setPosition(glm::vec4(0, 0, 0, 1));
	setDiffuse(aAmbientLight);
	setLightType(LightType::Ambient);
}

AmbientLight::~AmbientLight()
{
}

void AmbientLight::init()
{
}

void AmbientLight::gui()
{
	//guii colour
	if (ImGui::CollapsingHeader("Ambient Light Component"))
	{
		float tempColour[4];
		tempColour[0] = getDiffuse()->x;
		tempColour[1] = getDiffuse()->y;
		tempColour[2] = getDiffuse()->z;
		tempColour[3] = getDiffuse()->w;
		//colour
		if (ImGui::ColorPicker4("Colour", tempColour))
		{
			setDiffuse(glm::vec4(tempColour[0], tempColour[1], tempColour[2], tempColour[3]));
		}
	}
}

void AmbientLight::Save(std::ofstream& aFile)
{
	aFile << "	AmbientLightComponent:\n";
	aFile << "		DiffuseColour:" << getDiffuse()->x << "," << getDiffuse()->y << "," << getDiffuse()->z << "," << getDiffuse()->w << "\n";
	aFile << "		SpecularColour:" << getSpecular()->x << "," << getSpecular()->y << "," << getSpecular()->z << "," << getSpecular()->w << "\n";
	aFile << "!\n";
}
