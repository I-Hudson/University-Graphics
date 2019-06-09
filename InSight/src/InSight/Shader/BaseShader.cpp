#include "Shader/BaseShader.h"
#include "Utilities.h"
#include "Log.h"
#include "Entity/Entity.h"	

#include "Component/MeshComponent.h"
#include <glm/ext.hpp>

void BaseShader::setLightVolumeManager(LightVolumeManager * aManager)
{
	//set light volume manager
	mLightVolumes = aManager;
}

BaseShader::BaseShader() : 
	mShaderPriority(ShaderPriority::PRI_0)
{
	//set gbuffer and mode to render
	mGBuffer = nullptr;
	mModeToRender = RenderMode::Opaque;
}

BaseShader::BaseShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath, const char* aGeometryPath, const char* aFragmentPath,
	unsigned int aInputCount /*= 3*/, const char* aInputs[] /*= nullptr*/, unsigned int aOutputCount /*= 1*/, const char* aOutputs[]/*= nullptr*/)
	: mShaderPriority(ShaderPriority::PRI_0)
{
	//load all shaders 
	unsigned int vertex = Utility::loadShader(aVertexPath, GL_VERTEX_SHADER);
	unsigned int control = Utility::loadShader(aControlpath, GL_TESS_CONTROL_SHADER);
	unsigned int evaluation = Utility::loadShader(aEvaluationpath, GL_TESS_EVALUATION_SHADER);
	unsigned int geometry = Utility::loadShader(aGeometryPath, GL_GEOMETRY_SHADER);
	unsigned int fragment = Utility::loadShader(aFragmentPath, GL_FRAGMENT_SHADER);
	
	//const char* szInputs[] = { "Position", "Color", "Normal", "Tangnet", "Bitangent", "TexCoord1" };
	const char* szInputs[] = { "Position", "Normal", "TexCoord1" };
	const char* szOutputs[] = { "FragColour" };

	//create program
	mShaderID = Utility::createProgram(
		vertex, 
		control, 
		evaluation, 
		geometry, 
		fragment,
		aInputCount,
		aInputs != nullptr ? aInputs : szInputs,
		aOutputCount,
		aOutputs != nullptr ? aOutputs : szOutputs);
	
	//delete shaders
	glDeleteShader(vertex);
	glDeleteShader(control);
	glDeleteShader(evaluation);
	glDeleteShader(geometry);
 	glDeleteShader(fragment);

	//reserve splace for the vector
	//mMeshes.reserve(16);

	//set gbuffer and mode to render
	mGBuffer = nullptr;
	mModeToRender = RenderMode::Opaque;
}

BaseShader::~BaseShader()
{

}

void BaseShader::destroy()
{
	//delete/destroy
	glDeleteProgram(mShaderID);
	glDeleteFramebuffers(1, &mFrameBuffer);

	mBoolUniforms.clear();
	mUnsignedIntUniforms.clear();
	mFloatUniforms.clear();
	mVec2Uniforms.clear();
	mVec3Uniforms.clear();
	mVec4Uniforms.clear();
	mMat2Uniforms.clear();
	mMat3Uniforms.clear();
	mMat4Uniforms.clear();

	for (auto& t : mFrameBufferTextures)
	{
		glDeleteTextures(1, &t.mTexture);
	}
	mFrameBufferTextures.clear();

	if (mGBuffer != nullptr && !mGBuffer->hasBeenDeleted())
	{
		mGBuffer->destroy();
		delete mGBuffer;
	}

	mMeshComponent = nullptr;
	mMeshes.clear();

	EN_TRACE("BaseShader Has Been Deleted");
}

void BaseShader::addMesh(MeshComponent* aMesh)
{
	bool found = false;
	short insertLocation = 0;
	//loop thought all current meshes
	for(int i = 0; i < mMeshes.size(); ++i)
	{
		//if we find the the mesh we want to add
		//then break
		if (mMeshes[i] == aMesh)
		{
			found = true;
			break;
		}
		//get the position of the mesh. If it is closer to the center of the world
		//insert before current mesh
		glm::vec3 newMeshVec3 = *aMesh->entity->getComponent<TransformComponent>()->getPosition();
		float newMeshDist = glm::distance(newMeshVec3, glm::vec3(0, 0, 0));
		glm::vec3 currentMeshVec3 = *mMeshes[i]->entity->getComponent<TransformComponent>()->getPosition();
		float currentMeshDist = glm::distance(currentMeshVec3, glm::vec3(0, 0, 0));

		if (newMeshDist < currentMeshDist)
		{
			insertLocation = i;
		}
	}

	if (!found)
	{
		//if aMesh is not found add it
		mMeshes.insert(mMeshes.begin() + 0, aMesh);
	}
}

void BaseShader::removeMesh(MeshComponent* aMesh)
{
	if (std::find(mMeshes.begin(), mMeshes.end(), aMesh) != mMeshes.end())
	{
		//if aMesh is not found add it
		//mMeshes.(aMesh);
	}
}

bool compareDistance(MeshComponent* aM1, MeshComponent* aM2)
{
	//compare the distance between two meshes
	glm::vec3 newMeshVec3 = *aM1->entity->getComponent<TransformComponent>()->getPosition();
	float newMeshDist = glm::distance(newMeshVec3, glm::vec3(0, 0, 0));
	glm::vec3 currentMeshVec3 = *aM2->entity->getComponent<TransformComponent>()->getPosition();
	float currentMeshDist = glm::distance(currentMeshVec3, glm::vec3(0, 0, 0));

	return newMeshDist < currentMeshDist;
}

void BaseShader::sortMeshOrder()
{
	//sort all meshes 
	std::sort(mMeshes.begin(), mMeshes.end(), compareDistance);
}

void BaseShader::setDrawBuffers()
{
	//set the draw buffers
	glDrawBuffers(drawBuffers.size(), drawBuffers.data());
}

void BaseShader::drawMeshes(const bool& aBindTextures, const glm::mat4& aModelMatrx)
{
	//draw all meshes
	for (unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i]->draw(this, aModelMatrx, aBindTextures);
	}
}

void BaseShader::drawMeshes(std::vector<MeshComponent*>& aMeshes, const bool& aBindTextures, const glm::mat4& aModelMatrx)
{
	//draw all meshes
	for (unsigned int i = 0; i < aMeshes.size(); ++i)
	{
		if (aMeshes[i]->getRenderMode() == mModeToRender)
		{
			aMeshes[i]->draw(this, aModelMatrx, aBindTextures);
		}
	}
}

void BaseShader::gui()
{
	if (ImGui::CollapsingHeader("BaseShader"))
	{

	}
}

void BaseShader::useShader(bool aClear, const bool& aActiveTextures, Entity* aE)
{
	//if clear is true clear
	if (aClear)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glUseProgram(mShaderID);

	setUniforms();

	//get all mesh components. if they are the same render mode draw them
	std::vector<MeshComponent*> allMeshes = aE->getEntityManger()->getAllEntityComponents<MeshComponent>();
	for (unsigned int i = 0; i < allMeshes.size(); ++i)
	{
		if (allMeshes[i]->getRenderMode() == mModeToRender)
		{
			allMeshes[i]->draw(this, glm::mat4(), aActiveTextures);
		}
	}

	glUseProgram(0);
}

void BaseShader::useShader(bool aClear /* = false */, const bool& aActiveTextures /*= true */)
{
	//sort meshes
	//sortMeshOrder();

	//if (mFrameBuffer != -1)
	//{
	//	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
	//}
	//else
	//{
	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//}

	//if (aClear)
	//{
	//	//bind framebuffer
	//	if (mFrameBuffer != -1)
	//	{
	//		//glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
	//	}
	//	else
	//	{
	//		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//	}
	//	//clear framebuffer
	//	//glClearColor(0.25, 0.25, 0.25, 1.0);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//}

	//bind gbuffer
	if (mGBuffer != nullptr)
	{
		mGBuffer->bindBuffer();
	}
	else
	{
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//if clear is true, clear
	if (aClear)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glUseProgram(mShaderID);

	setUniforms();

	//get all mesh components. if they are the same render mode draw them
	std::vector<MeshComponent*> allMeshes = mMeshComponent->entity->getEntityManger()->getAllEntityComponents<MeshComponent>();
	for (unsigned int i = 0; i < allMeshes.size(); ++i)
	{
		if (allMeshes[i]->getRenderMode() == mModeToRender)
		{
			allMeshes[i]->draw(this, glm::mat4(), aActiveTextures);
		}
	}

	glUseProgram(0);
}

void BaseShader::useShader(bool aClear, const int& aDrawMode)
{
	//if clear is true, clear
	if (aClear)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glUseProgram(mShaderID);

	setUniforms();

	//std::vector<MeshComponent*> allMeshes = mMeshComponent->entity->getEntityManger()->getAllEntityComponents<MeshComponent>();
	//draw all meshes
	for (unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		if (mMeshes[i]->getRenderMode() == mModeToRender)
		{
			mMeshes[i]->draw(aDrawMode, this);
		}
	}

	glUseProgram(0);
}

void BaseShader::useShader(bool aClear /* = false */)
{
	//sort meshes
	//sortMeshOrder();

	//if (mFrameBuffer != -1)
	//{
	//	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
	//}
	//else
	//{
	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//}

	//if (aClear)
	//{
	//	//bind framebuffer
	//	if (mFrameBuffer != -1)
	//	{
	//		//glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
	//	}
	//	else
	//	{
	//		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//	}
	//	//clear framebuffer
	//	//glClearColor(0.25, 0.25, 0.25, 1.0);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//}

		//if clear is true, clear
	if (aClear)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glUseProgram(mShaderID);

	setUniforms();

	//std::vector<MeshComponent*> allMeshes = mMeshComponent->entity->getEntityManger()->getAllEntityComponents<MeshComponent>();
	//draw all meshes
	for (unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		if (mMeshes[i]->getRenderMode() == mModeToRender)
		{
			mMeshes[i]->draw(this);
		}
	}

	glUseProgram(0);
}

void BaseShader::useShaderPP(bool aClear)
{
	//if clear is true, clear
	if (aClear)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glUseProgram(mShaderID);

	setUniforms();
}

void BaseShader::createFrameBuffer()
{
	//generate framebuffer
	glGenFramebuffers(1, &mFrameBuffer);
}

unsigned int BaseShader::attachTextureToFrameBuffer(int aLevel, int aInternalFormat,
										int aWidth, int aHeight, 
										int aBoarder, GLenum aFormat, 
										GLenum aType, const void* aPixels,
										GLenum aAttachment)
{
	//bind buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

	unsigned int fbo;

	//gen texture, set texture props
	glGenTextures(1, &fbo);
	glBindTexture(GL_TEXTURE_2D, fbo);
	glTexImage2D(GL_TEXTURE_2D, aLevel, aInternalFormat, aWidth, aHeight, aBoarder, aFormat, aType, aPixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (aAttachment == GL_DEPTH_ATTACHMENT)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	glFramebufferTexture(GL_FRAMEBUFFER, aAttachment, fbo, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	FBTexture fbTexture;
	fbTexture.mTextureSize = glm::vec2(aWidth, aHeight);
	fbTexture.mTexture = fbo;

	//add texture to mFrameBufferTextures
	mFrameBufferTextures.push_back(fbTexture);

	//update draw buffers
	if (aAttachment != GL_DEPTH_STENCIL_ATTACHMENT &&
		aAttachment != GL_COLOR_ATTACHMENT4 && aAttachment != GL_DEPTH_ATTACHMENT)
	{
		drawBuffers.push_back(aAttachment);
		glDrawBuffers(drawBuffers.size(), drawBuffers.data());
		//setDrawBuffers();
	}

	//check status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{

		printf("Framebuffer Error\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return fbo;
}

void BaseShader::setMeshComponent(MeshComponent* aMeshComponent)
{
	//set mesh component
	mMeshComponent = aMeshComponent;
	mMeshes.push_back(aMeshComponent);
}

MeshComponent* BaseShader::getMeshComponent()
{
	//get mesh component
	return mMeshComponent;
}

void BaseShader::setGBuffer(GBuffer * aGBuffer)
{
	//set gbuffer
	mGBuffer = aGBuffer;
}

GBuffer* BaseShader::getGBuffer()
{
	//get pointer to gbuffer
	return mGBuffer;
}

void BaseShader::setShaderPriority(ShaderPriority a_shaderPriority /*= ShaderPriority::PRI_5*/)
{
	//set priority
	mShaderPriority = a_shaderPriority;
}

ShaderPriority BaseShader::getShaderPriority()
{
	return mShaderPriority;
}

unsigned int* BaseShader::getTexture(int aIndex)
{
	//get a texture
	if (mFrameBufferTextures.size() > 0)
	{
		return &mFrameBufferTextures[aIndex].mTexture;
	}
	else
	{
		return 0;
	}
}

unsigned int BaseShader::numOfTexturesAttached() const
{
	//return number of textures attached
	return mFrameBufferTextures.size();
}

std::vector<FBTexture>* BaseShader::getAllTextures()
{
	//get all texures
	return &mFrameBufferTextures;
}

void BaseShader::setBool(const std::string& name, bool& aValue)
{
	//glUniform1i(glGetUniformLocation(mShaderID, name.c_str()), (int)value);
	//add a new bool uniform
	mBoolUniforms.emplace(std::pair<std::string, bool*>(name, &aValue));
}
// ------------------------------------------------------------------------
void BaseShader::setUnsignedIntUniforms(const std::string &aName, unsigned int& aValue)
{
	//glUniform1i(glGetUniformLocation(mShaderID, name.c_str()), value);
	//add a new unsigned int uniform
	mUnsignedIntUniforms.emplace(std::pair<std::string, unsigned int*>(aName, &aValue));
}
void BaseShader::setUnsignedIntUniformsOverride(const std::string& aName, unsigned int& aValue)
{
	//find uniform with aName
	std::map< std::string, unsigned int*>::iterator it = mUnsignedIntUniforms.find(aName);
	if (it != mUnsignedIntUniforms.end())
	{
		//if uniform found set it's value to aValue
		mUnsignedIntUniforms[aName] = &aValue;
	}
	else
	{
		//add a new unsigned int uniform
		mUnsignedIntUniforms.emplace(std::pair<std::string, unsigned int*>(aName, &aValue));
	}
}
// ------------------------------------------------------------------------
void BaseShader::setFloat(const std::string& name, float& value, bool aSetEveryFrame /*= false*/)
{
	//if aSetEveryFrame is true then all set uniform for this frame
	if (aSetEveryFrame)
	{
		unsigned int loc = glGetUniformLocation(mShaderID, name.c_str());
		glUniform1f(loc, value);
		return;
	}
	//glUniform1f(glGetUniformLocation(mShaderID, name.c_str()), value);
	//add a new float int uniform
	mFloatUniforms.emplace(std::pair<std::string, float*>(name, &value));
}
void BaseShader::setFloatOverride(const std::string & aName, float & aValue)
{	
	//find uniform with aName
	std::map< std::string, float*>::iterator it = mFloatUniforms.find(aName);
	if (it != mFloatUniforms.end())
	{
		//if uniform found set it's value to aValue
		mFloatUniforms[aName] = &aValue;
	}
	else
	{
		//add a new float uniform
		mFloatUniforms.emplace(std::pair<std::string, float*>(aName, &aValue));
	}
}
// ------------------------------------------------------------------------
void BaseShader::setVec2(const std::string& name, glm::vec2& value)
{
	//glUniform2fv(glGetUniformLocation(mShaderID, name.c_str()), 1, &value[0]);
	//add a new vec2 uniform
	mVec2Uniforms.emplace(std::pair<std::string, glm::vec2*>(name, &value));
}
void BaseShader::setVec2(const std::string& name, float x, float y)
{
	//glUniform2f(glGetUniformLocation(mShaderID, name.c_str()), x, y);
	//mVec2Uniforms.insert(std::pair<std::string, std::shared_ptr<glm::vec2>>(name, 
	//	std::make_shared<glm::vec2>(glm::vec2(x,y))));
}
// ------------------------------------------------------------------------
void BaseShader::setVec3(const std::string& name, glm::vec3& value, bool aSetEveryFrame /*= false*/)
{
	//if aSetEveryFrame is true then all set uniform for this frame
	if (aSetEveryFrame)
	{
		unsigned int loc = glGetUniformLocation(mShaderID, name.c_str());
		glUniform3fv(loc, 1, glm::value_ptr(value));
		return;
	}
	//glUniform3fv(glGetUniformLocation(mShaderID, name.c_str()), 1, &value[0]);
	//add a new vec3 uniform
	mVec3Uniforms.emplace(std::pair<std::string, glm::vec3*>(name, &value));
}
void BaseShader::setVec3(const std::string& name, float x, float y, float z)
{
	//glUniform3f(glGetUniformLocation(mShaderID, name.c_str()), x, y, z);
	//mVec3Uniforms.insert(std::pair<std::string, std::shared_ptr<glm::vec3>>(name,
	//	std::make_shared<glm::vec3>(glm::vec3(x, y, z))));
}
// ------------------------------------------------------------------------
void BaseShader::setVec4(const std::string& name, glm::vec4& value, bool aSetEveryFrame /*= false*/)
{
	//if aSetEveryFrame is true then all set uniform for this frame
	if (aSetEveryFrame)
	{
		unsigned int loc = glGetUniformLocation(mShaderID, name.c_str());
		glUniform4fv(loc, 1, glm::value_ptr(value));
		return;
	}
	//glUniform4fv(glGetUniformLocation(mShaderID, name.c_str()), 1, &value[0]);
	//add a new vec4 uniform
	mVec4Uniforms.emplace(std::pair<std::string, glm::vec4*>(name, &value));
}
void BaseShader::setVec4(const std::string & name, float x, float y, float z, float w)
{
	//glUniform4f(glGetUniformLocation(mShaderID, name.c_str()), x, y, z, w);
	//mVec2Uniforms.emplace(std::pair<std::string, glm::mat3*>(name, &mat));
}
// ------------------------------------------------------------------------
void BaseShader::setMat2(const std::string& name, glm::mat2& mat)
{
	//glUniformMatrix2fv(glGetUniformLocation(mShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	//add a new mat2 uniform
	mMat2Uniforms.emplace(std::pair<std::string, glm::mat2*>(name, &mat));
}
// ------------------------------------------------------------------------
void BaseShader::setMat3(const std::string& name, glm::mat3& mat)
{
	//glUniformMatrix3fv(glGetUniformLocation(mShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	//add a new vec3 uniform
	mMat3Uniforms.emplace(std::pair<std::string, glm::mat3*>(name, &mat));
// ------------------------------------------------------------------------
}
// ------------------------------------------------------------------------
void BaseShader::setMat4(const std::string& aName, glm::mat4& aMat, bool aSetEveryFrame /*= false*/)
{
	//unsigned int uniformLoc = glGetUniformLocation(mShaderID, name.c_str());
	//glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mat));

	//if aSetEveryFrame is true then all set uniform for this frame
	if (aSetEveryFrame)
	{
		unsigned int loc = glGetUniformLocation(mShaderID, aName.c_str());
		glUniformMatrix4fv(loc, 1, false, glm::value_ptr(aMat));
		return;
	}

	bool add = true;
	//if mat4 is found update it's value
	for(auto& mat4 : mMat4Uniforms)
	{
		if (mat4.first == aName)
		{
			mat4.second = &aMat;
			add = false;
			break;
		}
	}

	if (add)
	{
		//add a new mat4 uniform
		mMat4Uniforms.emplace(std::pair<std::string, glm::mat4*>(aName, &aMat));
	}
}
void BaseShader::setMat4Override(const std::string & aName, glm::mat4 & aMat)
{
	//find uniform with aName
	std::map< std::string, glm::mat4*>::iterator it = mMat4Uniforms.find(aName);
	if (it != mMat4Uniforms.end())
	{
		//if uniform found set it's value to aMat
		mMat4Uniforms[aName] = &aMat;
	}
	else
	{
		mMat4Uniforms.emplace(std::pair<std::string, glm::mat4*>(aName, &aMat));
	}
}

void BaseShader::setBaseRenderer(BaseRenderer* aBaseRenderer)
{
	//set BaseRenderer
	mBaseRenderer = aBaseRenderer;
}

BaseRenderer* BaseShader::getBaseRenderer()
{
	//get pointer to BaseRenderer
	return mBaseRenderer;
}

void BaseShader::setUniforms(unsigned int aProgramID)
{
	//set all unioforms
	//get program
	unsigned int program = aProgramID == -1 ? mShaderID : aProgramID;

	//for though all uniforms store of it's tpye
	for (std::map<std::string, bool*>::iterator it = mBoolUniforms.begin();
		it != mBoolUniforms.end(); ++it)
	{
		//get the uniform location, then set it;s value 
		unsigned int loc = glGetUniformLocation(program, it->first.c_str());
		float v = *it->second;
		glUniform1f(loc, v);
	}

	int index = 0;
	for (std::map<std::string, unsigned int*>::iterator it = mUnsignedIntUniforms.begin();
		it != mUnsignedIntUniforms.end(); ++it)
	{
		glActiveTexture(GL_TEXTURE0 + index);

		unsigned int loc = glGetUniformLocation(program, it->first.c_str());
		glUniform1i(loc, index);

		unsigned int i = *it->second;
		glBindTexture(GL_TEXTURE_2D, i);

		index++;
	}
	for (std::map<std::string, float*>::iterator it = mFloatUniforms.begin();
		it != mFloatUniforms.end(); ++it)
	{
		unsigned int loc = glGetUniformLocation(program, it->first.c_str());
		glUniform1f(loc, *it->second);
	}

	for (std::map<std::string, glm::vec2*>::iterator it = mVec2Uniforms.begin();
		it != mVec2Uniforms.end(); ++it)
	{
		unsigned int loc = glGetUniformLocation(program, it->first.c_str());
		glUniform2fv(loc, 1, glm::value_ptr(*it->second));
	}
	for (std::map<std::string, glm::vec3*>::iterator it = mVec3Uniforms.begin();
		it != mVec3Uniforms.end(); ++it)
	{
		unsigned int loc = glGetUniformLocation(program, it->first.c_str());
		glUniform3fv(loc, 1, glm::value_ptr(*it->second));
	}
	for (std::map<std::string, glm::vec4*>::iterator it = mVec4Uniforms.begin();
		it != mVec4Uniforms.end(); ++it)
	{
		unsigned int loc = glGetUniformLocation(program, it->first.c_str());
		glUniform4fv(loc, 1, glm::value_ptr(*it->second));
	}

	for (std::map<std::string, glm::mat2*>::iterator it = mMat2Uniforms.begin();
		it != mMat2Uniforms.end(); ++it)
	{
		unsigned int loc = glGetUniformLocation(program, it->first.c_str());
		glUniformMatrix2fv(loc, 1, false, glm::value_ptr(*it->second));
	}
	for (std::map<std::string, glm::mat3*>::iterator it = mMat3Uniforms.begin();
		it != mMat3Uniforms.end(); ++it)
	{
		unsigned int loc = glGetUniformLocation(program, it->first.c_str());
		glUniformMatrix3fv(loc, 1, false, glm::value_ptr(*it->second));
	}
	for (std::map<std::string, glm::mat4*>::iterator it = mMat4Uniforms.begin();
		it != mMat4Uniforms.end(); ++it)
	{
		unsigned int loc = glGetUniformLocation(program, it->first.c_str());
		glUniformMatrix4fv(loc, 1, false, glm::value_ptr(*it->second));
	}
}

