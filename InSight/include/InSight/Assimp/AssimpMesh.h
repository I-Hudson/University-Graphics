#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader/Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

struct AssimpVertex
{
public:
	//position
	glm::vec4 mPosition;
	//colour
	//glm::vec4 mColor;
	//normal
	glm::vec4 mNormal;
	//tangent
	//glm::vec4 mTangent;
	//bitangnet
	//glm::vec4 mBitangent;
	//texCoords
	glm::vec2 mTexCoord1;

	enum Offsets
	{
		PositionOffset = 0,										//0
		//ColourOffset = PositionOffset + sizeof(glm::vec4),
		NormalOffset = PositionOffset + sizeof(glm::vec4),		//16
		TexCoord1Offset = NormalOffset + sizeof(glm::vec4),		//32
		//TangentOffset = TexCoord1Offset + sizeof(glm::vec2),	//40
		//BiTangentOffset = TangentOffset + sizeof(glm::vec4),	//56
	};
};

struct AssimpTexture
{
	unsigned int mTextureID;
	std::string mType;
	std::string mPath;
};

class AssimpMesh
{
public:
	/*
		MeshData
	*/
	std::vector<AssimpVertex> mVertices;
	std::vector<unsigned int> mIndices;
	std::vector<AssimpTexture> mTextures;
	unsigned int mVAO;

	/*
		Functions
	*/
	AssimpMesh();
	~AssimpMesh();

	AssimpMesh(std::vector<glm::vec4> aVertices, std::vector<unsigned int> aIndices, std::vector<glm::vec2> aUV);
	AssimpMesh(std::vector<AssimpVertex> aVertices, std::vector<unsigned int> aIndices, std::vector<AssimpTexture> aTextures);
	AssimpMesh(std::vector<AssimpVertex> aVertices, std::vector<unsigned int> aIndices);

	//Draw the mesh
	void Draw(Shader* aShader = nullptr, const bool& aBindTextures = true);
	void Draw(unsigned int& aProgram, Shader* aShader = nullptr, const bool& aBindTextures = true);
	void Draw(const int& aDrawMode, Shader* aShader = nullptr, const bool& aBindTextures = true);
	//unload the mesh
	void unloadMesh();

private:
	/*
		Render Data
	*/
	unsigned int mVBO;
	unsigned int mIBO;

	/*
		Functions
	*/
	void setupMesh();
};