#pragma once
#include <d3d11.h>
#include <string>
class DeferredRendering
{
private:
	
	ID3D11Device* device;
	ID3D11VertexShader*& vShader;
	ID3D11PixelShader*& pShader;
	std::string & vShaderByteCode;
	ID3D11Texture2D*& texture;
	std::string filePath;

public:
	void position();
	void Gbuffer(ID3D11DepthStencilView dsvGbuffer);
	/*ID3D11Buffer gBuffer(ID3D11RenderTargetView gbufferRtv, ID3D11Texture2D texture2d, ID3D11ShaderResourceView shadersgbuffer);*/
};
