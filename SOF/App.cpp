#include "stdafx.h"
#include "App.h"
#include "window.h"

void SOF::App::SetParent( SOF::Window *p )
{
	parent = p;
}

bool SOF::App::IsKeyDown(unsigned int key) // is a key down
{
	return parent->IsKeyDown(key);
}

bool SOF::App::IsKeyPressed(unsigned int key) // was this key pressed in the last update? Only returns true in the duration of one Draw call.
{
	return parent->IsKeyPressed(key);
}

bool SOF::App::IsButtonDown(unsigned int button) // is a mouse button down
{
	return parent->IsKeyDown(button);
}

bool SOF::App::IsButtonPressed(unsigned int button) // was this mouse button pressed in the last update? Only returns true in the duration of one Draw call.
{
	return parent->IsButtonPressed(button);
}

void SOF::App::ClearButtons() // forget which button was last pressed
{
	return parent->ClearButtons();
}

