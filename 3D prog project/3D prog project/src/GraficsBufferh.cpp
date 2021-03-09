#include "GraficsBufferh.h"

graficsBuffer::graficsBuffer()
{
	int i;
	for(i=0;i<BUFFERCOUNT;i++)
	{
		renderTargetTextureArray[i] = 0;
		renderTargetVeiwArray[i] = 0;
		shaderRescourceViewArray[i] = 0;
	}
	depthStencilView = 0;
	depthStencilBuffer = 0;
}

graficsBuffer::graficsBuffer(const graficsBuffer& graficBuffer)
{
}

graficsBuffer::~graficsBuffer()
{
}

bool graficsBuffer::initialize(ID3D11Device* device, int widthTexture,  int heightTexture, float screenDepth, float screenNear)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	textureWidth = widthTexture;
	textureHeight = heightTexture;

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	for(int i=0 ;i<BUFFERCOUNT;i++)
	{
		result = device->CreateTexture2D(&textureDesc, NULL, &renderTargetTextureArray[i]);
		if (FAILED(result)) 
		{
			return false;
		}
	
	}
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;


	for(int i=0;i<BUFFERCOUNT;i++)
	{
		result = device->CreateRenderTargetView(renderTargetTextureArray[i], &renderTargetViewDesc, &renderTargetVeiwArray[i]);
		if (FAILED(result))
		{
			return false;
		}
	
	}
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	for (int i = 0; i < BUFFERCOUNT; i++)
	{
		result = device->CreateShaderResourceView(renderTargetTextureArray[i], &shaderResourceViewDesc, &shaderRescourceViewArray[i]);
		if (FAILED(result))
		{
			return false;
		}

	}
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	if(FAILED(result))
	{
		return false;
	}
	ZeroMemory(&depthStencilViewDesc,sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
		if (FAILED(result))
		{
			return false;
		
		}
		viewport.Width = (float)textureWidth;
		viewport.Height = (float)textureHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

	return true;
}

void graficsBuffer::Release()
{
	
	if(depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = 0;
	
	}
	if(depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		depthStencilBuffer = 0;
	
	}
	for(int i=0;i<BUFFERCOUNT;i++)
	{
		if(shaderRescourceViewArray[i])
		{
			shaderRescourceViewArray[i]->Release();
			shaderRescourceViewArray[i] = 0;
		
		}
		if(renderTargetVeiwArray)
		{
			renderTargetVeiwArray[i]->Release();
			renderTargetVeiwArray[i] = 0;
		
		}
		if(renderTargetTextureArray)
		{
			renderTargetTextureArray[i]->Release();
			renderTargetTextureArray[i] = 0;
		
		}
	
	}
	


	
}

void graficsBuffer::setRenderTagers(ID3D11DeviceContext* deviceContext)
{
	deviceContext->OMSetRenderTargets(BUFFERCOUNT,renderTargetVeiwArray,depthStencilView);
	deviceContext->RSSetViewports(1, &viewport);


}

void graficsBuffer::clearRenderTargets(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha)
{
	float color[4];
	
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	for(int i=0;i>BUFFERCOUNT;i++)
	{
		deviceContext->ClearRenderTargetView(renderTargetVeiwArray[i], color);
	}
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

ID3D11ShaderResourceView* graficsBuffer::getShaderResoureceView(int view)
{
	return shaderRescourceViewArray[view];
}
