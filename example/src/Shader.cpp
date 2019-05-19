#include "Shader.h"
#include "Utilities.h"
#include "Log.h"
#include "Entity.h"	

#include "MeshComponent.h"
#include <glm/ext.hpp>

void Shader::setLightVolumeManager(LightVolumeManager * aManager)
{
	mLightVolumes = aManager;
}

Shader::Shader() : 
	mShaderPriority(ShaderPriority::PRI_0)
{
	mGBuffer = nullptr;
	mModeToRender = RenderMode::Opaque;
}

Shader::Shader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath, const char* aGeometryPath, const char* aFragmentPath,
	unsigned int aInputCount /*= 3*/, const char* aInputs[] /*= nullptr*/, unsigned int aOutputCount /*= 1*/, const char* aOutputs[]/*= nullptr*/)
	: mShaderPriority(ShaderPriority::PRI_0)
{
	unsigned int vertex = Utility::loadShader(aVertexPath, GL_VERTEX_SHADER);
	unsigned int control = Utility::loadShader(aControlpath, GL_TESS_CONTROL_SHADER);
	unsigned int evaluation = Utility::loadShader(aEvaluationpath, GL_TESS_EVALUATION_SHADER);
	unsigned int geometry = Utility::loadShader(aGeometryPath, GL_GEOMETRY_SHADER);
	unsigned int fragment = Utility::loadShader(aFragmentPath, GL_FRAGMENT_SHADER);
	
	//const char* szInputs[] = { "Position", "Color", "Normal", "Tangnet", "Bitangent", "TexCoord1" };
	const char* szInputs[] = { "Position", "Normal", "TexCoord1" };
	const char* szOutputs[] = { "FragColour" };

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
	
	glDeleteShader(vertex);
	glDeleteShader(control);
	glDeleteShader(evaluation);
	glDeleteShader(geometry);
 	glDeleteShader(fragment);

	//reserve splace for the vector
	mMeshes.reserve(16);

	mGBuffer = nullptr;
	mModeToRender = RenderMode::Opaque;
}

Shader::~Shader()
{
	glDeleteProgram(mShaderID);
	glDeleteFramebuffers(1, &mFrameBuffer);

	for (auto& t : mFrameBufferTextures)
	{
		glDeleteTextures(1, &t.mTexture);
	}
	mFrameBufferTextures.clear();

	if (!mGBuffer->hasBeenDeleted() && mGBuffer != nullptr)
	{
		mGBuffer->destroy();
		delete mGBuffer;
	}

	mMeshComponent = nullptr;
	mMeshes.clear();

	EN_TRACE("Shader Has Been Deleted");
}

void Shader::setAttributes()
{
}

void Shader::addMesh(MeshComponent* aMesh)
{
	bool found = false;
	short insertLocation = 0;
	for(int i = 0; i < mMeshes.size(); ++i)
	{
		//if we find the the mesh we want to add
		//then break
		if (mMeshes[i] == aMesh)
		{
			found = true;
			break;
		}
		glm::vec3 newMeshVec3 = *aMesh->entity->getComponent<TransformComponent>().getPosition();
		float newMeshDist = glm::distance(newMeshVec3, glm::vec3(0, 0, 0));
		glm::vec3 currentMeshVec3 = *mMeshes[i]->entity->getComponent<TransformComponent>().getPosition();
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

void Shader::removeMesh(MeshComponent* aMesh)
{
	if (std::find(mMeshes.begin(), mMeshes.end(), aMesh) != mMeshes.end())
	{
		//if aMesh is not found add it
		//mMeshes.(aMesh);
	}
}

bool compareDistance(MeshComponent* aM1, MeshComponent* aM2)
{
	glm::vec3 newMeshVec3 = *aM1->entity->getComponent<TransformComponent>().getPosition();
	float newMeshDist = glm::distance(newMeshVec3, glm::vec3(0, 0, 0));
	glm::vec3 currentMeshVec3 = *aM2->entity->getComponent<TransformComponent>().getPosition();
	float currentMeshDist = glm::distance(currentMeshVec3, glm::vec3(0, 0, 0));

	return newMeshDist < currentMeshDist;
}

void Shader::sortMeshOrder()
{
	std::sort(mMeshes.begin(), mMeshes.end(), compareDistance);
}

void Shader::setDrawBuffers()
{
	glDrawBuffers(drawBuffers.size(), drawBuffers.data());
}

void Shader::drawMeshes(const bool& aBindTextures, const glm::mat4& aModelMatrx)
{
	for (unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i]->draw(this, aModelMatrx, aBindTextures);
	}
}

void Shader::drawMeshes(std::vector<MeshComponent*>& aMeshes, const bool& aBindTextures, const glm::mat4& aModelMatrx)
{
	for (unsigned int i = 0; i < aMeshes.size(); ++i)
	{
		if (aMeshes[i]->getRenderMode() == mModeToRender)
		{
			aMeshes[i]->draw(this, aModelMatrx, aBindTextures);
		}
	}
}

void Shader::gui()
{
	if (ImGui::CollapsingHeader("Shader"))
	{

	}
}

void Shader::useShader(bool aClear, const bool& aActiveTextures, Entity* aE)
{
	if (aClear)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glUseProgram(mShaderID);

	setUniforms();

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

void Shader::useShader(bool aClear /* = false */, const bool& aActiveTextures /*= true */)
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

	if (mGBuffer != nullptr)
	{
		mGBuffer->bindBuffer();
	}
	else
	{
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	if (aClear)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glUseProgram(mShaderID);

	setUniforms();

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

void Shader::useShader(bool aClear, const int& aDrawMode)
{
	if (aClear)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glUseProgram(mShaderID);

	setUniforms();

	//std::vector<MeshComponent*> allMeshes = mMeshComponent->entity->getEntityManger()->getAllEntityComponents<MeshComponent>();
	for (unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		if (mMeshes[i]->getRenderMode() == mModeToRender)
		{
			mMeshes[i]->draw(aDrawMode, this);
		}
	}

	glUseProgram(0);
}

void Shader::useShader(bool aClear /* = false */)
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

	if (aClear)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glUseProgram(mShaderID);

	setUniforms();

	//std::vector<MeshComponent*> allMeshes = mMeshComponent->entity->getEntityManger()->getAllEntityComponents<MeshComponent>();
	for (unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		if (mMeshes[i]->getRenderMode() == mModeToRender)
		{
			mMeshes[i]->draw(this);
		}
	}

	glUseProgram(0);
}

void Shader::useShaderPP(bool aClear)
{
	if (aClear)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glUseProgram(mShaderID);

	setUniforms();
}

void Shader::createFrameBuffer()
{
	glGenFramebuffers(1, &mFrameBuffer);
}

unsigned int Shader::attachTextureToFrameBuffer(int aLevel, int aInternalFormat,
										int aWidth, int aHeight, 
										int aBoarder, GLenum aFormat, 
										GLenum aType, const void* aPixels,
										GLenum aAttachment)
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

	unsigned int fbo;

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

	mFrameBufferTextures.push_back(fbTexture);

	if (aAttachment != GL_DEPTH_STENCIL_ATTACHMENT &&
		aAttachment != GL_COLOR_ATTACHMENT4 && aAttachment != GL_DEPTH_ATTACHMENT)
	{
		drawBuffers.push_back(aAttachment);
		glDrawBuffers(drawBuffers.size(), drawBuffers.data());
		//setDrawBuffers();
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{

		printf("Framebuffer Error\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return fbo;
}

void Shader::setMeshComponent(MeshComponent* aMeshComponent)
{
	mMeshComponent = aMeshComponent;
	mMeshes.push_back(aMeshComponent);
}

MeshComponent* Shader::getMeshComponent()
{
	return mMeshComponent;
}

void Shader::setGBuffer(GBuffer * aGBuffer)
{
	mGBuffer = aGBuffer;
}

GBuffer* Shader::getGBuffer()
{
	return mGBuffer;
}

void Shader::setShaderPriority(ShaderPriority a_shaderPriority /*= ShaderPriority::PRI_5*/)
{
	mShaderPriority = a_shaderPriority;
}

ShaderPriority Shader::getShaderPriority()
{
	return mShaderPriority;
}

unsigned int* Shader::getTexture(int aIndex)
{
	if (mFrameBufferTextures.size() > 0)
	{
		return &mFrameBufferTextures[aIndex].mTexture;
	}
	else
	{
		return 0;
	}
}

unsigned int Shader::numOfTexturesAttached() const
{
	return mFrameBufferTextures.size();
}

std::vector<FBTexture>* Shader::getAllTextures()
{
	return &mFrameBufferTextures;
}

void Shader::setBool(const std::string& name, bool& aValue)
{
	//glUniform1i(glGetUniformLocation(mShaderID, name.c_str()), (int)value);
	mBoolUniforms.emplace(std::pair<std::string, bool*>(name, &aValue));
}
// ------------------------------------------------------------------------
void Shader::setUnsignedIntUniforms(const std::string &aName, unsigned int& aValue)
{
	//glUniform1i(glGetUniformLocation(mShaderID, name.c_str()), value);
	mUnsignedIntUniforms.emplace(std::pair<std::string, unsigned int*>(aName, &aValue));
}
void Shader::setUnsignedIntUniformsOverride(const std::string& aName, unsigned int& aValue)
{
	std::map< std::string, unsigned int*>::iterator it = mUnsignedIntUniforms.find(aName);
	if (it != mUnsignedIntUniforms.end())
	{
		mUnsignedIntUniforms[aName] = &aValue;
	}
	else
	{
		mUnsignedIntUniforms.emplace(std::pair<std::string, unsigned int*>(aName, &aValue));
	}
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string& name, float& value, bool aSetEveryFrame /*= false*/)
{
	if (aSetEveryFrame)
	{
		unsigned int loc = glGetUniformLocation(mShaderID, name.c_str());
		glUniform1f(loc, value);
		return;
	}
	//glUniform1f(glGetUniformLocation(mShaderID, name.c_str()), value);
	mFloatUniforms.emplace(std::pair<std::string, float*>(name, &value));
}
void Shader::setFloatOverride(const std::string & aName, float & aValue)
{
	std::map< std::string, float*>::iterator it = mFloatUniforms.find(aName);
	if (it != mFloatUniforms.end())
	{
		mFloatUniforms[aName] = &aValue;
	}
	else
	{
		mFloatUniforms.emplace(std::pair<std::string, float*>(aName, &aValue));
	}
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string& name, glm::vec2& value)
{
	//glUniform2fv(glGetUniformLocation(mShaderID, name.c_str()), 1, &value[0]);
	mVec2Uniforms.emplace(std::pair<std::string, glm::vec2*>(name, &value));
}
void Shader::setVec2(const std::string& name, float x, float y)
{
	//glUniform2f(glGetUniformLocation(mShaderID, name.c_str()), x, y);
	//mVec2Uniforms.insert(std::pair<std::string, std::shared_ptr<glm::vec2>>(name, 
	//	std::make_shared<glm::vec2>(glm::vec2(x,y))));
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string& name, glm::vec3& value)
{
	//glUniform3fv(glGetUniformLocation(mShaderID, name.c_str()), 1, &value[0]);
	mVec3Uniforms.emplace(std::pair<std::string, glm::vec3*>(name, &value));
}
void Shader::setVec3(const std::string& name, float x, float y, float z)
{
	//glUniform3f(glGetUniformLocation(mShaderID, name.c_str()), x, y, z);
	//mVec3Uniforms.insert(std::pair<std::string, std::shared_ptr<glm::vec3>>(name,
	//	std::make_shared<glm::vec3>(glm::vec3(x, y, z))));
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string& name, glm::vec4& value, bool aSetEveryFrame /*= false*/)
{
	if (aSetEveryFrame)
	{
		unsigned int loc = glGetUniformLocation(mShaderID, name.c_str());
		glUniform4fv(loc, 1, glm::value_ptr(value));
		return;
	}
	//glUniform4fv(glGetUniformLocation(mShaderID, name.c_str()), 1, &value[0]);
	mVec4Uniforms.emplace(std::pair<std::string, glm::vec4*>(name, &value));
}
void Shader::setVec4(const std::string & name, float x, float y, float z, float w)
{
	//glUniform4f(glGetUniformLocation(mShaderID, name.c_str()), x, y, z, w);
	//mVec2Uniforms.emplace(std::pair<std::string, glm::mat3*>(name, &mat));
}

// ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, glm::mat2& mat)
{
	//glUniformMatrix2fv(glGetUniformLocation(mShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	mMat2Uniforms.emplace(std::pair<std::string, glm::mat2*>(name, &mat));
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, glm::mat3& mat)
{
	//glUniformMatrix3fv(glGetUniformLocation(mShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	mMat3Uniforms.emplace(std::pair<std::string, glm::mat3*>(name, &mat));
// ------------------------------------------------------------------------
}
void Shader::setMat4(const std::string& aName, glm::mat4& aMat, bool aSetEveryFrame /*= false*/)
{
	//unsigned int uniformLoc = glGetUniformLocation(mShaderID, name.c_str());
	//glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mat));

	if (aSetEveryFrame)
	{
		unsigned int loc = glGetUniformLocation(mShaderID, aName.c_str());
		glUniformMatrix4fv(loc, 1, false, glm::value_ptr(aMat));
		return;
	}

	bool add = true;
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
		mMat4Uniforms.emplace(std::pair<std::string, glm::mat4*>(aName, &aMat));
	}
}

void Shader::setMat4Override(const std::string & aName, glm::mat4 & aMat)
{
	std::map< std::string, glm::mat4*>::iterator it = mMat4Uniforms.find(aName);
	if (it != mMat4Uniforms.end())
	{
		mMat4Uniforms[aName] = &aMat;
	}
	else
	{
		mMat4Uniforms.emplace(std::pair<std::string, glm::mat4*>(aName, &aMat));
	}
}

void Shader::setUniforms(unsigned int aProgramID)
{
	unsigned int program = aProgramID == -1 ? mShaderID : aProgramID;

	for (std::map<std::string, bool*>::iterator it = mBoolUniforms.begin();
		it != mBoolUniforms.end(); ++it)
	{
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

