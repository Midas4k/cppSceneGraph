#pragma once
#include "stdafx.h"

namespace SOF
{
	class Window;
	class App
	{
		Window *parent = nullptr;
	public:
		void SetParent(Window *p);
		bool IsKeyDown(unsigned int key); // is a key down
		bool IsKeyPressed(unsigned int key); // was this key pressed in the last update? Only returns true in the duration of one Draw call.
	
		bool IsButtonDown(unsigned int button); // is a mouse button down
		bool IsButtonPressed(unsigned int button); // was this mouse button pressed in the last update? Only returns true in the duration of one Draw call.
		void ClearButtons(); // forget which button was last pressed

		virtual void Init() = 0;
		virtual void Draw() = 0;
		virtual void KeyCallback(const GLFWwindow*, int, int, int, int) {} // general keyboard response
		virtual void MouseButtonCallback(const GLFWwindow*, int, int, int) {} // general mouse response
		virtual void Update( float deltaT ) = 0; // called just before each Draw()
	};
}