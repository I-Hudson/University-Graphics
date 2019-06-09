#pragma once

#include "Shader/BaseShader.h"

#include <vector>

class EntityManager;

class BaseRenderer
{
public:
	//Constructor
	BaseRenderer();
	//Destructor
	~BaseRenderer();

	//add entity
	template<typename T, typename... TArgs>
	T* addShader(TArgs&&... mArgs)
	{
		T* BaseShader = new T(std::forward<TArgs>(mArgs)...);
		BaseShader->setBaseRenderer(this);
		mShadersToRender.emplace_back(BaseShader);

		return BaseShader;
	}

	//remove entity
	void removeShader(BaseShader* a_removeShader);

	//draw
	void draw();

	//destroy
	void destroy();

	//set entity manager
	void setEntityManager(EntityManager* aEntityManager);
	//get entity manager
	EntityManager* getEntityManager() const;

	//set the screen size
	void setScreenSize(const glm::vec2& aScreenSize);
	//get the screen size
	glm::vec2 getScreenSize();

private:
	//store all the enitity to draw
	std::vector<BaseShader*> mShadersToRender;
	//when a mesh component is added. Add that to this vector to 
	//draw 

	//store screen size
	glm::vec2 mScreenSize;

	//pointer to entit manager
	EntityManager* mEntityManager;
};