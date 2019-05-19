#include "PointLight.h"
#include "Shader.h"
#include "Gizmos.h"
#include "TransformComponent.h"

PointLight::PointLight()
{
}

PointLight::PointLight(glm::vec4 aAmbient /*= glm::vec4(0.015f, 0.015f, 0.015f, 1)*/, const float aConstant /*= 1.0f*/,
					   const float aLinear /*= 0.14f*/, const float aQuadratic /*= 0.07f*/, const float aRadius /*= 7.0f*/)
{
	mAmbient = aAmbient;
	mConstant = aConstant;
	mLinear = aLinear;
	mQuadratic = aQuadratic;
	mRadius = aRadius;

	setLightType(LightType::Point);
}

PointLight::~PointLight()
{
}

void PointLight::init()
{
	LightComponent::init();
	setPosition(glm::vec4(0, 0, 0, 1));
	setDiffuse(glm::vec4(1, 1, 1, 1));
	setSpecular(glm::vec4(1, 1, 1, 1));
}

void PointLight::update()
{
	if (*wireFrameStatus())
	{
		Gizmos::addSphere(*getPosition(), 64, 64, mRadius, true, glm::vec4(1, 1, 1, 1));
	}

	if (entity->getComponent<TransformComponent>().getHasChanged())
	{
		clearMesh();
		createSphereMesh();
	}
}

void PointLight::createSphereMesh()
{
	int a_rows = 64;
	int a_columns = 64;
	float a_latMax = 90;
	float a_latMin = -90;
	float a_longMax = 360;
	float a_longMin = 0;
	float a_radius = mRadius;
	glm::vec4 a_center = *getPosition();

	//Invert these first as the multiply is slightly quicker
	float invColumns = 1.0f / float(a_columns);
	float invRows = 1.0f / float(a_rows);

	float DEG2RAD = glm::pi<float>() / 180;

	//Lets put everything in radians first
	float latitiudinalRange = (a_latMax - a_latMin) * DEG2RAD;
	float longitudinalRange = (a_longMax - a_longMin) * DEG2RAD;
	// for each row of the mesh
	glm::vec3* v4Array = new  glm::vec3[a_rows*a_columns + a_columns];

	for (int row = 0; row <= a_rows; ++row)
	{
		// y ordinates this may be a little confusing but here we are navigating around the xAxis in GL
		float ratioAroundXAxis = float(row) * invRows;
		float radiansAboutXAxis = ratioAroundXAxis * latitiudinalRange + (a_latMin * DEG2RAD);
		float y = a_radius * sin(radiansAboutXAxis);
		float z = a_radius * cos(radiansAboutXAxis);

		for (int col = 0; col <= a_columns; ++col)
		{
			float ratioAroundYAxis = float(col) * invColumns;
			float theta = ratioAroundYAxis * longitudinalRange + (a_longMin * DEG2RAD);
			glm::vec3 v4Point(-z * sinf(theta), y, -z * cosf(theta));

			int index = row * a_columns + (col % a_columns);
			v4Array[index] = a_center.xyz + v4Point;
		}
	}

	for (int face = 0; face < (a_rows)*(a_columns); ++face)
	{
		int iNextFace = face + 1;

		if (iNextFace % a_columns == 0)
		{
			iNextFace = iNextFace - (a_columns);
		}

		if (face % a_columns == 0 && longitudinalRange < (glm::pi<float>() * 2))
		{
			continue;
		}

		addTri(glm::vec4(v4Array[iNextFace + a_columns], 1.0f), glm::vec4(v4Array[face], 1.0f), glm::vec4(v4Array[iNextFace], 1.0f));
		addTri(glm::vec4(v4Array[iNextFace + a_columns], 1.0f), glm::vec4(v4Array[face + a_columns], 1.0f), glm::vec4(v4Array[face], 1.0f));
	}

	delete[] v4Array;
}

void PointLight::setUniforms(Shader* aShader)
{
	aShader->setVec4("pLight.Position", *getPosition(), true);
	aShader->setFloat("pLight.Constant", mConstant, true);
	aShader->setFloat("pLight.Linear", mLinear, true);
	aShader->setFloat("pLight.Quadratic", mQuadratic, true);
	aShader->setFloat("pLight.Radius", mRadius, true);
	aShader->setVec4("pLight.Ambient", mAmbient, true);
	aShader->setVec4("pLight.Diffuse", *getDiffuse(), true);
	aShader->setVec4("pLight.Specular", *getSpecular(), true);
	aShader->setFloat("pLight.Intensity", *getIntensity(), true);

	float l = 0.0f;
	aShader->setFloat("IsDirLight", l, true);
}

void PointLight::draw(Shader* aShader, const bool& aBindTextures)
{
	aBindTextures;
	//checl if we have a mesh
	if (!meshGenerated())
	{
		//we have no vertices then draw the mesh 
		//we want to render
		createSphereMesh();
	}
	if (aShader != nullptr)
	{
		setUniforms(aShader);
	}

	//draw the mesh by calling the base class
	LightComponent::draw();
}

void PointLight::gui()
{
	if (ImGui::CollapsingHeader("Point Light Component"))
	{
		bool hasChange = false;
		//colour
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

		//size
		float halfRadius = mRadius * 0.5f;
		if (ImGui::SliderFloat("Radius", &halfRadius, 0.1f, 10.0f))
		{
			mRadius = halfRadius * 2;
			hasChange = true;
		}

		if (ImGui::SliderFloat("Intensity", getIntensity(), 0.0f, 10.0f))
		{
		}

		//intensity
		if (ImGui::Checkbox("Wireframe", wireFrameStatus()))
		{
		}

		if (hasChange)
		{
			clearMesh();
			createSphereMesh();
		}
	}
}

void PointLight::setConstant(const float & aConstant)
{
	mConstant = aConstant;
}

void PointLight::setlinear(const float & aLinear)
{
	mLinear = aLinear;
}

void PointLight::setQuadratic(const float & aQuadratic)
{
	mQuadratic = aQuadratic;
}

void PointLight::setRadius(const float & aRadius)
{
	mRadius = aRadius;
}

void PointLight::setAmbient(const glm::vec4 & aAmbient)
{
	mAmbient = aAmbient;
}

float* PointLight::getConstant()
{
	return &mConstant;
}

float* PointLight::getLinear()
{
	return &mLinear;
}

float* PointLight::getQuadratic()
{
	return &mQuadratic;
}

float* PointLight::getRadius()
{
	return &mRadius;
}

glm::vec4* PointLight::getAmbient()
{
	return &mAmbient;
}
