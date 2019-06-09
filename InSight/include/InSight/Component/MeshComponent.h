#pragma once

#ifndef __MESH_Component_H__
#define __MESH_Component_H__

#include "Entity/ECS.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Assimp/AssimpModel.h"
#include "Shader/BaseShader.h"
#include "Component/TransformComponent.h"

//Which loader to use
enum LoadType
{
	AssimpLoader,
	FBXLoader
};

//
//enum ShaderAttrib
//{
//	Position,
//	Color,
//	Normal,
//	Tangent,
//	Bitangent,
//	TexCoord1
//};

class MeshComponent : public Component
{
public:
	//Constructors
	MeshComponent() = default;
	MeshComponent(const char* aFile, LoadType aModelType);
	MeshComponent(AssimpModel* aModel);
	
	//Destructor 
	~MeshComponent();
	
	//Draw this mesh
	void		draw(BaseShader* aShader, const glm::mat4& aModelMatrx = glm::mat4(), const bool& aBindTextures = true) override;
	//Draw this mesh
	void		draw(BaseShader* aShader, unsigned int& aProgram, const glm::mat4& aModelMatrx = glm::mat4(), const bool& aBindTextures = true);
	//Draw this mesh
	void		draw(const int& aDrawMode, BaseShader* aShader, const glm::mat4& aModelMatrx = glm::mat4(), const bool& aBindTextures = true);
	//Draw this mesh's GUI
	void		gui() override;
	//Destroy this component
	void		destroy() override;

	//setMesh
	void		setMesh();

	//set this mesh's render mode
	void		setRenderMode(const RenderMode& aRenderMode);
	//get this mesh's render mode
	RenderMode&	getRenderMode();

	//Set mShader
	void		addShader(BaseShader* aShader);

	//Return this mesh's BaseShader as T is possible 
	template<typename T>
	T*			 getShader()
	{
		//cast BaseShader to T*
		T* tempT =  dynamic_cast<T*>(mShader);
		//If cast was successful
		if (tempT != nullptr)
		{
			//return BaseShader csated to T*
			return dynamic_cast<T*>(mShader);
		}
		//cast not successful. Return nullptr
		return nullptr;

		//for (auto& s : mShaders)
		//{
		//	T* tempT = dynamic_cast<T*>(s);
		//	if (tempT != nullptr)
		//	{
		//		return dynamic_cast<T*>(s);
		//	}
		//}
		//return nullptr;
	}

	//Create a plane for this mesh comonent
	void			addPlane(const int& aWidth, const int& aLength);

private:

	/*
		VAO, VBO, IBO for meshes created by 
		this meshcomponent
	*/
	//vao
	//unsigned int mVAO;
	//vbo
	//unsigned int mVBO;
	//ibo
	//unsigned int mIBO;
	//unsigned int indicsSize;

	//Meshes render mode. Is this object opaque or transparent
	RenderMode mRenderMode;

	//BaseShader to use with this mesh
	BaseShader* mShader;
	//std::vector<BaseShader*> mShaders;

	//transform pointer 
	TransformComponent* mTransform;
	//Normal matrix
	glm::mat4 mNormalMatrix;
	//model pointer
	AssimpModel* model;

};

#endif // __MESH_Component_H__
