#pragma once

#ifndef __WINODWS_INPUT_H__
#define __WINODWS_INPUT_H__

#include "Input/Input.h"

namespace InSight
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int aKeycode) override;

		virtual bool IsMouseButtonPressedImpl(int aButton) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}
#endif // !__WINODWS_INPUT_H__
