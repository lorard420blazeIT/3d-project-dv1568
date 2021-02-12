#pragma once
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include "Keyboard.h"
#include "Mouse.h"
#include <d3d11.h>

namespace dx = DirectX;

class Window
{
private:
	//Används inte just nu 
	HINSTANCE instance;
	float height;
	float width;
	int nCmdShow;
	HWND win;

public:
	Window();
	bool setUpWindow(HINSTANCE instance, UINT width, UINT height, int nCmdShow);
	bool setUpConsole();
	void moveWindow(UINT xPos, UINT yPos);
	float getHeight() const { return this->height; }
	float getWidth() const { return this->width; }
	HWND getHWND() const { return this->win; }
};