#pragma once

#include <Windows.h>
#include <io.h>
#include <fcntl.h>


bool SetupWindow(HINSTANCE instance, UINT width, UINT height, int nCmdShow, HWND& window);
