#include <Windows.h>
#include <iostream>
#include <d3d11.h>
#include "d3d11Helper.h"
#include <chrono>
#include "PipelineHelper.h"
#include "windowHelper.h"

class Engine
{
private:


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
	void render();

};