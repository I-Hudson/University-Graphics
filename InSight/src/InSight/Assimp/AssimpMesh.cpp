#include "Assimp/AssimpMesh.h"
#include "Log.h"

AssimpMesh::AssimpMesh()
{
}

AssimpMesh::~AssimpMesh()
{
}

AssimpMesh::AssimpMesh(std::vector<glm::vec4> aVertices, std::vector<unsigned int> aIndices, std::vector<glm::vec2> aUV)
{
	std::vector <AssimpVertex> vertices;
	vertices.reserve(aVertices.size());

	for (unsigned int i = 0; i < aVertices.size(); ++i)
	{
		AssimpVertex vertex;
		glm::vec4 vector;

		//position
		vector = aVertices[i];
		vertex.mPosition = vector;

		//colour
		//vector = glm::vec4(1, 1, 1, 1);
		//vertex.mColor = vector;

		//normal
		vector = glm::vec4(1, 1, 1, 1);
		vertex.mNormal = vector;

		//tangent
		//vector = glm::vec4(1, 1, 1, 1);
		//vertex.mTangent = vector;

		//bitangent
		//vector = glm::vec4(1, 1, 1, 1);
		//vertex.mBitangent = vector;

		//uv
		glm::vec2 uv = glm::vec2(aUV[i].x, aUV[i].y);
		vertex.mTexCoord1 = uv;

		vertices.push_back(vertex);

	}

	this->mVertices = vertices;
	this->mIndices = aIndices;

	setupMesh();
}

AssimpMesh::AssimpMesh(std::vector<AssimpVertex> aVertices, std::vector<unsigned int> aIndices, std::vector<AssimpTexture> aTextures)
{
	this->mVertices = aVertices;
	this->mIndices = aIndices;
	this->mTextures = aTextures;

	//set the vertex buffers
	setupMesh();
}

AssimpMesh::AssimpMesh(std::vector<AssimpVertex> aVertices, std::vector<unsigned int> aIndices)
{
	std::vector <AssimpVertex> vertices;
	vertices.reserve(aVertices.size());

	for (unsigned int i = 0; i < aVertices.size(); ++i)
	{
		//push back new vertex
		AssimpVertex vertex;	
		vertex = aVertices[i];
		vertices.push_back(vertex);
	}

	this->mVertices = vertices;
	this->mIndices = aIndices;

	//set the vertex buffers
	setupMesh();
}

void AssimpMesh::Draw(Shader* aShader, const bool& aBindTextures)
{
	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;
	unsigned int normalNr = 0;
	unsigned int heightNr = 0;
	if (aBindTextures)
	{
		for (unsigned int i = 0; i < mTextures.size(); ++i)
		{
			//texture
			glActiveTexture(GL_TEXTURE0 + i);

			std::string number;
			std::string name = mTextures[i].mType;
			if (name == "texture_diffuse")
			{
				number = std::to_string(diffuseNr++);
			}
			else if (name == "texture_specular")
			{
				number = std::to_string(specularNr++);
			}
			else if (name == "texture_normal")
			{
				number = std::to_string(normalNr++);
			}
			else if (name == "texture_height")
			{
				number = std::to_string(heightNr++);
			}

			if (aShader != nullptr)
			{
				if (aShader->getShader() == 17 && name == "texture_specular")
				{
					printf("");
				}
				else
				{
					unsigned int loc = glGetUniformLocation(aShader->getShader(), (name).c_str());
					glUniform1i(loc, i);
					glBindTexture(GL_TEXTURE_2D, mTextures[i].mTextureID);
				}
			}
		}
	}

	//Draw Mesh
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void AssimpMesh::Draw(unsigned int& aProgram, Shader* aShader, const bool& aBindTextures)
{
	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;
	unsigned int normalNr = 0;
	unsigned int heightNr = 0;
	if (aBindTextures)
	{
		for (unsigned int i = 0; i < mTextures.size(); ++i)
		{
			//active proper texture unit before binding
			glActiveTexture(GL_TEXTURE0 + i);

			std::string number;
			std::string name = mTextures[i].mType;
			if (name == "texture_diffuse")
			{
				number = std::to_string(diffuseNr++);
			}
			else if (name == "texture_specular")
			{
				number = std::to_string(specularNr++); // transfer unsigned int to stream
			}
			else if (name == "texture_normal")
			{
				number = std::to_string(normalNr++); // transfer unsigned int to stream
			}
			else if (name == "texture_height")
			{
				number = std::to_string(heightNr++); // transfer unsigned int to stream
			}

			if (aShader != nullptr)
			{
				unsigned int loc = glGetUniformLocation(aProgram, (name).c_str());
				glUniform1i(loc, i);
				glBindTexture(GL_TEXTURE_2D, mTextures[i].mTextureID);
			}
		}
	}

	//Draw Mesh
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void AssimpMesh::Draw(const int& aDrawMode, Shader* aShader, const bool& aBindTextures)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	if (aBindTextures)
	{
		for (unsigned int i = 0; i < mTextures.size(); ++i)
		{
			//active proper texture unit before binding
			glActiveTexture(GL_TEXTURE0 + i);

			std::string number;
			std::string name = mTextures[i].mType;
			if (name == "texture_diffuse")
			{
				number = std::to_string(diffuseNr++);
			}
			else if (name == "texture_specular")
			{
				number = std::to_string(specularNr++); // transfer unsigned int to stream
			}
			else if (name == "texture_normal")
			{
				number = std::to_string(normalNr++); // transfer unsigned int to stream
			}
			else if (name == "texture_height")
			{
				number = std::to_string(heightNr++); // transfer unsigned int to stream
			}

			if (aShader != nullptr)
			{
				//bind texture
				unsigned int loc = glGetUniformLocation(aShader->getShader(), (name).c_str());
				glUniform1i(loc, i);
				glBindTexture(GL_TEXTURE_2D, mTextures[i].mTextureID);
			}
		}
	}

	//Draw Mesh
	glBindVertexArray(mVAO);
	glDrawElements(aDrawMode, (GLsizei)mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void AssimpMesh::setupMesh()
{
	// create buffers/arrays
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mIBO);

	glBindVertexArray(mVAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(AssimpVertex), &mVertices[0], GL_STATIC_DRAW);
	//load data into element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex), ((char*)0));
	
	// vertex colour
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex), ((char *)0) + AssimpVertex::ColourOffset);
	
	// normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex), ((char *)0) + AssimpVertex::NormalOffset);
	
	//tex coord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex), ((char *)0) + AssimpVertex::TexCoord1Offset);

	////tangent
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex), ((char *)0) + AssimpVertex::TangentOffset);
	//
	////bitangent
	//glEnableVertexAttribArray(4);
	//glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex), ((char *)0) + AssimpVertex::BiTangentOffset);


	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void AssimpMesh::unloadMesh()
{
	//delete buffers
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mIBO);

	//delete all textures
	for (unsigned int i = 0; i < mTextures.size(); ++i)
	{
		glDeleteTextures(1, &mTextures[i].mTextureID);
	}
	mTextures.clear();
}
