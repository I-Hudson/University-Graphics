#pragma once

#ifndef __PP_SCREEN_OVERLAY_H__
#define	__PP_SCREEN_OVERLAY_H__

#include "InSight/PP/PPEffect.h"

class PPScreenOverlay : public PPEffect
{
public:
	//Constructor
	PPScreenOverlay();
	//Destructor
	~PPScreenOverlay();

	//perform effect
	void			draw()override;

private:
	//program
	unsigned int mProgram;
};

#endif // __PP_SCREEN_OVERLAY_H__
