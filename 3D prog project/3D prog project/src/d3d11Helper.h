#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <iostream>
#include <assert.h>

bool CreateInterface(UINT width, UINT height, HWND window, ID3D11Device*& device, ID3D11DeviceContext*& immadeiateContext, IDXGISwapChain*& swapChain);

bool CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11RenderTargetView*& rtv);

bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*&);

void SetViewPort(D3D11_VIEWPORT& viewport, UINT width, UINT height);

bool SetupD3D11(UINT width, UINT height, HWND window, ID3D11Device*& device, ID3D11DeviceContext*& immadeiateContect, IDXGISwapChain*& swapChain, ID3D11RenderTargetView*& rtv,
	ID3D11Texture2D*& disTexture, ID3D11DepthStencilView*& dsView, D3D11_VIEWPORT& viewport);
