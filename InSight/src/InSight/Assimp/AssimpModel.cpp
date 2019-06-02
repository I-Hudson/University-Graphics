#include "Assimp/AssimpModel.h"
#include "Log.h"

//#include <thread>

AssimpModel::~AssimpModel()
{
}

AssimpModel::AssimpModel(const char* aPath)
{
	//std::thread t1(&AssimpModel::loadModel, aPath);
	//load model
	loadModel(aPath);
	//t1.join();
}

AssimpModel::AssimpModel(AssimpMesh aPath)
{
	//add a new mesh
	mMeshes.push_back(aPath);
}

AssimpModel::AssimpModel(std::vector<AssimpVertex> aVertices, std::vector<unsigned int> aIndices)
{
	//add a new mesh
	AssimpMesh mesh(aVertices, aIndices);
	mMeshes.push_back(mesh);
}

void AssimpModel::Draw(Shader* aShader, const bool& aBindTextures)
{
	//draw all meshes
	for (unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i].Draw(aShader, aBindTextures);
	}
}

void AssimpModel::Draw(unsigned int & aProgram, Shader * aShader, const bool & aBindTextures)
{
	//draw all meshes

	for (unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i].Draw(aProgram, aShader, aBindTextures);
	}
}

void AssimpModel::Draw(const int& aDrawMode, Shader* aShader, const bool& aBindTextures)
{
	//draw all meshes
	for (unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i].Draw(aDrawMode, aShader, aBindTextures);
	}
}

void AssimpModel::Destroy()
{
	//destroy all meshes
	for (unsigned int i = 0; i < mMeshes.size(); i++)
	{
		mMeshes[i].unloadMesh();

	}
	EN_TRACE("Model Has Been Unloaded");
}

std::string* AssimpModel::getModelFileName()
{
	//get model file name
	return &mDirectory;
}

void AssimpModel::loadModel(const char* aPath)
{
	//get the assimp importer, load the model from file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(aPath, aiProcess_Triangulate |
													aiProcess_FlipUVs | 
													aiProcess_CalcTangentSpace |
													aiProcess_GenUVCoords);

	if (!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	mDirectory = std::string(aPath).substr(0, std::string(aPath).find_last_of('/'));
	//process scene root node
	processNode(scene->mRootNode, scene);
}

void AssimpModel::processNode(aiNode* aNode, const aiScene* aScene)
{
	//process the node's meshes 
	for (unsigned int i = 0; i < aNode->mNumMeshes; i++)
	{
		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
		mMeshes.push_back(processMesh(mesh, aScene));
	}
	//process the node's children 
	for (unsigned int i = 0; i < aNode->mNumChildren; i++)
	{
		processNode(aNode->mChildren[i], aScene);
	}
}

AssimpMesh AssimpModel::processMesh(aiMesh* aMesh, const aiScene* aScene)
{
	std::vector<AssimpVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<AssimpTexture> textures;

	/*
		Vertices
	*/
	for (unsigned int i = 0; i < aMesh->mNumVertices; ++i)
	{
		AssimpVertex vertex;

		//process vertex
		glm::vec4 position;
		position.x = aMesh->mVertices[i].x;
		position.y = aMesh->mVertices[i].y;
		position.z = aMesh->mVertices[i].z;
		position.w = 1;
		vertex.mPosition = position;

		//glm::vec4 color = glm::vec4();
		//color.r = 1.0f;
		//color.g = 1.0f;
		//color.b = 1.0f;
		//color.a = 1.0f;
		//vertex.mColor = color;

		glm::vec4 normal = glm::vec4();
		if (aMesh->mNormals != nullptr)
		{
			normal.x = aMesh->mNormals[i].x;
			normal.y = aMesh->mNormals[i].y;
			normal.z = aMesh->mNormals[i].z;
			normal.w = 0;
		}
		vertex.mNormal = normal;

		//glm::vec4 tangent = glm::vec4();
		//if(aMesh->mTangents != nullptr)
		//{
		//	tangent.x = aMesh->mTangents[i].x;
		//	tangent.y = aMesh->mTangents[i].y;
		//	tangent.z = aMesh->mTangents[i].z;
		//	tangent.w = 0;
		//}
		//vertex.mTangent = tangent;
		//
		//glm::vec4 bitangent = glm::vec4();
		//if (aMesh->mBitangents != nullptr)
		//{
		//	bitangent.x = aMesh->mBitangents[i].x;
		//	bitangent.y = aMesh->mBitangents[i].y;
		//	bitangent.z = aMesh->mBitangents[i].z;
		//	bitangent.w = 0;
		//}
		//vertex.mBitangent = bitangent;

		if (aMesh->mTextureCoords[0])
		{
			glm::vec2 texCoord;
			texCoord.x = aMesh->mTextureCoords[0][i].x;
			texCoord.y = aMesh->mTextureCoords[0][i].y;
			vertex.mTexCoord1 = texCoord;
		}
		else
		{
			vertex.mTexCoord1 = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	/*
		Indices
	*/
	for (unsigned int i = 0; i < aMesh->mNumFaces; ++i)
	{
		aiFace face = aMesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	/*
		Material
	*/
	if (aMesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = aScene->mMaterials[aMesh->mMaterialIndex];
		std::vector<AssimpTexture> diffuseMaps = loadMaterialTextures(material, 
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<AssimpTexture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<AssimpTexture> normalMaps = loadMaterialTextures(material,
			aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<AssimpTexture> heightMap = loadMaterialTextures(material,
			aiTextureType_HEIGHT, "texture_height");
		textures.insert(textures.end(), heightMap.begin(), heightMap.end());
	}

	return AssimpMesh(vertices, indices, textures);
}

std::vector<AssimpTexture> AssimpModel::loadMaterialTextures(aiMaterial* aMaterial, aiTextureType aType, std::string aName)
{
	//load a texture
	std::vector<AssimpTexture> textures;
	for (unsigned int i = 0; i < aMaterial->GetTextureCount(aType); ++i)
	{
		aiString str;
		//get texture index and string
		aMaterial->GetTexture(aType, i, &str);
		bool skip = false;
		//if we have already loaded the texture, push it back and set skip to true
		for (unsigned int j = 0; j < loadedTextures.size(); ++j)
		{
			if (std::strcmp(loadedTextures[j].mPath.data(), str.C_Str()) == 0)
			{
				loadedTextures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}
		//if texture has been loaded skip
		if (!skip)
		{
			//load texture from file
			AssimpTexture texture;
			texture.mTextureID = TextureFromFile(str.C_Str(), mDirectory);
			texture.mType = aName;
			texture.mPath = str.C_Str();
			textures.push_back(texture);
		}
	}
	return textures;
}

unsigned int AssimpModel::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	//texture filename
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	//gen texture
	unsigned int textureID;
	glGenTextures(1, &textureID);

	//load texture with stbi
	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		//texture format
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		//bind and set texture
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
