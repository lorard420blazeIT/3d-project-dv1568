#include "DeferredRendering.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

DeferredRendering::DeferredRendering()
{
}

DeferredRendering::~DeferredRendering()
{
}

void DeferredRendering::Initialize(ID3D11Device*& device, ID3D11DeviceContext*& immadeiateContect, ID3D11RenderTargetView*& rtv, ID3D11DepthStencilView*& dsView, D3D11_VIEWPORT& viewport, std::string filePath, Model &obj)
{
	this->device = device;
	this->immediateConxtex = immadeiateContect;
	this->rtv = rtv;
	this->viewport = viewport;
	this->filePath = filePath;
	this->dsView = dsView;
	this->object = obj;
}

bool DeferredRendering::LoadShaders()
{
	std::string shaderData;
	std::ifstream reader;

	//VertexShader
	reader.open("../bin/Debug/VertexShader.cso", std::ios::binary | std::ios::ate);

	if (!reader.is_open())
	{
		std::cerr << "Could not open vertex shader file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &this->vShader)))
	{
		std::cerr << "Failed to create vertex shader!" << std::endl;
		return false;
	}

	this->vShaderByteCode = shaderData;
	shaderData.clear();
	reader.close();

	//Pixelshader
	reader.open("../bin/Debug/PixelShader.cso", std::ios::binary | std::ios::ate);

	if (!reader.is_open())
	{
		std::cerr << "Could not open pixel shader file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	if (FAILED(this->device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &this->pShader)))
	{
		std::cerr << "Failed to create pixel shader!" << std::endl;
		return false;
	}

	return true;
}

bool DeferredRendering::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[4] = //Beskriver datan som vertexshadern vill ha in
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}

		//Namnet som matchar i shadern , 
		//index p� ifall de finns fler med samma namn tex 4x4 som d� har 4 olika idex, 
		//vilket format datan �r i - tex har pos RGB eftersom det �r 3 v�rlden medans uv ba har RG f�r den har bara tv�
		//Deffinierar input asselmblern
		//Offset i bytes, tex POSITION b�rjar i 0 medans UV b�rjar p� byte 12 eftersom POSITION har 3 v�rlden 4 bytes var 3*4 = 12 
		//Deffinierr input data klassen
		// M�ste vara 0 n�r det �r INPUT_PER_VERTEX_DATA
	};

	HRESULT hr = this->device->CreateInputLayout(inputDesc, 4, this->vShaderByteCode.c_str(), this->vShaderByteCode.length(), &this->inputLayout);

	return !FAILED(hr);
}

bool DeferredRendering::CreateVertexBuffer()
{
	SimpleVertex quad[] =
	{
		//A
		{ {-0.5f, 0.5f, 0.0f},  {0.0f, 0.0f},  {0.0f, 0.0f, -1.0f},	 {0, 0, 1} },

		//B
		{ {0.5f, 0.5f, 0.0f},   {1.0f, 0.0f},  {0.0f, 0.0f, -1.0f},  {0, 0, 1} },

		//E
		{ {-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f},  {0.0f, 0.0f, -1.0f},  {0, 0, 1} },

		//C
		{ {0.5f, -0.5f, 0.0f},  {1.0f, 1.0f},  {0.0f, 0.0f, -1.0f},  {0, 0, 1} },

		//D
		{ {-0.5f, 0.5f, 0.0f},  {0.0f, 0.0f},  {0.0f, 0.0f, -1.0f},  {0, 0, 1} },

		//F (B2)
		{ {0.5f, 0.5f, 0.0f},  {1.0f, 0.0f},   {0.0f, 0.0f, -1.0f},  {0, 0, 1} }

	};

	D3D11_BUFFER_DESC bufferDesc = { 0 }; //Deaufalt alla v�rden till 0;
	bufferDesc.ByteWidth = sizeof(quad);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = quad;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = this->device->CreateBuffer(&bufferDesc, &data, &this->vertexBuffer);
	return !FAILED(hr);
}

bool DeferredRendering::CreateIndexBuffer()
{
	DWORD indicies[] =
	{
		0, 1, 2,
		1, 2, 3,
		2, 3, 4,
		3, 4, 5
	};

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&this->indexBuffer, sizeof(this->indexBuffer));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(DWORD) * 4 * 3;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = indicies;

	HRESULT hr = this->device->CreateBuffer(&bufferDesc, &data, &this->indexBuffer);

	return !FAILED(hr);
}

bool DeferredRendering::CreateCbPerObj()
{
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	assert(sizeof(cbFrameObj) % 16 == 0); //Ser till att det �r 16 bitar
	bufferDesc.ByteWidth = sizeof(cbFrameObj); //S� buffern vet hur mycket data den ska ta in
	bufferDesc.Usage = D3D11_USAGE_DEFAULT; //m�ste ha default s� jag kan transpose innan(s� du kan skriva till den p� gpu)
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &this->constantBufferObj;

	HRESULT hr = this->device->CreateBuffer(&bufferDesc, &data, &this->constantBufferObj);

	return !FAILED(hr);
}

bool DeferredRendering::CreateCbLight()
{
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	assert(sizeof(cbFrameLight) % 16 == 0);
	bufferDesc.ByteWidth = sizeof(cbFrameLight);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &this->constantBufferLight;

	HRESULT hr = this->device->CreateBuffer(&bufferDesc, &data, &this->constantBufferLight);

	return !FAILED(hr);
}

bool DeferredRendering::CreateTexture()
{
	int textureWidth = 0;
	int textureHeight = 0;
	int channels = sizeof(textureWidth);

	//Check if file exist
	std::ifstream ifile;
	ifile.open(this->filePath);
	if (ifile)
	{
		std::cout << "Texture file exists" << std::endl;
	}
	else
	{
		std::cout << "Texture file doesn't exist" << std::endl;
	}

	unsigned char* image = stbi_load(this->filePath.c_str(), &textureWidth, &textureHeight, &channels, STBI_rgb_alpha);


	std::vector<unsigned char> textureData;
	textureData.resize(textureWidth * textureHeight * 4);

	for (int h = 0; h < textureHeight; h++)
	{
		for (int w = 0; w < textureWidth; w++)
		{
			unsigned char r = w < textureWidth / 3 ? 255 : 0;
			unsigned char g = w >= textureWidth / 3 && w <= textureWidth / 1.5 ? 255 : 0;
			unsigned char b = w > textureWidth / 1.5 ? 255 : 0;
			unsigned int pos0 = w * 4 + textureWidth * 4 * h;

			textureData[pos0 + 0] = r;
			textureData[pos0 + 1] = g;
			textureData[pos0 + 2] = b;
			textureData[pos0 + 3] = 255;
		}
	}

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = textureWidth;
	desc.Height = textureHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//8 bitar 4 kompoenter - 1 bite = 8 bitar - unorm(normaliserar)
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;		//Kan ej �ndras efter vi skapat den, f�r vi ej vill r�ra v�r textur
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//S� den kan bindas som en shader RESOURCE
	desc.CPUAccessFlags = 0;	//vi beh�ver inte komma �t den - ej l�sa eller skriva till den 
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data; //Eftersom den �r imnutable
	data.pSysMem = image; //pekar till en array
	data.SysMemPitch = textureWidth * 4; //4 f�r antalet kanaler
	data.SysMemSlicePitch = 0;

	if (FAILED(this->device->CreateTexture2D(&desc, &data, &this->texture)))
	{
		std::cerr << "Failed to create texture" << std::endl;
		return false;
	}

	HRESULT hr = this->device->CreateShaderResourceView(this->texture, nullptr, &this->textureSRV);
	stbi_image_free(image);
	return !FAILED(hr);
}

bool DeferredRendering::CreateSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0;
	desc.MaxAnisotropy = 16;
	desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = this->device->CreateSamplerState(&desc, &this->sampler);
	return !FAILED(hr);
}

bool DeferredRendering::SetupPipeline()
{
	if (!LoadShaders())
	{
		std::cerr << "Error loading shader!" << std::endl;
		return false;
	}

	if (!CreateInputLayout())
	{
		std::cerr << "Error creating input layout!" << std::endl;
		return false;
	}

	if (!CreateVertexBuffer())
	{
		std::cerr << "Error creating vertex buffer!" << std::endl;
		return false;
	}

	if (!CreateIndexBuffer())
	{
		std::cerr << "Error creating index buffer!" << std::endl;
		return false;
	}

	if (!CreateCbPerObj())
	{
		std::cerr << "Error creating constant buffer for objects!" << std::endl;
		return false;
	}

	if (!CreateCbLight())
	{
		std::cerr << "Error creating constant buffer for lights!" << std::endl;
		return false;
	}

	if (!CreateTexture())
	{
		std::cerr << "Error creating texture!" << std::endl;
		return false;
	}
	if (!CreateSamplerState())
	{
		std::cerr << "Error creating sampler state!" << std::endl;
		return false;
	}

	return true;
}

void DeferredRendering::position()
{
}

void DeferredRendering::Render(cbFrameObj* cbPerObj)
{
	float clearColor[4] = { 0.58f, 0.44, 0.86, 1.0 };
	this->immediateConxtex->ClearRenderTargetView(this->rtv, clearColor);
	this->immediateConxtex->ClearDepthStencilView(this->dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	this->immediateConxtex->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	this->immediateConxtex->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
	this->immediateConxtex->IASetInputLayout(this->inputLayout);
	this->immediateConxtex->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	this->immediateConxtex->VSSetShader(this->vShader, nullptr, 0);
	this->immediateConxtex->RSSetViewports(1, &this->viewport);
	this->immediateConxtex->PSSetShader(this->pShader, nullptr, 0);
	this->immediateConxtex->PSSetShaderResources(0, 1, &this->textureSRV);
	this->immediateConxtex->PSSetSamplers(0, 1, &this->sampler);
	this->immediateConxtex->OMSetRenderTargets(1, &this->rtv, this->dsView);

	this->immediateConxtex->DrawIndexed(12, 0, 0);

	this->translate = dx::XMMatrixTranslation(2.0f, 0.0f, 0.0f);

	this->world = this->scale * this->translate * this->rotate;

	dx::XMMATRIX wvp = this->world * this->view * this->perspectiveProjection;
	dx::XMFLOAT4X4 saveMe;
	dx::XMStoreFloat4x4(&saveMe, dx::XMMatrixTranspose(wvp));
	cbPerObj->wvp = saveMe;
	dx::XMStoreFloat4x4(&saveMe, dx::XMMatrixTranspose(world));
	cbPerObj->world = saveMe;

	this->immediateConxtex->UpdateSubresource(this->constantBufferObj, 0, nullptr, cbPerObj, 0, 0);
	this->immediateConxtex->VSSetConstantBuffers(0, 1, &this->constantBufferObj);
	this->immediateConxtex->DrawIndexed(12, 0, 0);

}

void DeferredRendering::Update(cbFrameObj* cbPerObj, float& rot, cbFrameLight* lightBuffer, Camera& cam)
{
	this->translateCube = dx::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	this->translate = dx::XMMatrixTranslation(-2.0f, 0.0f, 0.0f);
	this->scale = dx::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	this->rotate = dx::XMMatrixRotationRollPitchYawFromVector({ 0, rot, 0 });

	this->world = this->scale * this->translate * this->rotate;

	this->view = dx::XMLoadFloat4x4(&cam.getView());
	this->perspectiveProjection = dx::XMLoadFloat4x4(&cam.getPP());

	dx::XMMATRIX wvp = this->world * this->view * this->perspectiveProjection;
	dx::XMFLOAT4X4 saveMe;
	dx::XMStoreFloat4x4(&saveMe, dx::XMMatrixTranspose(wvp));
	cbPerObj->wvp = saveMe;
	dx::XMStoreFloat4x4(&saveMe, dx::XMMatrixTranspose(world));
	cbPerObj->world = saveMe;


	this->immediateConxtex->UpdateSubresource(this->constantBufferObj, 0, nullptr, cbPerObj, 0, 0);
	this->immediateConxtex->VSSetConstantBuffers(0, 1, &this->constantBufferObj);

	lightBuffer->camPos = cam.getPos();

	this->immediateConxtex->UpdateSubresource(this->constantBufferLight, 0, nullptr, lightBuffer, 0, 0);
	this->immediateConxtex->PSSetConstantBuffers(0, 1, &this->constantBufferLight);
}

void DeferredRendering::Release()
{
	this->inputLayout->Release();
	this->textureSRV->Release();
	this->sampler->Release();
	this->vShader->Release();
	this->pShader->Release();
	this->texture->Release();
	this->vertexBuffer->Release();
	this->indexBuffer->Release();
	this->constantBufferObj->Release();
	this->constantBufferLight->Release();
}

bool DeferredRendering::CreateCubeSamplerState()
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = this->device->CreateSamplerState(&sampDesc, &CubesTextSamplerState);
	return !FAILED(hr);
}

bool DeferredRendering::ObjCreateBuffers()
{
	//Create indexBuffer
	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * this->object.getMeshTriangles() * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &this->object.getIndicies()[0];

	//for (int i = 0; i < object.getIndicies().size(); i++)
	//{
	//	std::cout << (this->object.getIndicies()[i]+1) << " ";

	//	if (i % 3 == 0)
	//	{
	//		std::cout << std::endl;
	//	}
	//}
	//std::cout << std::endl;

	//std::cout << "indiceis size: "<<object.getIndicies().size() << std::endl;

	HRESULT hrI = this->device->CreateBuffer(&indexBufferDesc, &iinitData, &this->cubeIndexBuffer);

	if (FAILED(hrI))
	{
		std::cerr << "Couldnt create CubeIndexBuffer" << std::endl;
		return false;
	}

	//Create Vertexbuffer
	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.ByteWidth = sizeof(SimpleVertex) * this->object.getTotaltVerts();
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	vertexBufferData.pSysMem = &this->object.getVerticies()[0];
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	
	HRESULT hrV = this->device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &this->cubeVertexBuffer);

	if (FAILED(hrV))
	{
		std::cerr << "Couldnt create CubeVertexBuffer" << std::endl;
		return false;
	}

	return true;
}

void DeferredRendering::RenderObj(cbFrameObj* cbPerObj, Camera& cam)
{
	//std::cout << object.getMeshSubsets() << std::endl;

	//for (int i = 0; i < object.getMeshSubsets(); ++i)
	//{
		//std::cout << "in for loop" << std::endl;

		this->immediateConxtex->IASetIndexBuffer(this->cubeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		this->immediateConxtex->IASetVertexBuffers(0, 1, &this->cubeVertexBuffer, &this->stride, &this->offset);
		//this->immediateConxtex->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		this->translate = dx::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		this->scale = dx::XMMatrixScaling(1.0f, 1.0f, 1.0f);
		this->rotate = dx::XMMatrixRotationRollPitchYawFromVector({ 0, 0, 0 });
		this->worldCube = this->scale * this->translateCube * this->rotate;

		this->view = dx::XMLoadFloat4x4(&cam.getView());
		this->perspectiveProjection = dx::XMLoadFloat4x4(&cam.getPP());

		dx::XMMATRIX wvp = this->worldCube * this->view * this->perspectiveProjection;
		dx::XMFLOAT4X4 saveMe;
		dx::XMStoreFloat4x4(&saveMe, dx::XMMatrixTranspose(wvp));
		cbPerObj->wvp = saveMe;
		dx::XMStoreFloat4x4(&saveMe, dx::XMMatrixTranspose(worldCube));
		cbPerObj->world = saveMe;

		this->immediateConxtex->UpdateSubresource(this->constantBufferObj, 0, nullptr, cbPerObj, 0, 0);
		this->immediateConxtex->VSSetConstantBuffers(0, 1, &this->constantBufferObj);
		this->immediateConxtex->PSSetConstantBuffers(1, 1, &this->constantBufferObj);
		this->immediateConxtex->PSSetShaderResources(0, 1, &this->textureSRV);
		this->immediateConxtex->PSSetSamplers(0, 1, &this->sampler);

		int indexStart = this->object.getSubsetsIndexStart()[0];
		int indexDrawAmunt = this->object.getSubsetsIndexStart()[0 + 1] - this->object.getSubsetsIndexStart()[0];
		this->immediateConxtex->DrawIndexed(indexDrawAmunt, 0, 0);
	//}
}