#pragma once

#include "AssimpMesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class AssimpModel
{
public:
	/*
		Functions
	*/
	AssimpModel() {}
	~AssimpModel();
	AssimpModel(const char* aPath);
	AssimpModel(AssimpMesh aPath);
	AssimpModel(std::vector<AssimpVertex> aVertices, std::vector<unsigned int> aIndices);

	void Draw(Shader* aShader = nullptr, const bool& aBindTextures = true);
	void Draw(unsigned int& aProgram, Shader* aShader = nullptr, const bool& aBindTextures = true);
	void Draw(const int& aDrawMode, Shader* aShader = nullptr, const bool& aBindTextures = true);

	void Destroy();
	int getMeshCount() const { return (int)mMeshes.size(); }
	AssimpMesh getMeshIndex(int a_i) const { return mMeshes[a_i]; }

	std::string* getModelFileName();

private:
	/*
		Model Data
	*/
	std::vector<AssimpMesh> mMeshes;
	std::string mDirectory;
	std::vector<AssimpTexture> loadedTextures;
	std::string mModelName;

	/*
		Functions
	*/
	void loadModel(const char* aPath);
	void processNode(aiNode* aNode, const aiScene* aScene);
	AssimpMesh processMesh(aiMesh* aMesh, const aiScene* aScene);
	std::vector<AssimpTexture> loadMaterialTextures(aiMaterial* aMaterial, aiTextureType aType,
		std::string aName);

	unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
};