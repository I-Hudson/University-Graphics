#include "Component/MeshComponent.h"
#include "Utilities.h"
#include "Log.h"
#include "Component/CameraComponent.h"

#include <string>
#include "imgui_fileDialog.h"

namespace InSight
{
	MeshComponent::MeshComponent(const char* aFile, LoadType aLoadType)
	{
		if (aLoadType == LoadType::AssimpLoader)
		{
			//load new model
			model = new AssimpModel(aFile);
			//set render mode
			mRenderMode = RenderMode::Opaque;
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

	void MeshComponent::draw(BaseShader* aShader, const glm::mat4& aModelMatrx, const bool& aBindTextures)
	{
		if (model == nullptr)
		{
			return;
		}

		//bing our BaseShader program
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

	void MeshComponent::draw(BaseShader* aShader, unsigned int& aProgram, const glm::mat4 & aModelMatrx, const bool & aBindTextures)
	{
		if (model == nullptr)
		{
			return;
		}

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

	void MeshComponent::draw(const int& aDrawMode, BaseShader* aShader, const glm::mat4& aModelMatrx, const bool& aBindTextures)
	{
		if (model == nullptr)
		{
			return;
		}

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
		if (ImGui::TreeNode("Model Component"))
		{
			ImGui::Text(std::string("Model Path :" + (model != nullptr ? *model->getModelFileName() : "")).c_str());
			if (ImGui::Button("Load Model"))
			{
				mShowFileExplorer = true;
			}

			if (mShowFileExplorer)
			{
				if (ImGuiFileDialog::Instance()->FileDialog("File Explorer", ".obj\0.fbx", "."))
				{
					if (ImGuiFileDialog::Instance()->IsOk == true)
					{
						std::string modelPath = ImGuiFileDialog::Instance()->GetFilepathName();
						model = new AssimpModel(modelPath.c_str(), true);
					}
					mShowFileExplorer = false;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Unload Model"))
			{
				if (model != nullptr)
				{
					//destroy model
					model->Destroy();
					delete model;
					model = nullptr;
				}
			}

			if (ImGui::TreeNode("BaseShader Properties"))
			{
				mShader->gui();
				/*for (auto& s : mShaders)
				{
					if (ImGui::CollapsingHeader(std::string(typeid(*s).name()).c_str()))
					{
						s->gui();
					}
				}*/
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}

	void MeshComponent::destroy()
	{
		if (model != nullptr)
		{
			//destroy model
			model->Destroy();
			delete model;
		}
	}

	int MeshComponent::Save()
	{
		return GetUnqiueID();
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

	void MeshComponent::addShader(BaseShader* aShader)
	{
		//add BaseShader
		mShader = aShader;
		mShader->setMeshComponent(this);
		//check if BaseShader it good
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
}