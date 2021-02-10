#include <Windows.h>
#include <iostream>
#include <d3d11.h>
#include "d3d11Helper.h"
#include <chrono>
#include "PipelineHelper.h"
#include "Window.h"
#include "Render.h"
#include "Camera.h"
#include "InputHandler.h"

class Engine
{
private:
	const UINT WIDTH = 700;
	const UINT HEIGHT = 500;

	cbFrameObj frame;
	cbFrameLight lightFrame;
	//InputHandler input;

	//For update 
	MSG msg;
	std::chrono::high_resolution_clock clock;
	float deltaTime;
	float currentRotation;
	float speed;
	Render render;

	//Window stuff
	Window windowMain;
	
	HWND winHWND;
	HINSTANCE instance;
	int nCmdShow;

	std::string textureFilePath;

public:
	ID3D11Device* device;
	ID3D11DeviceContext* immediateConxtex;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* rtv;
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;
	D3D11_VIEWPORT viewport;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vertexBuffer;
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* textureSRV;
	ID3D11SamplerState* sampler;
	ID3D11Buffer* constantBufferObj;
	ID3D11Buffer* constantBufferLight;

	Engine(HINSTANCE& hinstance, HINSTANCE& hPrevIntance, LPWSTR& lpmCmdLine, int& nCmdShow);
	bool SetUp();
	void Update();
	void ReleaseAll();
	void Run();

};