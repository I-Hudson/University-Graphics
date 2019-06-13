#include "Component/SpotLight.h"
#include "Shader/BaseShader.h"
#include "Gizmos.h"
#include "Component/TransformComponent.h"

SpotLight::SpotLight(const glm::vec4 & aLightTarget, const float & aInnerCutOff, const float & aOuterCutoff, const float & aLength)
{
	//set target position, inner cutoff, outer cutoff and length
	mTargetPosition = aLightTarget;
	mInnerCutoff = aInnerCutOff;
	mOuterCutoff = aOuterCutoff;
	mLength = aLength;
}

SpotLight::~SpotLight()
{
}

void SpotLight::init()
{
	//set position, diffuse, specular, light type, light directional
	//constant, linear, quadtratic
	LightComponent::init();
	setPosition(glm::vec4(0, 7, 0, 1.0f));
	setDiffuse(glm::vec4(1, 1, 1, 1));
	setSpecular(glm::vec4(1, 1, 1, 1));
	setLightType(LightType::Spot);
	mLightDir = glm::normalize(mTargetPosition - *getPosition());
	mConstant = 1.0f;
	mLinear = 0.7f;
	mQuadratic = 0.14f;
}

void SpotLight::draw(BaseShader* aShader, const bool& aBindTextures)
{
	//check if we have a mesh
	if (!meshGenerated())
	{
		//we have no vertices then draw the mesh 
		//we want to render
		createConeMesh();
	}

	if (aShader != nullptr)
	{
		setUniforms(aShader);
	}
	//draw
	LightComponent::draw(aShader);
}

void SpotLight::update()
{
	//if wireframe is active, draw sphere mesh 
	if (*wireFrameStatus())
	{
		Gizmos::addCone(*getPosition(), *getLightDirection(), 64, mLength, mInnerCutoff / 4, mInnerCutoff / 4, false, glm::vec4(1, 1, 1, 1));
		Gizmos::addCone(*getPosition(), *getLightDirection(), 64, mLength, mInnerCutoff / 4, mOuterCutoff / 4, false, glm::vec4(1, 1, 1, 1));
	}
	//if shadows have been added
	if (shadowsAdded())
	{
		//upate shadow projection view matrix
		glm::vec4 pos = *getPosition();
		glm::vec3 targetPos = pos.xyz + (glm::normalize(mLightDir).xyz * mLength);
		glm::vec3 lightInvDir = -mLightDir;

		glm::vec3 position = pos.xyz;
		glm::vec3 tarPos = glm::vec3(*getTargetPosition());
		glm::mat4 depthViewMatrix = glm::lookAt(position, tarPos, glm::vec3(0, 0, 1));
		glm::mat4 depthProjectionMatrix = glm::perspective<float>(glm::radians(45.0f), 1.0f, 2.0f, 50.0f);

		setShadowProViewMatrix(depthProjectionMatrix * depthViewMatrix);
	}

	//check if transform componment has changed this frame
	if (entity->getComponent<TransformComponent>()->getHasChanged())
	{
		clearMesh();
		createConeMesh();
	}

	//glm::mat4 matrix = entity->getComponent<TransformComponent>().getTransformMatrix()[2];
	//mLightDir = *getPosition() + (matrix[2] * mLength);
	//mTargetPosition = *getPosition() + (mLightDir * mLength);
}

void SpotLight::gui()
{
	if(ImGui::CollapsingHeader("Spot Light Component"))
	{
		bool hasChange = false;
		float tempColour[4] = 
		{
			getDiffuse()->x,
			getDiffuse()->y,
			getDiffuse()->z,
			getDiffuse()->w,
		};

		//colour
		if (ImGui::ColorPicker4("Colour", tempColour))
		{
			setDiffuse(glm::vec4(tempColour[0], tempColour[1], tempColour[2], tempColour[3]));
			hasChange = true;
		}

		tempColour[0] = getSpecular()->x;
		tempColour[1] = getSpecular()->y;
		tempColour[2] = getSpecular()->z;
		tempColour[3] = getSpecular()->w;
		//spec
		if (ImGui::ColorPicker4("Specular", tempColour))
		{
			setSpecular(glm::vec4(tempColour[0], tempColour[1], tempColour[2], tempColour[3]));
			hasChange = true;
		}

		//inner
		if (ImGui::SliderFloat("Inner Angle", &mInnerCutoff, 0.1f, mOuterCutoff))
		{
			hasChange = true;
		}

		//outer
		if (ImGui::SliderFloat("Outer Angle", &mOuterCutoff, mInnerCutoff, 60.0f))
		{
			hasChange = true;
		}

		//light length
		if (ImGui::SliderFloat("Light Length", &mLength, 0.1f, 25.0f))
		{
			hasChange = true;
		}

		//intensity
		if (ImGui::SliderFloat("Intensity", getIntensity(), 0.0f, 10.0f))
		{
		}

		if (ImGui::Checkbox("Wireframe", wireFrameStatus()))
		{
		}

		//length

		if (hasChange)
		{
			clearMesh();
			createConeMesh();
		}
	}
}

void SpotLight::Save(std::ofstream& aFile)
{
	aFile << "	SpotLightComponent:\n";
	aFile << "		InnerCutoff:"		<< mInnerCutoff << "\n";
	aFile << "		OuterCutoff:"		<< mOuterCutoff << "\n";
	aFile << "		Length:"			<< mLength << "\n";
	aFile << "		Constant:"			<< mConstant << "\n";
	aFile << "		Linear:"			<< mLinear << "\n";
	aFile << "		Quadratic:"			<< mQuadratic << "\n";
	aFile << "		DiffuseColour:"		<< getDiffuse()->x << "," << getDiffuse()->y << "," << getDiffuse()->z << "," << getDiffuse()->w << "\n";
	aFile << "		SpecularColour:"	<< getSpecular()->x << "," << getSpecular()->y << "," << getSpecular()->z << "," << getSpecular()->w << "\n";
	aFile << "!\n";
}

void SpotLight::createConeMesh()
{
	//create cone mesh
	glm::vec4 originPoint = *getPosition();
	glm::vec4 endPoint = originPoint + (mLightDir * mLength);

	int segments = 64;

	for (int i = 1; i < segments; ++i)
	{
		//get the previous angle
		float angle = 2 * glm::pi<float>() * (i - 1) / segments;

		float angleSin = glm::sin(angle);
		float angleCos = glm::cos(angle);

		glm::vec4 v1 = endPoint;
		v1 += glm::vec4(angleCos, 0.0f, angleSin, 0.0f) * mOuterCutoff;

		//get the current angle
		angle = 2 * glm::pi<float>() * i / segments;

		angleSin = glm::sin(angle);
		angleCos = glm::cos(angle);
		glm::vec4 v2 = endPoint;
		v2 += glm::vec4(angleCos, 0.0f, angleSin, 0.0f) * mOuterCutoff;

		addTri(v1, originPoint, v2);
		addTri(endPoint, v1, v2);
	}
	
	//do the last triangle needed
	//get the previous angle
	float angle = 2 * glm::pi<float>() * (segments - 1) / segments;

	float angleSin = glm::sin(angle);
	float angleCos = glm::cos(angle);

	glm::vec4 v1 = endPoint;
	v1 += glm::vec4(angleCos, 0.0f, angleSin, 0.0f) * mOuterCutoff;

	//get the current angle
	angle = 0;

	angleSin = glm::sin(angle);
	angleCos = glm::cos(angle);
	glm::vec4 v2 = endPoint;
	v2 += glm::vec4(angleCos, 0.0f, angleSin, 0.0f) * mOuterCutoff;

	addTri(v1, originPoint, v2);
	addTri(endPoint, v1, v2);
}

void SpotLight::setUniforms(BaseShader* aShader)
{
	//set uniforms
	aShader->setVec4("sLight.Direction", mLightDir, true);

	float angle = glm::cos(glm::radians(mInnerCutoff));
	aShader->setFloat("sLight.InnerCutoff", angle, true);
	float angle1 = glm::cos(glm::radians(mOuterCutoff));
	aShader->setFloat("sLight.OuterCutoff", angle1, true);

	aShader->setVec4("sLight.baseLight.Position", *getPosition(), true);
	aShader->setFloat("sLight.baseLight.Constant", mConstant, true);
	aShader->setFloat("sLight.baseLight.Linear", mLinear, true);
	aShader->setFloat("sLight.baseLight.Quadratic", mQuadratic, true);
	aShader->setVec4("sLight.baseLight.Ambient", mAmbient, true);
	aShader->setVec4("sLight.baseLight.Diffuse", *getDiffuse(), true);
	aShader->setVec4("sLight.baseLight.Specular", *getSpecular(), true);
	aShader->setFloat("sLight.Intensity", *getIntensity(), true);

	float l = 0.5f;
	aShader->setFloat("IsDirLight", l, true);
}
