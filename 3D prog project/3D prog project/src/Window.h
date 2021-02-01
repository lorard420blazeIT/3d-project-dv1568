#pragma once
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>

class Window 
{
private:
	HINSTANCE instance;
	UINT height;
	UINT width;
	int nCmdShow;
	HWND win;

public:
	Window();
	bool setUpWindow(HINSTANCE instance, UINT width, UINT height, int nCmdShow, HWND& win);
	bool setUpConsole();
};