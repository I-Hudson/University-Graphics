#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Shader/Shader.h"

#include <vector>

class EntityManager;

class Renderer
{
public:
	//Constructor
	Renderer();
	//Destructor
	~Renderer();

	//add entity
	template<typename T, typename... TArgs>
	T* addShader(TArgs&&... mArgs)
	{
		T* shader = new T(std::forward<TArgs>(mArgs)...);
		shader->setRenderer(this);
		mShadersToRender.emplace_back(shader);

		return shader;
	}

	//remove entity
	void removeShader(Shader* a_removeShader);

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
	std::vector<Shader*> mShadersToRender;
	//when a mesh component is added. Add that to this vector to 
	//draw 

	//store screen size
	glm::vec2 mScreenSize;

	//pointer to entit manager
	EntityManager* mEntityManager;
};

#endif //__RENDERER_H__
