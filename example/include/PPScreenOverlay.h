#pragma once

#ifndef __PP_SCREEN_OVERLAY_H__
#define	__PP_SCREEN_OVERLAY_H__

#include "PPEffect.h"

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
	unsigned int mProgram;
};

#endif // __PP_SCREEN_OVERLAY_H__
