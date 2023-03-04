#include "stdafx.h"
#include "Window.h"
#include "App.h"
#include "timer.h"

int SOF::Window::width;
int SOF::Window::height;
GLFWwindow* SOF::Window::pWindow;
SOF::App* SOF::Window::app;

bool* SOF::Window::keys;
bool* SOF::Window::lastKeys;

bool* SOF::Window::buttons;
bool* SOF::Window::lastButtons;

glm::vec4 SOF::Window::color;

static void GLAPIENTRY ErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

void SOF::Window::CreateWindow(int nWidth, int nHeight, std::string name, SOF::App *nApp)
{
	// set a default background color
	color = glm::vec4(0.5f, 0.2f, 0.3f, 1.0f);

	// initialise GLFW
	if (!glfwInit())
		throw std::runtime_error("glfwInit failed");

	// open a window with GLFW
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	width = nWidth;
	height = nHeight;
	app = nApp;

	pWindow = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (!pWindow)
		throw std::runtime_error("glfwCreateWindow failed");

	glfwMakeContextCurrent(pWindow);

	// initialise GLEW
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("glewInit failed");

	// GLEW throws some errors, so discard all the errors so far
	while (glGetError() != GL_NO_ERROR) {}

	// make sure OpenGL version 4.2 API is available
	if (!GLEW_VERSION_4_2)
		throw std::runtime_error("OpenGL 4.2 API is not available.");

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(ErrorCallback, 0);

	// initialize keyboard input
	keys = new bool[GLFW_KEY_LAST + 1];
	lastKeys = new bool[GLFW_KEY_LAST + 1];
	std::memset(keys, 0, sizeof(bool)*(GLFW_KEY_LAST + 1));
	std::memset(lastKeys, 0, sizeof(bool)*(GLFW_KEY_LAST + 1));

	// initialize mouse input
	buttons = new bool[GLFW_MOUSE_BUTTON_LAST + 1];
	lastButtons = new bool[GLFW_MOUSE_BUTTON_LAST + 1];
	std::memset(buttons, 0, sizeof(bool)*(GLFW_MOUSE_BUTTON_LAST + 1));
	std::memset(lastButtons, 0, sizeof(bool)*(GLFW_MOUSE_BUTTON_LAST + 1));
}

bool SOF::Window::IsKeyDown(unsigned int key)
{
	return keys[key];
}

bool SOF::Window::IsKeyPressed(unsigned int key)
{
	return lastKeys[key];
}

void SOF::Window::ClearKeys()
{
	std::memset(lastKeys, 0, sizeof(bool)*(GLFW_KEY_LAST + 1));
}

void SOF::Window::KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		keys[key] = true;
		lastKeys[key] = true;
	}
	if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}
	app->KeyCallback(window, key, scanCode, action, mods);
}


bool SOF::Window::IsButtonDown(unsigned int button)
{
	return buttons[button];
}

bool SOF::Window::IsButtonPressed(unsigned int button)
{
	return lastButtons[button];
}

void SOF::Window::ClearButtons()
{
	std::memset(lastButtons, 0, sizeof(bool)*(GLFW_MOUSE_BUTTON_LAST + 1));
}

void SOF::Window::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		buttons[button] = true;
		lastButtons[button] = true;
	}
	if (action == GLFW_RELEASE)
	{
		buttons[button] = false;
	}
	app->MouseButtonCallback(window, button, action, mods);
}

void SOF::Window::SetBackground(glm::vec4 c)
{
	color = c;
}


void SOF::Window::Run()
{
	app->Init();
	// set keyboard callback
	glfwSetKeyCallback(pWindow, KeyCallback);
	// and mouse callback
	glfwSetMouseButtonCallback(pWindow, MouseButtonCallback);
	SOF::Timer timer;
	timer.Reset();
	while (!glfwWindowShouldClose(pWindow)) {


		// process pending events
		glfwPollEvents();

		//call the update function (just allows separation of the data-update and the drawing)
		float deltaT = timer.Elapsed();
		timer.Reset();
		app->Update( deltaT );

		glClearColor(color[0], color[1], color[2], color[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// call the render function
		app->Draw();

		glfwSwapBuffers(pWindow);

		// check for errors
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			std::cerr << "OpenGL Error " << error << std::endl;

		//exit program if escape key is pressed
		if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(pWindow, GL_TRUE);
	}

	// clean up and exit
	glfwTerminate();
}

int SOF::Window::GetWidth()
{
	return width;
}

int SOF::Window::GetHeight()
{
	return height;
}