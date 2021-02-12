#include <Windows.h>
#include <iostream>
#include <d3d11.h>
#pragma comments(lib,"d3d11.lib")
#include "d3d11Helper.h"
#include <chrono>
#include "PipelineHelper.h"
#include "Render.h"
#include "Engine.h"
#include <Keyboard.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevIntance, _In_ LPWSTR lpmCmdLine, _In_ int nCmdShow)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	Engine engine(hInstance, hPrevIntance, lpmCmdLine, nCmdShow);
	engine.Run();

	return 0;
}