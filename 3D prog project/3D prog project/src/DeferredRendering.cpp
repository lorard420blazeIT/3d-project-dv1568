#include "DeferredRendering.h"

DeferredRendering::DeferredRendering()
{
}

DeferredRendering::~DeferredRendering()
{
}

void DeferredRendering::Initialize(ID3D11Device*& device, ID3D11DeviceContext*& immadeiateContect, ID3D11RenderTargetView*& rtv, ID3D11DepthStencilView*& dsView, D3D11_VIEWPORT& viewport, Model& obj, Window* win)
{
	this->device = device;
	this->immediateConxtex = immadeiateContect;
	this->rtv = rtv;
	this->viewport = viewport;
	this->dsView = dsView;
	this->object = obj;
	this->win = win;
}

bool DeferredRendering::LoadShaders()
{
	LoadShaderData("deferred_geometry_vs", deferred_geometry_vs_bytecode);
	LoadShaderData("deferred_geometry_ps", deferred_geometry_ps_bytecode);
	LoadShaderData("deferred_light_vs", deferred_light_vs_bytecode);
	LoadShaderData("deferred_light_ps", deferred_light_ps_bytecode);

	if (FAILED(device->CreateVertexShader(deferred_geometry_vs_bytecode.c_str(), deferred_geometry_vs_bytecode.length(), nullptr, &deferred_geometry_vs)))
	{
		std::cerr << "Failed to create deferred_geometry_vs shader!" << std::endl;
		return false;
	}

	if (FAILED(device->CreatePixelShader(deferred_geometry_ps_bytecode.c_str(), deferred_geometry_ps_bytecode.length(), nullptr, &deferred_geometry_ps)))
	{
		std::cerr << "Failed to create  deferred_geometry_ps shader!" << std::endl;
		return false;
	}

	if (FAILED(device->CreateVertexShader(deferred_light_vs_bytecode.c_str(), deferred_light_vs_bytecode.length(), nullptr, &deferred_light_vs)))
	{
		std::cerr << "Failed to create deferred_light_vs shader!" << std::endl;
		return false;
	}

	if (FAILED(device->CreatePixelShader(deferred_light_ps_bytecode.c_str(), deferred_light_ps_bytecode.length(), nullptr, &deferred_light_ps)))
	{
		std::cerr << "Failed to create deferred_light_ps shader!" << std::endl;
		return false;
	}

	return true;
}

bool DeferredRendering::LoadShaderData(const std::string& filename, std::string& shaderByteCode)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open("../bin/Debug/" + filename + ".cso", std::ios::binary | std::ios::ate);

	if (!reader.is_open())
	{
		std::cerr << "Could not open " + filename + " shader file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	shaderByteCode = shaderData;
	reader.close();

	return true;
}

bool DeferredRendering::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = //Beskriver datan som vertexshadern vill ha in
	{
		{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0,					0,					D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV",			0, DXGI_FORMAT_R32G32_FLOAT,	0,		D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0,		D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = this->device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), this->deferred_geometry_vs_bytecode.c_str(), this->deferred_geometry_vs_bytecode.length(), &this->inputLayout);

	return !FAILED(hr);
}

bool DeferredRendering::CreateGraphicsBuffer()
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = win->getWidth();
	textureDesc.Height = win->getHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		hr = device->CreateTexture2D(&textureDesc, NULL, &graphicsBuffer[i].texture);
		if (FAILED(hr))
		{
			std::cerr << "Failed to create Texture2D for graphicsBuffern" << std::endl;
			return false;
		}
	}

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		hr = device->CreateRenderTargetView(graphicsBuffer[i].texture.Get(), &rtvDesc, &graphicsBuffer[i].renderTargetView);
		if (FAILED(hr))
		{
			std::cerr << "Failed to create RenderTargetView for graphicsBuffern" << std::endl;
			return false;
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		hr = device->CreateShaderResourceView(graphicsBuffer[i].texture.Get(), &srvDesc, &graphicsBuffer[i].shaderResourceView);
		if (FAILED(hr))
		{
			std::cerr << "Failed to create ShaderResourceView for graphicsBuffern" << std::endl;
			return false;
		}
	}

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthBufferDesc.Width = textureDesc.Width;
	depthBufferDesc.Height = textureDesc.Height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create depthStencilBuffer for graphicsBuffer" << std::endl;
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilDesc, &depthStencilView);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create depthStencilView for graphicsBuffer" << std::endl;
		return false;
	}

	return true;
}



//Buffers for quads

//bool DeferredRendering::CreateVertexBuffer()
//{
//	SimpleVertex quad[] =
//	{
//		//F1
//		//POSITION					UV			COLOR		NORMAL
//		{ {-0.5f, 0.5f, 0.0f},  {0.0f, 0.0f},	{0.0f, 0.0f, -1.0f} },
//
//		{ {0.5f, 0.5f, 0.0f},   {1.0f, 0.0f},	{0.0f, 0.0f, -1.0f} },
//
//		{ {-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f},	{0.0f, 0.0f, -1.0f} },
//
//		//F2
//		{ {-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f},	{0.0f, 0.0f, -1.0f} },
//
//		{ {0.5f, 0.5f, 0.0f},   {1.0f, 0.0f},	{0.0f, 0.0f, -1.0f} },
//
//		{ {0.5f, -0.5f, 0.0f},  {1.0f, 1.0f},	{0.0f, 0.0f, -1.0f} },
//		
//	};
//
//	D3D11_BUFFER_DESC bufferDesc = { 0 }; //Deaufalt alla v�rden till 0;
//	bufferDesc.ByteWidth = sizeof(quad);
//	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
//	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bufferDesc.CPUAccessFlags = 0;
//	bufferDesc.MiscFlags = 0;
//	bufferDesc.StructureByteStride = 0;
//
//	D3D11_SUBRESOURCE_DATA data;
//	data.pSysMem = quad;
//	data.SysMemPitch = 0;
//	data.SysMemSlicePitch = 0;
//
//	HRESULT hr = this->device->CreateBuffer(&bufferDesc, &data, &this->vertexBuffer);
//	return !FAILED(hr);
//}
//
//bool DeferredRendering::CreateIndexBuffer()
//{
//	DWORD indicies[] =
//	{
//		0, 1, 2,
//		1, 2, 3,
//		2, 3, 4,
//		3, 4, 5
//	};
//
//	D3D11_BUFFER_DESC bufferDesc;
//	ZeroMemory(&this->indexBuffer, sizeof(this->indexBuffer));
//	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	bufferDesc.ByteWidth = sizeof(DWORD) * 4 * 3;
//	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	bufferDesc.CPUAccessFlags = 0;
//	bufferDesc.MiscFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA data;
//	data.pSysMem = indicies;
//
//	HRESULT hr = this->device->CreateBuffer(&bufferDesc, &data, &this->indexBuffer);
//
//	return !FAILED(hr);
//}

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
	assert(sizeof(Light) % 16 == 0);
	bufferDesc.ByteWidth = sizeof(Light);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &this->constantBufferLight;

	HRESULT hr = this->device->CreateBuffer(&bufferDesc, &data, &this->constantBufferLight);

	return !FAILED(hr);
}

bool DeferredRendering::CreateTexture(std::string filePath, ID3D11ShaderResourceView*& textureSRV)
{
	int textureWidth = 0;
	int textureHeight = 0;
	int channels = sizeof(textureWidth);

	//Check if file exist
	std::ifstream ifile;
	ifile.open(filePath);
	if (ifile)
	{
		std::cout << "Texture file exists: " << filePath << std::endl;
	}
	else
	{
		std::cout << "Texture file doesn't exist: " << filePath << std::endl;
	}

	unsigned char* image = stbi_load(filePath.c_str(), &textureWidth, &textureHeight, &channels, STBI_rgb_alpha);

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = textureWidth;
	desc.Height = textureHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//8 bitar 4 kompoenter - 1 bite = 8 bitar - unorm(normaliserar)
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_IMMUTABLE;		//Kan ej �ndras efter vi skapat den, f�r vi ej vill r�ra v�r textur
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//S� den kan bindas som en shader RESOURCE


	D3D11_SUBRESOURCE_DATA data; //Eftersom den �r imnutable
	data.pSysMem = image; //pekar till en array
	data.SysMemPitch = textureWidth * 4; //4 f�r antalet kanaler
	data.SysMemSlicePitch = 0;

	if (FAILED(this->device->CreateTexture2D(&desc, &data, &this->texture)))
	{
		std::cerr << "Failed to create texture" << std::endl;
		return false;
	}

	HRESULT hr = this->device->CreateShaderResourceView(this->texture, nullptr, &textureSRV);
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

bool DeferredRendering::CreateQuadAndBuffer()
{
	screenQuad[0] = { {-1.0f,	1.0f, 0.0f},	{0.0f, 0.0f},	{0.0f, 0.0f, -1.0f} };
	screenQuad[1] = { {1.0f,	1.0f, 0.0f},	{1.0f, 0.0f},	{0.0f, 0.0f, -1.0f} };
	screenQuad[2] = { {-1.0f,	-1.0f, 0.0f },	{ 0.0f, 1.0f},  { 0.0f, 0.0f, -1.0f} };
	screenQuad[3] = { {1.0f,	-1.0f, 0.0f},	{1.0f, 1.0f},	{0.0f, 0.0f, -1.0f} };

	D3D11_BUFFER_DESC bufferDesc = { 0 }; //Deaufalt alla v�rden till 0;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.ByteWidth = sizeof(screenQuad);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = screenQuad;

	HRESULT hr = this->device->CreateBuffer(&bufferDesc, &data, &this->vertexBufferQuad);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create screeenQuad vertexBuffer" << std::endl;
		return false;
	}

	DWORD indicies[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	D3D11_BUFFER_DESC bufferDescIndex;
	ZeroMemory(&bufferDescIndex, sizeof(D3D11_BUFFER_DESC));
	bufferDescIndex.Usage = D3D11_USAGE_DEFAULT;
	bufferDescIndex.ByteWidth = sizeof(DWORD) * 2 * 3;
	bufferDescIndex.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA dataIndex;
	ZeroMemory(&dataIndex, sizeof(D3D11_SUBRESOURCE_DATA));
	dataIndex.pSysMem = indicies;

	hr = this->device->CreateBuffer(&bufferDescIndex, &dataIndex, &this->indexBufferQuad);

	if (FAILED(hr))
	{
		std::cerr << "Failed to create screeenQuad indexbuffer" << std::endl;
		return false;
	}
	return true;
}


bool DeferredRendering::SetupPipeline()
{


	if (!LoadShaders())
	{
		std::cerr << "Error loading shaders!" << std::endl;
		return false;
	}

	if (!CreateInputLayout())
	{
		std::cerr << "Error creating input layout!" << std::endl;
		return false;
	}

	//if (!CreateVertexBuffer())
	//{
	//	std::cerr << "Error creating vertex buffer!" << std::endl;
	//	return false;
	//}

	//if (!CreateIndexBuffer())
	//{
	//	std::cerr << "Error creating index buffer!" << std::endl;
	//	return false;
	//}
	if (!CreateGraphicsBuffer())
	{
		std::cerr << "Error creating graphics buffers!" << std::endl;
		return false;
	}

	if (!CreateQuadAndBuffer())
	{
		std::cerr << "Error creating quad or/and buffers!" << std::endl;
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

	//Create charlie texture
	if (!CreateTexture(filePathCharlie, textureSRVCharlie))
	{
		std::cerr << "Error creating texture!" << std::endl;
		return false;
	}
	//Create obj texture
	if (!CreateTexture(object.getTexturePath(), textureSRVObj))
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



void DeferredRendering::Update(cbFrameObj* cbPerObj, float& rot, Light* lightBuffer, Camera& cam)
{
	this->translateCube = dx::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	this->translate = dx::XMMatrixTranslation(-2.0f, 0.0f, 0.0f);
	this->scale = dx::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	this->rotate = dx::XMMatrixRotationRollPitchYawFromVector({ 0, rot, 0 });

	this->world = this->scale * this->translate * this->rotate;

	this->view = dx::XMLoadFloat4x4(&cam.getView());
	this->perspectiveProjection = dx::XMLoadFloat4x4(&cam.getPP());

	dx::XMFLOAT4X4 saveMe;

	dx::XMStoreFloat4x4(&saveMe, dx::XMMatrixTranspose(world));
	cbPerObj->world = saveMe;
	dx::XMStoreFloat4x4(&saveMe, dx::XMMatrixTranspose(view));
	cbPerObj->view = saveMe;
	dx::XMStoreFloat4x4(&saveMe, dx::XMMatrixTranspose(perspectiveProjection));
	cbPerObj->projection = saveMe;

	this->immediateConxtex->UpdateSubresource(this->constantBufferObj, 0, nullptr, cbPerObj, 0, 0);
	this->immediateConxtex->VSSetConstantBuffers(0, 1, &this->constantBufferObj);

	lightBuffer->camPos = cam.getPos();

	this->immediateConxtex->UpdateSubresource(this->constantBufferLight, 0, nullptr, lightBuffer, 0, 0);
	this->immediateConxtex->PSSetConstantBuffers(0, 1, &this->constantBufferLight);
}

void DeferredRendering::Release()
{
	this->inputLayout->Release();
	this->textureSRVCharlie->Release();
	this->textureSRVObj->Release();
	this->sampler->Release();
	this->deferred_geometry_vs->Release();
	this->deferred_geometry_ps->Release();
	this->deferred_light_vs->Release();
	this->deferred_light_ps->Release();
	this->texture->Release();
	this->vertexBuffer->Release();
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
	//Create 

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.ByteWidth = sizeof(SimpleVertex) * this->object.getTotaltVerts();
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	vertexBufferData.pSysMem = &this->object.getVerticies()[0];

	HRESULT hr = this->device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &this->vertexBuffer);

	if (FAILED(hr))
	{
		std::cerr << "Couldnt create vertexBuffer" << std::endl;
		return false;
	}

	return true;
}

void DeferredRendering::RenderGeometryPass(cbFrameObj* cbPerObj)
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	float clearColor[4] = { 0.58f, 0.44, 0.86, 1.0 };
	this->immediateConxtex->ClearRenderTargetView(this->rtv, clearColor);
	this->immediateConxtex->ClearDepthStencilView(this->dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	this->immediateConxtex->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
	//this->immediateConxtex->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	this->immediateConxtex->IASetInputLayout(this->inputLayout);
	this->immediateConxtex->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->immediateConxtex->VSSetShader(this->deferred_geometry_vs, nullptr, 0);
	this->immediateConxtex->RSSetViewports(1, &this->viewport);
	this->immediateConxtex->PSSetShader(this->deferred_geometry_ps, nullptr, 0);
	this->immediateConxtex->PSSetShaderResources(0, 1, &this->textureSRVCharlie);
	this->immediateConxtex->PSSetSamplers(0, 1, &this->sampler);
	this->immediateConxtex->OMSetRenderTargets(1, &this->rtv, this->dsView);
}

void DeferredRendering::RenderObj(cbFrameObj* cbPerObj, Camera& cam)
{
	/*for (int i = 0; i < object.getSubSetCount(); ++i)
	{*/

	this->translate = dx::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	this->scale = dx::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	this->rotate = dx::XMMatrixRotationRollPitchYawFromVector({ 0, 0, 0 });
	this->worldCube = this->scale * this->translateCube * this->rotate;

	this->view = dx::XMLoadFloat4x4(&cam.getView());
	this->perspectiveProjection = dx::XMLoadFloat4x4(&cam.getPP());

	dx::XMFLOAT4X4 saveMe;

	dx::XMStoreFloat4x4(&saveMe, dx::XMMatrixTranspose(worldCube));
	cbPerObj->world = saveMe;

	this->immediateConxtex->UpdateSubresource(this->constantBufferObj, 0, nullptr, cbPerObj, 0, 0);
	this->immediateConxtex->VSSetConstantBuffers(0, 1, &this->constantBufferObj);
	this->immediateConxtex->PSSetConstantBuffers(1, 1, &this->constantBufferObj);
	this->immediateConxtex->PSSetShaderResources(0, 1, &textureSRVObj);
	this->immediateConxtex->PSSetSamplers(0, 1, &this->CubesTextSamplerState);

	//int indexStart = this->object.getSubsetsIndexStart()[i];
	//int indexDrawAmunt = this->object.getSubsetsIndexStart()[i + 1] - this->object.getSubsetsIndexStart()[i];
	//this->immediateConxtex->DrawIndexed(indexDrawAmunt, indexStart, 0);
	this->immediateConxtex->Draw(object.getTotaltVerts(), 0);
	//}
}

void DeferredRendering::RenderLightPass()
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	immediateConxtex->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateConxtex->IASetVertexBuffers(0, 1, &vertexBufferQuad, &stride, &offset);
	immediateConxtex->IASetIndexBuffer(indexBufferQuad, DXGI_FORMAT_R32_UINT, 0);
	immediateConxtex->VSSetShader(deferred_light_vs, nullptr, 0);
	immediateConxtex->PSSetShader(deferred_light_ps, nullptr, 0);
}