#pragma once

#ifndef __PP_BLUR_H__
#define __PP_BLUR_H__

#include "GBuffer.h"
#include "Shader.h"
#include "PPEffect.h"

class PPBlur : public PPEffect
{
public:
	//Constructor
	PPBlur();
	//Destructor
	~PPBlur();

	void		draw() override;

	void		setFinalImage(unsigned int& aImage);

private:

	Shader* mBlurShader;
	Shader* mCompositeShader;
	//final rendered image from deferred 
	unsigned int mFinalImage;

	unsigned int mPro;

	float mHorizontalSize;
	float mVerticalSize;

	GBuffer mHorBuffer;
	GBuffer mVerBuffer;
};

#endif // __PP_BLUR_H__
