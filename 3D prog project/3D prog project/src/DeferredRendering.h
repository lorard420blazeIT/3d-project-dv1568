#pragma once
#include <d3d11.h>
#include <string>
#include <DirectXMath.h>
#include "Camera.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "structs.h"

namespace dx = DirectX;

class DeferredRendering
{
private:
	
	ID3D11Device* device;
	ID3D11DeviceContext* immediateConxtex;
	ID3D11RenderTargetView* rtv;
	ID3D11DepthStencilView* dsView;
	ID3D11InputLayout* inputLayout;
	ID3D11ShaderResourceView* textureSRV;
	ID3D11SamplerState* sampler;
	D3D11_VIEWPORT viewport;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	std::string vShaderByteCode;
	ID3D11Texture2D* texture;
	std::string filePath;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* constantBufferObj;
	ID3D11Buffer* constantBufferLight;


	Camera cam;

public:
	DeferredRendering();
	~DeferredRendering();
	void Initialize(ID3D11Device*& device, ID3D11DeviceContext*& immadeiateContect, ID3D11RenderTargetView *&rtv, ID3D11DepthStencilView*& dsView, D3D11_VIEWPORT& viewport, Camera &cam, std::string filePath);
	bool LoadShaders();
	bool CreateInputLayout();
	bool CreateVertexBuffer();
	bool CreateIndexBuffer();
	bool CreateCbPerObj();
	bool CreateCbLight();
	bool CreateTexture();
	bool CreateSamplerState();
	bool SetupPipeline();
	void position();
	void Render();
	void Update(cbFrameObj* frameBuffer, float& rot, cbFrameLight* lightBuffer);
	/*ID3D11Buffer gBuffer(ID3D11RenderTargetView gbufferRtv, ID3D11Texture2D texture2d, ID3D11ShaderResourceView shadersgbuffer);*/
};
