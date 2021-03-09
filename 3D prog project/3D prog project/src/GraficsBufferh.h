#pragma once
#include <d3d11.h>

const int BUFFERCOUNT= 2;
class graficsBuffer
{
private:
	
	int textureWidth, textureHeight;
	ID3D11Texture2D* renderTargetTextureArray[BUFFERCOUNT];
	ID3D11RenderTargetView* renderTargetVeiwArray[BUFFERCOUNT];
	ID3D11ShaderResourceView* shaderRescourceViewArray[BUFFERCOUNT];
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;
	D3D11_VIEWPORT viewport;


public:
	graficsBuffer();
	graficsBuffer(const graficsBuffer&graficBuffer);
	~graficsBuffer();
	
	bool initialize(ID3D11Device* device, int widthTexture, int heightTexture, float screenDepth, float screenNear);
	void Release();

	void setRenderTagers(ID3D11DeviceContext* deviceContext);
	void clearRenderTargets(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha);

	ID3D11ShaderResourceView* getShaderResoureceView(int view);

};