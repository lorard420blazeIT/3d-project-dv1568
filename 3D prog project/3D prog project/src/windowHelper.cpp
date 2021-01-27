#include "windowHelper.h"
#include <iostream>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	//Ifall meddelandet �r DESTROY(n�r vi st�nger f�nstret) s� st�nger vi ner programmet
	switch (message) 
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default: //Annars ba break
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam); //Default hantering av messeges 
}

bool SetupWindow(HINSTANCE instance, UINT width, UINT height, int nCmdShow, HWND& window)
{
	//Console window
	AllocConsole();
	HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int hConsole = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
	FILE* fp = _fdopen(hConsole, "w");
	freopen_s(&fp, "CONOUT$", "w", stdout); //S� cout syns i konsol f�nstret
	freopen_s(&fp, "CONOUT$", "w", stderr); // S� cerr syns i konsol f�nstret

	const wchar_t CLASS_NAME[] = L"Demo WindowClass"; //L = wide

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);
	
	//Skapar f�nsret,
	//(optional styling = default, klassnamn, namn, style, default pos, ignorerar y, bredd, h�jd, igen parentWindow, ingen meny, hanterar instance, ingen extra data)
	window = CreateWindowEx(0, CLASS_NAME, L"Demo Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, width, height, nullptr, nullptr, instance, nullptr);

	if (window == nullptr) //Kollar ifall de gick och ger felmedelande ifall de inte gick
	{
		std::cerr << "HWND was nullptr, last error: " << GetLastError() << std::endl;
		return false;
	}

	SetWindowPos(window, nullptr, 20, 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	ShowWindow(window, nCmdShow);

	return true;
}
