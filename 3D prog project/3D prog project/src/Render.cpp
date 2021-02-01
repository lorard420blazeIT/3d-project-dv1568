#include "Render.h"

void Render::render()
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
