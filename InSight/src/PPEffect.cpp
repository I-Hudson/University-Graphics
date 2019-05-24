#include "PPEffect.h"

PPEffect::PPEffect()
{
}

PPEffect::~PPEffect()
{
}

void PPEffect::init(PostProcessing * aPostProcessing)
{
	//set post processing
	mPostProcessing = aPostProcessing;
	mActive = false;
}

PostProcessing* PPEffect::getPostProcessing() const
{
	//return pointer to post processing
	return mPostProcessing;
}
