#include "Engine.h"

Engine Engine::engine()
{
	return Engine();
}

void Engine::render(ID3D11DeviceContext* immediateConxtex, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsView, D3D11_VIEWPORT& viewport, ID3D11VertexShader* vShader, ID3D11PixelShader* pShader
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

void Engine::relese()
{
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
}
