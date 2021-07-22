#pragma once
#include "Camera.h"
#include "pch.h"
#include "Model.h"
#include "Window.h" 

const UINT BUFFER_COUNT = 3;

class DeferredRendering
{
private:
	ID3D11Device* device;
	ID3D11DeviceContext* immediateConxtex;
	ID3D11RenderTargetView* rtv;
	ID3D11DepthStencilView* dsView;
	ID3D11InputLayout* inputLayout;
	ID3D11SamplerState* sampler;
	ID3D11SamplerState* CubesTextSamplerState;
	ID3D11RasterizerState* renderState;
	D3D11_VIEWPORT viewport;
	Window* win;

	SimpleVertex screenQuad[4];
	ID3D11Buffer* vertexBufferQuad;
	ID3D11Buffer* indexBufferQuad;

	ID3D11VertexShader* deferred_geometry_vs;
	ID3D11PixelShader* deferred_geometry_ps;
	ID3D11VertexShader* deferred_light_vs;
	ID3D11PixelShader* deferred_light_ps;
	std::string deferred_geometry_vs_bytecode;
	std::string deferred_light_vs_bytecode;
	std::string deferred_geometry_ps_bytecode;
	std::string deferred_light_ps_bytecode;

	ID3D11ShaderResourceView* textureSRVCharlie;
	ID3D11Texture2D* texture;
	std::string filePathCharlie = "../Texture/this-is-fine-Charlie.png";

	ID3D11ShaderResourceView* textureSRVObj;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* constantBufferObj;
	ID3D11Buffer* constantBufferLight;

	dx::XMMATRIX world = dx::XMMatrixIdentity();
	dx::XMMATRIX translate = dx::XMMatrixIdentity();
	dx::XMMATRIX scale = dx::XMMatrixIdentity();
	dx::XMMATRIX rotate = dx::XMMatrixIdentity();
	dx::XMMATRIX view = dx::XMMatrixIdentity();
	dx::XMMATRIX perspectiveProjection = dx::XMMatrixIdentity();

	dx::XMMATRIX translateCube = dx::XMMatrixIdentity();
	dx::XMMATRIX worldCube = dx::XMMatrixIdentity();

	Model object;
	//Camera cam;

	constantBufferMatrixes test;

	TextureRenderTarget graphicsBuffer[BUFFER_COUNT];
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;


public:
	DeferredRendering();
	~DeferredRendering();
	void Initialize(ID3D11Device*& device, ID3D11DeviceContext*& immadeiateContect, ID3D11RenderTargetView *&rtv, ID3D11DepthStencilView*& dsView, D3D11_VIEWPORT& viewport, Model& obj, Window* win);
	bool LoadShaders();
	bool LoadShaderData(const std::string& filename, std::string& vShaderByteCode);

	bool CreateInputLayout();
	//bool CreateVertexBuffer();
	//bool CreateIndexBuffer();
	bool CreateCbPerObj();
	bool CreateCbLight();
	bool CreateTexture(std::string filepath, ID3D11ShaderResourceView*& textureSRV);
	bool CreateSamplerState();
	bool CreateQuadAndBuffer();
	bool CreateGraphicsBuffer();

	bool SetupPipeline();
	void RenderGeometryPass(cbFrameObj* cbPerObj);
	void RenderLightPass();

	void Update(cbFrameObj* frameBuffer, float& rot, Light* lightBuffer, Camera& cam);
	bool ObjCreateBuffers();
	void RenderObj(cbFrameObj* cbPerObj, Camera& cam);
	bool CreateCubeSamplerState();
	void Release();
};
