#pragma once

#include "InSight/Debug.h"

#ifndef __POST_PROCESSING_H__
#define __POST_PROCESSING_H__

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

class PPEffect;

class PostProcessing
{
public:
	//Constructor
	PostProcessing(); 
	//Destructor
	~PostProcessing();

	//Add effect 
	template<typename T>
	void						addEffect()
	{
		//if we do not have effect of T. Then add the effect
		if (getEffect<T>() == nullptr)
		{
			T* newEffect = DEBUG_NEW T();
			newEffect->init(this);

			mEffects.push_back(newEffect);
		}
	}

	//return effect
	template<typename T>
	T*							getEffect()
	{
		//loop though all effects
		for (auto& effect :mEffects)
		{
			//cast effect to T*
			T* tempT = dynamic_cast<T*>(effect);
			//if tempT is not null (cast worked) then return effect
			if (tempT != nullptr)
			{
				return dynamic_cast<T*>(effect);
			}
		}
		return nullptr;
	}

	//remove effect 
	template<typename T>
	void						removeEffect()
	{
		T* effect = getEffect<T>();
		if (effect != nullptr)
		{
			//get effect in mEffects
			std::vector<PPEffect*>::iterator it = std::find(mEffects.begin(), mEffects.end(), effect);
			//get the index of effect
			int index = std::distance(mEffects.begin(), it);

			//delete effect
			delete effect;
			//remove from mEffects
			mEffects.erase(mEffects.begin() + index);
		}
	}

	//return all effects 
	std::vector<PPEffect*>*		getAllEffects();

	//show gui
	void						gui();

	//destroy
	void						destroy();

	//perform all pp effects
	void						draw();

	//render the fullscreen quad
	void						renderQuad();

	//set the exposure 
	float mExposure;
	//set the gamma
	float mGamma;

private:

	//generate the buffers needed to render the quad
	void						generateBuffers();

	//current pp effect
	int mPPEffectActive;

	//store all the current effects in use
	std::vector<PPEffect*> mEffects;

	//Buffers for the full screen quad
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mIBO;

	//struct for the full screen quad
	struct FSVertex
	{
		glm::vec4 mPostion;
		glm::vec2 mTexCoord1;

		FSVertex() {}
		FSVertex(const glm::vec4& aPosition, const glm::vec2& aTexCoord1) 
		{
			mPostion = aPosition;
			mTexCoord1 = aTexCoord1;
		}
	};
};

#endif // __POST_PROCESSING_H__

