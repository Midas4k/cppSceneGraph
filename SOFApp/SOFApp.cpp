// SOFApp.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Week5App.h"

int main()
{
	Week5App *app = new Week5App();
	SOF::Window::CreateWindow(1024, 768, "Week5 Lab 2", app);
	SOF::Window::Run();

    return 0;
}

