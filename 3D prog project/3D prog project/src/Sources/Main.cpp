#include <Windows.h>
#pragma comments(lib,"d3d11.lib")
#include "Engine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevIntance, _In_ LPWSTR lpmCmdLine, _In_ int nCmdShow)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	Engine engine(hInstance, hPrevIntance, lpmCmdLine, nCmdShow);
	engine.Run();

	return 0;
}