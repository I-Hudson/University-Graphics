#pragma once

#ifndef __PP_EFFECT_H__
#define __PP_EFFECT_H__

#include "PostProcessing.h"

class PPEffect
{
public:
	//Constructor
	PPEffect();
	//Destructor
	~PPEffect();

	virtual void		init(PostProcessing* aPostProcessing);
	virtual void		draw() {}
	virtual void		destory() {}

	PostProcessing*		getPostProcessing() const;

private:
	PostProcessing* mPostProcessing;
};

#endif // __PP_EFFECT_H__
