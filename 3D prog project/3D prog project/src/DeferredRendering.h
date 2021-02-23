#pragma once
#include <d3d11.h>
#include <string>
#include <DirectXMath.h>
#include "Camera.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "structs.h"
#include "Model.h"

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
	ID3D11SamplerState* CubesTextSamplerState;

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

	ID3D11Buffer* cubeVertexBuffer;
	ID3D11Buffer* cubeIndexBuffer;

	dx::XMMATRIX world = dx::XMMatrixIdentity();
	dx::XMMATRIX translate = dx::XMMatrixIdentity();
	dx::XMMATRIX scale = dx::XMMatrixIdentity();
	dx::XMMATRIX rotate = dx::XMMatrixIdentity();
	dx::XMMATRIX view = dx::XMMatrixIdentity();
	dx::XMMATRIX perspectiveProjection = dx::XMMatrixIdentity();

	dx::XMMATRIX translateCube = dx::XMMatrixIdentity();
	dx::XMMATRIX worldCube = dx::XMMatrixIdentity();

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	Model object;
	//Camera cam;

public:
	DeferredRendering();
	~DeferredRendering();
	void Initialize(ID3D11Device*& device, ID3D11DeviceContext*& immadeiateContect, ID3D11RenderTargetView *&rtv, ID3D11DepthStencilView*& dsView, D3D11_VIEWPORT& viewport, std::string filePath, Model& obj);
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
	void Render(cbFrameObj* cbPerObj);
	void Update(cbFrameObj* frameBuffer, float& rot, cbFrameLight* lightBuffer, Camera& cam);
	bool ObjCreateBuffers();
	void RenderObj(cbFrameObj* cbPerObj, Camera& cam);
	bool CreateCubeSamplerState();
	void Release();
};
