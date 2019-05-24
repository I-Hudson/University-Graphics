#include "MeshComponent.h"
#include "FBXFile.h"
#include "Utilities.h"
#include "Log.h"
#include "CameraComponent.h"

MeshComponent::MeshComponent(const char* aFile, LoadType aLoadType)
{
	if (aLoadType == LoadType::AssimpLoader)
	{
		//load new model
		model = new AssimpModel(aFile);
		//set render mode
		mRenderMode = RenderMode::Opaque;
	}
	else if (aLoadType == LoadType::FBXLoader)
	{
		////fbxFile->load("./models/stanford/Dragon.fbx", FBXFile::UNITS_DECIMETER);

		//std::vector<glm::vec4> vertices;
		//std::vector<unsigned int> indices;

		////for (unsigned int i = 0; i < fbxFile->getMeshCount(); i++)
		////{
		////	for (unsigned int j = 0; j < fbxFile->getMeshByIndex(i)->m_vertices.size(); j++)
		////	{
		////		vertices.push_back(fbxFile->getMeshByIndex(i)->m_vertices[j].position);
		////	}
		////
		////	for (unsigned int j = 0; j < fbxFile->getMeshByIndex(i)->m_indices.size(); j++)
		////	{
		////		indices.push_back(fbxFile->getMeshByIndex(i)->m_indices[j]);
		////	}
		////}

		///*
		//	Create basic shaders
		//*/
		//int success = GL_FALSE;

		//const char* vertexShader = "#version 150\n \
		//			 in vec4 Position; \
		//			 in vec4 Colour; \
		//			 out vec4 vColour; \
		//			 uniform mat4 ProjectionView; \
		//			 void main() { vColour = Colour; gl_Position = ProjectionView * Position; }";

		//unsigned int m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
		//glShaderSource(m_vertexShader, 1, (const char**)&vertexShader, 0);
		//glCompileShader(m_vertexShader);
		//Utility::TestShaderStatus(m_vertexShader, GL_COMPILE_STATUS, success, "Vertex");
		//if (!success)
		//{
		//	std::cout << "Basic Vertex shader has not compiled" << std::endl;
		//}

		//const char* fragmentShader = "#version 150\n \
		//			 in vec4 vColour; \
		//			 out vec4 FragColor; \
		//			 void main()	{ FragColor = vColour; }";

		//unsigned int m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		//glShaderSource(m_fragmentShader, 1, (const char**)&fragmentShader, 0);
		//glCompileShader(m_fragmentShader);
		//Utility::TestShaderStatus(m_fragmentShader, GL_COMPILE_STATUS, success, "Fragment");
		//if (!success)
		//{
		//	std::cout << "Basic Fragement shader has not compiled" << std::endl;
		//}

		///*
		//	Create our basic program
		//*/
		//const char* szInputs[] = { "Position", "Colour" };
		//const char* szOutputs[] = { "FragColor" };
		////bind the shaders to create our shader program
		//mProgrmID = Utility::createProgram(
		//	m_vertexShader,
		//	0,
		//	0,
		//	0,
		//	m_fragmentShader,
		//	2, szInputs, 1, szOutputs);

		//glDeleteShader(m_fragmentShader);
		//glDeleteShader(m_vertexShader);

		//glGenBuffers(1, &mVBO);
		//glGenBuffers(1, &mIBO);
		//glGenVertexArrays(1, &mVAO);
		//
		//// OPENGL: Bind  VAO, and then bind the VBO and IBO to the VAO
		//glBindVertexArray(mVAO);
		//glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
		//
		//// enable the attribute locations that will be used on our shaders
		//glEnableVertexAttribArray(0); //Pos
		//glEnableVertexAttribArray(1); //Col
		//
		//// tell our shaders where the information within our buffers lie
		////glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), ((char *)0) + FBXVertex::PositionOffset);
		////glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), ((char *)0) + FBXVertex::ColourOffset);
		//
		//// finally, where done describing our mesh to the shader
		//// we can describe the next mesh
		//glBindVertexArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

MeshComponent::MeshComponent(AssimpModel* aModel)
{
	//set model
	model = aModel;
}

MeshComponent::~MeshComponent()
{
	//delete model;
}

void MeshComponent::draw(Shader* aShader, const glm::mat4& aModelMatrx, const bool& aBindTextures)
{
	//bing our shader program
	//glUseProgram(mProgrmID);
	//bind our vertex array object
	//glBindVertexArray(mVAO);
	//
	//glDrawElements(GL_TRIANGLES, indicsSize, GL_UNSIGNED_INT, 0);

	//check for transform. If null get transform from entity
	if (mTransform == nullptr)
	{
		if (entity->hasComponent<TransformComponent>())
		{
			mTransform = entity->getComponent<TransformComponent>();
		}
		else
		{
			EN_ERROR("MeshComponent is trying to access a TransformComponent. No TransformComponent exists. \n\
				  Check that all Entiy's with a MeshComponent have a TransformComponent");
			return;
		}
	}
	//if aModelMatrx is indentiy, pass transform 
	if (aModelMatrx == glm::mat4())
	{
		aShader->setMat4("model", *mTransform->getTransformMatrix(), true);
	}
	else
	{
		//*  aModelMatrx and transform
		aShader->setMat4("model", aModelMatrx * *mTransform->getTransformMatrix(), true);
	}
	//normal matrix
	glm::mat4 cameraView = *entity->getEntityManger()->getEntity("Main Camera")->getComponent<CameraComponent>()->getViewMatrix();
	glm::mat4 normalMat = glm::transpose(glm::inverse(cameraView * *mTransform->getTransformMatrix()));
	aShader->setMat4("NormalMatrix", normalMat, true);

	//draw
	model->Draw(aShader, aBindTextures);
}

void MeshComponent::draw(Shader* aShader, unsigned int& aProgram, const glm::mat4 & aModelMatrx, const bool & aBindTextures)
{
	//check for transform. If null get transform from entity
	if (mTransform == nullptr)
	{
		if (entity->hasComponent<TransformComponent>())
		{
			mTransform = entity->getComponent<TransformComponent>();
		}
		else
		{
			EN_ERROR("MeshComponent is trying to access a TransformComponent. No TransformComponent exists. \n\
				  Check that all Entiy's with a MeshComponent have a TransformComponent");
			return;
		}
	}
	//if aModelMatrx is indentiy, pass transform 
	if (aModelMatrx == glm::mat4())
	{
		unsigned int loc = glGetUniformLocation(aProgram, "model");
		glUniformMatrix4fv(loc, 1, false, glm::value_ptr(*mTransform->getTransformMatrix()));
	}
	else
	{
		//*  aModelMatrx and transform
		unsigned int loc = glGetUniformLocation(aProgram, "model");
		glUniformMatrix4fv(loc, 1, false, glm::value_ptr(aModelMatrx * *mTransform->getTransformMatrix()));
	}
	//normal matrix
	glm::mat4 cameraView = *entity->getEntityManger()->getEntity("Main Camera")->getComponent<CameraComponent>()->getViewMatrix();
	aShader->setMat4("normalMatrix", glm::transpose(glm::inverse(cameraView * *mTransform->getTransformMatrix())), true);
	//mNormalMatrix = glm::transpose(glm::inverse(*mTransform->getTransformMatrix()));

	//draw
	model->Draw(aProgram, aShader, aBindTextures);
}

void MeshComponent::draw(const int& aDrawMode, Shader* aShader, const glm::mat4& aModelMatrx, const bool& aBindTextures)
{
	//check for transform. If null get transform from entity
	if (mTransform == nullptr)
	{
		if (entity->hasComponent<TransformComponent>())
		{
			mTransform = entity->getComponent<TransformComponent>();
		}
		else
		{
			EN_ERROR("MeshComponent is trying to access a TransformComponent. No TransformComponent exists. \n\
				  Check that all Entiy's with a MeshComponent have a TransformComponent");
			return;
		}
	}
	//if aModelMatrx is indentiy, pass transform 
	if (aModelMatrx == glm::mat4())
	{
		aShader->setMat4("model", *mTransform->getTransformMatrix(), true);
	}
	else
	{
		//*  aModelMatrx and transform
		aShader->setMat4("model", aModelMatrx * *mTransform->getTransformMatrix(), true);
	}
	//normal matrix
	glm::mat4 cameraView = *entity->getEntityManger()->getEntity("Main Camera")->getComponent<CameraComponent>()->getViewMatrix();
	aShader->setMat4("normalMatrix", glm::transpose(glm::inverse(cameraView * *mTransform->getTransformMatrix())), true);

	model->Draw(aDrawMode, aShader, aBindTextures);
}

void MeshComponent::gui()
{
	if (ImGui::CollapsingHeader("Model Component"))
	{
		ImGui::Text(std::string("Model Path :" + *model->getModelFileName()).c_str());

		if(ImGui::CollapsingHeader("Shader Properties"))
		{
			mShader->gui();
			/*for (auto& s : mShaders)
			{
				if (ImGui::CollapsingHeader(std::string(typeid(*s).name()).c_str()))
				{
					s->gui();
				}
			}*/
		}
	}
}

void MeshComponent::destroy()
{
	//destroy model
	model->Destroy();
	delete model;
}

void MeshComponent::setMesh()
{
}

void MeshComponent::setRenderMode(const RenderMode & aRenderMode)
{
	//set render mode
	mRenderMode = aRenderMode;
}

RenderMode& MeshComponent::getRenderMode()
{
	//get render mode
	return mRenderMode;
}

void MeshComponent::addShader(Shader* aShader)
{
	//add shader
	mShader = aShader;
	mShader->setMeshComponent(this);
	//check if shader it good
	/*mShaders.push_back(aShader);
	aShader->addMesh(this);*/
	//if it is then allow

	//other wise debug message 
}

float RandomFloat(float a_min, float a_max)
{
	//get a random float
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = a_max - a_min;
	float r = random * diff;
	return a_min + r;
}

void MeshComponent::addPlane(const int& aWidth, const int& aLength)
{
	//make plane
	//create a new Vertex array
	std::vector<AssimpVertex> m_vertices;

	//wave size x and y
	unsigned int size = aWidth;

	//setup the vertices
	for (int y = 0; y < aLength; ++y)
	{
		for (int x = 0; x < aWidth; ++x)
		{
			AssimpVertex vertex;
			if (y != 0)
			{
				vertex.mPosition = glm::vec4(-(aWidth * 0.5f) + x, RandomFloat(0.0f, 0.0f), -(aLength * 0.5f) + y, 1);
			}
			else
			{
				vertex.mPosition = glm::vec4(-(aWidth * 0.5f) + x, 0, -(aLength * 0.5f) + y, 1);
			}
			//set normal and uvs
			vertex.mNormal = glm::vec4(0, 1, 0, 0);
			glm::vec2 uv = glm::vec2(0, 0);
			float xF = (float)x;
			float yF = (float)y;
			float widthF = (float)aWidth;
			float lengthF = (float)aLength;

			if (x != 0)
			{
				uv.x = xF / aWidth;
			}
			if (y != 0)
			{
				uv.y = yF / aLength;
			}
			vertex.mTexCoord1 = uv;

			m_vertices.push_back(vertex);
		}
	}

	//create a new unsigned int array for all the indices
	std::vector<unsigned int> indices;
	for (int y = 0; y < size - 1; ++y)
	{
		for (int i = 0; i < size - 1; ++i)
		{
			int base = (y * size) + i;
			indices.push_back(base);
			indices.push_back(base + size);
			indices.push_back(base + size + 1);

			indices.push_back(base);
			indices.push_back(base + size + 1);
			indices.push_back(base + 1);
		}
	}

	model = new AssimpModel(m_vertices, indices);
}