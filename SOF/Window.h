#pragma once
#include "stdafx.h"
#include "app.h"
class Timer;
namespace SOF
{
	// define a namespace, so we can use common and obvious names like 'window' without conflict

	class Window
	{
		static int width;
		static int height;
		static GLFWwindow *pWindow;
		static App *app;
		static glm::vec4 color;
		static bool *keys;
		static bool *lastKeys;

		static bool *buttons;
		static bool *lastButtons;

		static void ClearKeys();
		
		

	public:
		static void CreateWindow(int width, int height, std::string name, App *app); 
		static void SetBackground(glm::vec4 c);
		static bool IsKeyDown(unsigned int key);
		static bool IsKeyPressed(unsigned int key);
		static bool IsButtonDown(unsigned int button);
		static bool IsButtonPressed(unsigned int button);
		static void Run();
		static int GetWidth();
		static int GetHeight();
		static void KeyCallback(GLFWwindow*, int, int, int, int);
		static void MouseButtonCallback(GLFWwindow*, int, int, int);
		static void ClearButtons();

	};
}