#include <Windows.h>
#include <iostream>
#include <d3d11.h>
#include "windowHelper.h"
#include "d3d11Helper.h"
#include "PipelineHelper.h"
#include <chrono>

void Render(ID3D11DeviceContext* immediateConxtex, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsView, D3D11_VIEWPORT& viewport, ID3D11VertexShader* vShader, ID3D11PixelShader* pShader
	, ID3D11InputLayout* inputLayout, ID3D11Buffer* vertexBuffer, ID3D11ShaderResourceView* textureSRV, ID3D11SamplerState* sampler)
{
	float clearColor[4] = { 0, 0, 0, 0 };
	immediateConxtex->ClearRenderTargetView(rtv, clearColor);
	immediateConxtex->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	immediateConxtex->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	immediateConxtex->IASetInputLayout(inputLayout);
	immediateConxtex->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	immediateConxtex->VSSetShader(vShader, nullptr, 0);
	immediateConxtex->RSSetViewports(1, &viewport);
	immediateConxtex->PSSetShader(pShader, nullptr, 0);
	immediateConxtex->PSSetShaderResources(0, 1, &textureSRV);
	immediateConxtex->PSSetSamplers(0, 1, &sampler);
	immediateConxtex->OMSetRenderTargets(1, &rtv, dsView);

	immediateConxtex->Draw(6, 0);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevIntance, _In_ LPWSTR lpmCmdLine, _In_ int nCmdShow)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	//How to find the memory leak tex: _crtBreakAlloc = 202216;

	const UINT WIDTH = 700;;
	const UINT HEIGHT = 500;
	HWND window;
	std::string filePath = "../Texture/this-is-fine-Charlie.png";

	if (!SetupWindow(hInstance, WIDTH, HEIGHT, nCmdShow, window))
	{
		std::cerr << "Failed to set up window!" << std::endl;
		return -1;
	}

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

	cbFrameLight lightFrame;
	lightFrame.ambient = { 0.3f, 0.3f, 0.3f };
	lightFrame.pos = { 0, 0, -3 };
	lightFrame.color = { 1, 1, 1 };
	lightFrame.specPower = 128.0f;

	if (!SetupD3D11(HEIGHT, WIDTH, window, device, immediateConxtex, swapChain, rtv, dsTexture, dsView, viewport))
	{
		std::cerr << "Failed to create D3D11!" << std::endl;
		return -1;
	}

	if (!SetupPipeline(device, vertexBuffer, vShader, pShader, inputLayout, texture, textureSRV, sampler, filePath, constantBufferObj, constantBufferLight))
	{
		std::cerr << "Failed to setup pipeline!" << std::endl;
		return -1;
	}

	MSG msg = {};

	std::chrono::high_resolution_clock clock;
	auto timeStart = clock.now();
	auto timeStop = clock.now();
	float deltaTime = 0;
	float currentRotation = 0;
	float speed = 0.5f;
	cbFrameObj frame;

	//Update
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		timeStop = clock.now();

		using ms = std::chrono::duration<float, std::milli>;
		deltaTime = std::chrono::duration_cast<ms>(timeStop - timeStart).count() / 1000;

		currentRotation = deltaTime * speed;
		//std::cout << deltaTime << std::endl;

		Render(immediateConxtex, rtv, dsView, viewport, vShader, pShader, inputLayout, vertexBuffer, textureSRV, sampler);
		swapChain->Present(0, 0);
		UpdateBuffer(constantBufferObj, constantBufferLight, immediateConxtex, &frame, currentRotation, &lightFrame);
	}

	constantBufferLight->Release();
	constantBufferObj->Release();
	texture->Release();
	textureSRV->Release();
	sampler->Release();
	vertexBuffer->Release();
	inputLayout->Release();
	vShader->Release();
	pShader->Release();
	dsView->Release();
	dsTexture->Release();
	rtv->Release();
	swapChain->Release();
	immediateConxtex->Release();
	device->Release();

	return 0;
}