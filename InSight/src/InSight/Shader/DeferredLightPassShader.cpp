#include "Shader/DeferredLightPassShader.h"
#include <glm/ext.hpp>
#include "Component/PointLight.h"
#include "BaseRenderer.h"
#include "Component/CameraComponent.h"

DeferredLightPassShader::DeferredLightPassShader()
{
}

DeferredLightPassShader::DeferredLightPassShader(const char * aVertexPath, const char * aControlpath, const char * aEvaluationpath, 
									 const char * aGeometryPath, const char * aFragmentPath, unsigned int aInputCount,
									 const char * aInputs[], unsigned int aOutputCount, const char * aOutputs[]) : 
	BaseShader(aVertexPath, aControlpath, aEvaluationpath, aGeometryPath, aFragmentPath, aInputCount, aInputs, aOutputCount, aOutputs)
{
	//create line and tri array
	mLines = new  Line[32728];
	mTris = new  Tri[32728];

	//gen line buffers and tri buffers
	glGenBuffers(1, &mLineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mLineVBO);
	glBufferData(GL_ARRAY_BUFFER, 32728 * sizeof(Line), mLines, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &mTriVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mTriVBO);
	glBufferData(GL_ARRAY_BUFFER, 32728 * sizeof(Tri), mTris, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &mLineVAO);
	glBindVertexArray(mLineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mLineVBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);

	glGenVertexArrays(1, &mTriVAO);
	glBindVertexArray(mTriVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mTriVBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//stencil BaseShader
	const char* szInputsFS1[] = { "Position" };
	 mStencilShader = new  VertexShader(
		"./shaders/deferred/vertex_stenicl.glsl",
		"",
		"",
		"",
		"./shaders/deferred/fragment_stenicl.glsl",
		1, szInputsFS1, 0, nullptr
	);

	 //post lighting pass BaseShader
	 const char* postLightInputs[] = { "Position" };
	 mPostLightShader = new BaseShader(
		 "./shaders/vertex_postLight.glsl",
		 "",
		 "",
		 "",
		 "./shaders/fragment_postLight.glsl",
		 1, postLightInputs
	 );

	 //mLightBuffer = new GBuffer();
	 //mLightBuffer->createFrameBuffer();
	 //mLightBuffer->attachTextureToFrameBuffer(0, GL_RGBA, 1920, 1080, 0,
	//	 GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT0, "Normalised Light Buffer");
	 //mLightBuffer->attachTextureToFrameBuffer(0, GL_RGBA16F, 1920, 1080, 0,
	//	 GL_RGBA, GL_FLOAT, 0, GL_COLOR_ATTACHMENT1, "HDR Buffer");
	 //delete mLightBuffer;

	 //setGBuffer(mLightBuffer);

	 //hdr
	 mHDRState = false;
	 setBool("hdrOn", mHDRState);
}

DeferredLightPassShader::~DeferredLightPassShader()
{

}

void DeferredLightPassShader::destroy()
{
	//delete/destroy
	delete[] mLines;
	delete[] mTris;
	mStencilShader->destroy();
	delete mStencilShader;
	mPostLightShader->destroy();
	delete mPostLightShader;

	BaseShader::destroy();
}

void DeferredLightPassShader::useShader(bool aClear)
{
	aClear;
	setVec2("screenSize", *mGBuffer->getScreenSize());

	//need deferred BaseShader depth buffer
	mGBuffer->bindBuffer();
	//mGBuffer->setDrawBuffers();

	//has the draw buffers been set by a point or spot light
	bool hasInit = false;
	//define the draw buffers attachment
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };

	//set draw buffers
	glDrawBuffers(2, drawBuffers);
	glClear(GL_COLOR_BUFFER_BIT);

	//stencil pass
	glEnable(GL_STENCIL_TEST);

	//Stencil render pass
	for (auto& kv : *mLightVolumes->getAllVolumes())
	{
		LightType lt = kv->getLightType();
		if (lt == LightType::Directional)
		{
			continue;
		}

		//stenicl pass
		mStencilShader->useShader();
		glDrawBuffer(GL_NONE);
		glEnable(GL_DEPTH_TEST);

		//disable cull mode
		glDisable(GL_CULL_FACE);

		//clear stencil buffer
		glClear(GL_STENCIL_BUFFER_BIT);

		//set stencil func
		glStencilFunc(GL_ALWAYS, 0, 0);

		//update back face, keep the current value if test fails, if test passes incerment value, keep current value if stencil and depth pass
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		//update front face, keep the current value if test fails, if test passes decrement value, keep current value if stencil and depth pass
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

		//draw light volume
		kv->draw();

		//light pass
		glDrawBuffers(2, drawBuffers);
		//mLightBuffer->setDrawBuffers();
		hasInit = true;

		//update the stencil func, disable depth test, and enable blend
		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		//enable cull
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		//set progeam and set uniforms
		glUseProgram(mShaderID);
		setUniforms();

		//draw light volume
		kv->draw(this);

		//set cull to back and disable blend
		glCullFace(GL_BACK);
		glDisable(GL_BLEND);
	}

	//disable stencil
	glDisable(GL_STENCIL_TEST);

	//check if a light volume (not directional) has drawn
	if (!hasInit)
	{
		//light pass
		glDrawBuffers(2, drawBuffers);
		//mLightBuffer->setDrawBuffers();
		glUseProgram(mShaderID);
		setUniforms();
	}

	//disable depth test, enable blend, set blend func
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	//find directional light and draw it
	for (auto& kv : *mLightVolumes->getAllVolumes())
	{
		if (kv->getLightType() == LightType::Directional)
		{
			kv->draw(this);
			break;
		}
	}

	//disable blend
	glDisable(GL_BLEND);

	//after all the lighting has been completed
	//redner the light boxes for the point lights
	glEnable(GL_DEPTH_TEST);

	glUseProgram(mPostLightShader->getShader());
	mPostLightShader->setMat4("ProjectionView",
	*getBaseRenderer()->getEntityManager()->getEntity("Main Camera")->getComponent<CameraComponent>()->getProjectionViewMatrix(),
		true);
	for (auto& kv : *mLightVolumes->getAllVolumes())
	{
		kv->PostLight(mPostLightShader);
	}
	glUseProgram(0);

	//unbind buffer
	mGBuffer->unBindBuffer();


}


void DeferredLightPassShader::addLine(glm::vec4 aP0, glm::vec4 aP1)
{
	//add line
	mLines[mLineCount].p0 = aP0;
	mLines[mLineCount].p1 = aP1;
	mLineCount++;
}

void DeferredLightPassShader::addTri(glm::vec4 aP0, glm::vec4 aP1, glm::vec4 aP2)
{
	//add tri
	mTris[mTriCount].p1 = aP0;
	mTris[mTriCount].p2 = aP1;
	mTris[mTriCount].p3 = aP2;

	mTriCount++;
}

//void DeferredLightPassShader::addPointLight(glm::vec4 aPosition, float aRadius)
//{
//	int a_rows = 64;
//	int a_columns = 64;
//	float a_latMax = 90;
//	float a_latMin = -90;
//	float a_longMax = 360;
//	float a_longMin = 0;
//	float a_radius = aRadius;
//	glm::vec4 a_center = aPosition;
//	//Invert these first as the multiply is slightly quicker
//	float invColumns = 1.0f / float(a_columns);
//	float invRows = 1.0f / float(a_rows);
//
//	float DEG2RAD = glm::pi<float>() / 180;
//
//	//Lets put everything in radians first
//	float latitiudinalRange = (a_latMax - a_latMin) * DEG2RAD;
//	float longitudinalRange = (a_longMax - a_longMin) * DEG2RAD;
//	// for each row of the mesh
//	glm::vec4* v4Array = new  glm::vec4[a_rows*a_columns + a_columns];
//
//	for (int row = 0; row <= a_rows; ++row)
//	{
//		// y ordinates this may be a little confusing but here we are navigating around the xAxis in GL
//		float ratioAroundXAxis = float(row) * invRows;
//		float radiansAboutXAxis = ratioAroundXAxis * latitiudinalRange + (a_latMin * DEG2RAD);
//		float y = a_radius * sin(radiansAboutXAxis);
//		float z = a_radius * cos(radiansAboutXAxis);
//
//		for (int col = 0; col <= a_columns; ++col)
//		{
//			float ratioAroundYAxis = float(col) * invColumns;
//			float theta = ratioAroundYAxis * longitudinalRange + (a_longMin * DEG2RAD);
//			glm::vec4 v4Point(-z * sinf(theta), y, -z * cosf(theta), 1.0f);
//
//			int index = row * a_columns + (col % a_columns);
//			v4Array[index] = a_center + v4Point;
//		}
//	}
//
//	for (int face = 0; face < (a_rows)*(a_columns); ++face)
//	{
//		int iNextFace = face + 1;
//
//		if (iNextFace % a_columns == 0)
//		{
//			iNextFace = iNextFace - (a_columns);
//		}
//
//		addLine(v4Array[face], v4Array[face + a_columns]);
//
//		if (face % a_columns == 0 && longitudinalRange < (glm::pi<float>() * 2))
//		{
//			continue;
//		}
//		addLine(v4Array[iNextFace + a_columns], v4Array[face + a_columns]);
//
//		addTri(v4Array[iNextFace + a_columns], v4Array[face], v4Array[iNextFace]);
//		addTri(v4Array[iNextFace + a_columns], v4Array[face + a_columns], v4Array[face]);
//	}
//
//	delete[] v4Array;
//}
//
//void DeferredLightPassShader::addFullScreenQuad()
//{
//	FSQuadVertex vertexData[4];
//	vertexData[0] = FSQuadVertex(glm::vec4(-1.f, -1.f, 0.f, 1.f), glm::vec2(0.f, 0.f));
//	vertexData[1] = FSQuadVertex(glm::vec4(1.f, -1.f, 0.f, 1.f), glm::vec2(1.f, 0.f));
//	vertexData[2] = FSQuadVertex(glm::vec4(-1.f, 1.f, 0.f, 1.f), glm::vec2(0.f, 1.f));
//	vertexData[3] = FSQuadVertex(glm::vec4(1.f, 1.f, 0.f, 1.f), glm::vec2(1.f, 1.f));
//
//	unsigned int indics[6] = { 0,1,2,1,3,2 };
//
//	addTri(vertexData[0].Position, vertexData[1].Position, vertexData[2].Position);
//	addTri(vertexData[1].Position, vertexData[3].Position, vertexData[2].Position);
//}

BaseShader* DeferredLightPassShader::getStencilShader()
{
	//return pointer to stencil BaseShader
	return mStencilShader;
}

void DeferredLightPassShader::setLightVolumeManager(LightVolumeManager* aManager)
{
	//set light volume manager
	mLightVolumes = aManager;
	for (auto& light : *mLightVolumes->getAllVolumes())
	{
		if (light->getLightType() == LightType::Ambient)
		{
			setVec4("AmbientColour", *light->getDiffuse());
			break;
		}
	}
}

void DeferredLightPassShader::setHDRState(bool aState)
{
	//set hdr state
	mHDRState = aState;
}

bool DeferredLightPassShader::getHDRState()
{
	//get hdr state
	return mHDRState;
}
