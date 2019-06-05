#pragma once

#ifndef __PP_BLOOM_H__
#define __PP_BLOOM_H__

#include "GBuffer.h"
#include "Shader/Shader.h"
#include "PP/PPEffect.h"

class PPBloom : public PPEffect
{
public:
	//Constructor
	PPBloom();
	//Destructor
	~PPBloom();

	//overrides
	void		draw() override;
	void		destroy() override;

	//set input images to use 
	void		setInputImages(unsigned int& aSceneImage, unsigned int& aHDRImage);

private:

	//shaders
	Shader* mEvalShader;
	Shader* mBlurShader;
	Shader* mCompositeShader;

	//input input to use
	unsigned int mHDRImage;
	//scene image
	unsigned int mSceneImage;

	//blur horizontal texture size
	float mHorizontalSize;
	//blur vertical texture size
	float mVerticalSize;

	//colour evaluation buffer
	GBuffer* mEvalBuffer;
	//blur horizontal bufer
	GBuffer* mHorBuffer;
	//blur vertical bufer
	GBuffer* mVerBuffer;
};

#endif // __PP_BLOOM_H__
