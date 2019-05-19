#include "PPEffect.h"

PPEffect::PPEffect()
{
}

PPEffect::~PPEffect()
{
}

void PPEffect::init(PostProcessing * aPostProcessing)
{
	mPostProcessing = aPostProcessing;
}

PostProcessing * PPEffect::getPostProcessing() const
{
	return mPostProcessing;
}
