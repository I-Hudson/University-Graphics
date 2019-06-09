#pragma once

#ifndef __INPUT_H__
#define __INPUT_H__

namespace InSight
{
	class Input
	{
	public:
		inline static bool IsKeyPressed(int aKeycode) { return sInstance->IsKeyPressedImpl(aKeycode); }

		inline static bool						IsMouseButtonPressed(int aButton) { return sInstance->IsMouseButtonPressedImpl(aButton); }
		inline static std::pair<float, float>	GetMousePosition() { return sInstance->GetMousePositionImpl(); }
		inline static float						GetMouseX() { return sInstance->GetMouseXImpl(); }
		inline static float						GetMouseY() { return sInstance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int aKeycode) = 0;

		virtual bool						IsMouseButtonPressedImpl(int aButton) = 0;
		virtual std::pair<float, float>		GetMousePositionImpl() = 0;
		virtual float						GetMouseXImpl() = 0;
		virtual float						GetMouseYImpl() = 0;

	private:
		static Input* sInstance;
	};
}
#endif // !__INPUT_H__
