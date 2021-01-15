#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef STRICT
#define STRICT
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <d3d11.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	std::cout << "Hello world" << std::endl;
	int y = 5;
	return 0;
	
}