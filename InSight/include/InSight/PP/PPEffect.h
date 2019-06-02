#pragma once

#ifndef __PP_EFFECT_H__
#define __PP_EFFECT_H__

#include "InSight/PP/PostProcessing.h"

class PPEffect
{
public:
	//Constructor
	PPEffect();
	//Destructor
	~PPEffect();

	//virtual
	virtual void		init(PostProcessing* aPostProcessing);
	virtual void		draw() {}
	virtual void		destroy() {}

	//get post processing parent
	PostProcessing*		getPostProcessing() const;

	bool mActive;

private:
	//ref to post processing parent
	PostProcessing* mPostProcessing;
};

#endif // __PP_EFFECT_H__
