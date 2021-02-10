#include "PipelineHelper.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <fstream>
#include <iostream>
#include <vector>

bool LoadShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, std::string& vShaderByteCode)
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

	if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vShader)))
	{
		std::cerr << "Failed to create vertex shader!" << std::endl;
		return false;
	}

	vShaderByteCode = shaderData;
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

	if (FAILED(device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pShader)))
	{
		std::cerr << "Failed to create pixel shader!" << std::endl;
		return false;
	}

	return true;
}

bool CreateInputLayout(ID3D11Device* device, ID3D11InputLayout*& inputLayout, const std::string& vShaderByteCode)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[4] = //Beskriver datan som vertexshadern vill ha in
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
		//Namnet som matchar i shadern , 
		//index på ifall de finns fler med samma namn tex 4x4 som då har 4 olika idex, 
		//vilket format datan är i - tex har pos RGB eftersom det är 3 världen medans uv ba har RG för den har bara två
		//Deffinierar input asselmblern
		//Offset i bytes, tex POSITION börjar i 0 medans UV börjar på byte 12 eftersom POSITION har 3 världen 4 bytes var 3*4 = 12 
		//Deffinierr input data klassen
		// Måste vara 0 när det är INPUT_PER_VERTEX_DATA
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, 4, vShaderByteCode.c_str(), vShaderByteCode.length(), &inputLayout);

	return !FAILED(hr);
}

bool CreateVertexBuffer(ID3D11Device* device, ID3D11Buffer*& vertexBuffer)
{
	SimpleVertex quad[] =
	{	
		//A
		{ {-0.5f, 0.5f, 0.0f},  {0.0f, 0.0f},  {0, 0, 1},  {0.0f, 0.0f, -1.0f} },

		//B
		{ {0.5f, 0.5f, 0.0f},   {1.0f, 0.0f},  {0, 0, 1},  {0.0f, 0.0f, -1.0f} },

		//E
		{ {-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f},  {0, 0, 1},  {0.0f, 0.0f, -1.0f} },

		//C
		{ {0.5f, -0.5f, 0.0f},  {1.0f, 1.0f},  {0, 0, 1},  {0.0f, 0.0f, -1.0f} },

		//D
		{ {-0.5f, 0.5f, 0.0f},  {0.0f, 0.0f},  {0, 0, 1},  {0.0f, 0.0f, -1.0f} },

		//F (B2)
		{ {0.5f, 0.5f, 0.0f},  {1.0f, 0.0f},  {0, 0, 1},  {0.0f, 0.0f, -1.0f} }
	
	};

	D3D11_BUFFER_DESC bufferDesc = { 0 }; //Deaufalt alla värden till 0;
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

	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	return !FAILED(hr);
}

bool CreateCbPerObj(ID3D11Device* device, ID3D11Buffer*& constantBuffer) //Use 1 or 0 instället fö bools. Allt måste alltid va float4 i storlek
{
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	assert(sizeof(cbFrameObj) % 16 == 0); //Ser till att det är 16 bitar
	bufferDesc.ByteWidth = sizeof(cbFrameObj); //Så buffern vet hur mycket data den ska ta in
	bufferDesc.Usage = D3D11_USAGE_DEFAULT; //måste ha default så jag kan transpose innan(så du kan skriva till den på gpu)
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &constantBuffer;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &constantBuffer);

	return !FAILED(hr);
}

bool CreateCbLight(ID3D11Device* device, ID3D11Buffer*& constantBuffer) //Use 1 or 0 instället fö bools. Allt måste alltid va float4 i storlek
{
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	assert(sizeof(cbFrameLight) % 16 == 0);
	bufferDesc.ByteWidth = sizeof(cbFrameLight);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &constantBuffer;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &constantBuffer);

	return !FAILED(hr);
}

void UpdateBuffer(ID3D11Buffer*& cbPerObj, ID3D11Buffer*& cbLight, ID3D11DeviceContext* immediateConxtex, cbFrameObj* frameBuffer, float& rot,  cbFrameLight* lightBuffer, Camera &cam)
{
	//Flags??? -> projekt

	dx::XMMATRIX world = dx::XMMatrixIdentity();
	dx::XMMATRIX translate = dx::XMMatrixIdentity();
	dx::XMMATRIX scale = dx::XMMatrixIdentity();
	dx::XMMATRIX rotate = dx::XMMatrixIdentity();
	dx::XMMATRIX view = dx::XMMatrixIdentity();
	dx::XMMATRIX perspectiveProjection = dx::XMMatrixIdentity();

	translate = dx::XMMatrixTranslation(0.0f, 0.0f, -1.5f);
	scale = dx::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	rotate = dx::XMMatrixRotationRollPitchYawFromVector({ 0, rot, 0 });

	world = scale * translate * rotate ;

	view = dx::XMLoadFloat4x4(&cam.getView());
	perspectiveProjection = dx::XMLoadFloat4x4(&cam.getPP());

	dx::XMMATRIX wvp = world * view * perspectiveProjection;
	dx::XMFLOAT4X4 saveMe;
	dx::XMStoreFloat4x4(&saveMe, dx::XMMatrixTranspose(wvp));
	frameBuffer->wvp = saveMe;
	dx::XMStoreFloat4x4(&saveMe, dx::XMMatrixTranspose(world));
	frameBuffer->world = saveMe;

	immediateConxtex->UpdateSubresource(cbPerObj, 0, nullptr, frameBuffer, 0, 0);
	immediateConxtex->PSSetConstantBuffers(0, 1, &cbPerObj);
	immediateConxtex->VSSetConstantBuffers(0, 1, &cbPerObj);

	lightBuffer->camPos = cam.getPos();

	immediateConxtex->UpdateSubresource(cbLight, 0, nullptr, lightBuffer, 0, 0);
	immediateConxtex->PSSetConstantBuffers(0, 1, &cbLight);
}

bool CreateTexture(ID3D11Device* device, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& textureSRV, std::string filePath) //Lägg in filepath
{
	int textureWidth = 0;
	int textureHeight = 0;
	int channels = sizeof(textureWidth);


	//Check if file exist
	std::ifstream ifile;
	ifile.open(filePath);
	if (ifile) 
	{
		std::cout << "file exists" << std::endl;
	}
	else 
	{
		std::cout << "file doesn't exist" << std::endl;
	}

	unsigned char* image = stbi_load(filePath.c_str(), &textureWidth, &textureHeight, &channels, STBI_rgb_alpha);


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
	desc.Usage = D3D11_USAGE_IMMUTABLE;		//Kan ej ändras efter vi skapat den, för vi ej vill röra vår textur
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//Så den kan bindas som en shader RESOURCE
	desc.CPUAccessFlags = 0;	//vi behöver inte komma åt den - ej läsa eller skriva till den 
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data; //Eftersom den är imnutable
	data.pSysMem = image; //pekar till en array
	data.SysMemPitch = textureWidth * 4; //4 för antalet kanaler
	data.SysMemSlicePitch = 0;

	if (FAILED(device->CreateTexture2D(&desc, &data, &texture)))
	{
		std::cerr << "Failed to create texture" << std::endl;
		return false;
	}
	
	HRESULT hr = device->CreateShaderResourceView(texture, nullptr, &textureSRV);
	stbi_image_free(image);
	return !FAILED(hr);
}

bool CreateSamplerState(ID3D11Device* device, ID3D11SamplerState*& sampler)
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

	HRESULT hr = device->CreateSamplerState(&desc, &sampler);
	return !FAILED(hr);
}

bool SetupPipeline(ID3D11Device* device, ID3D11Buffer*& vertexBuffer, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, ID3D11InputLayout*& inputLayer, ID3D11Texture2D*& texture,
				   ID3D11ShaderResourceView*& texturSRV, ID3D11SamplerState*& sampler, std::string filePath, ID3D11Buffer*& constantBufferObj, ID3D11Buffer*& constantBufferLight)
{
	std::string vShaderByteCode;
	if (!LoadShaders(device, vShader, pShader, vShaderByteCode))
	{
		std::cerr << "Error loading shader!" << std::endl;
		return false;
	}

	if (!CreateInputLayout(device, inputLayer, vShaderByteCode))
	{
		std::cerr << "Error creating input layout!" << std::endl;
		return false;
	}

	if (!CreateVertexBuffer(device, vertexBuffer))
	{
		std::cerr << "Error creating vertex buffer!" << std::endl;
		return false;
	}

	if (!CreateCbPerObj(device, constantBufferObj))
	{
		std::cerr << "Error creating constant buffer for objects!" << std::endl;
		return false;
	}

	if (!CreateCbLight(device, constantBufferLight))
	{
		std::cerr << "Error creating constant buffer for lights!" << std::endl;
		return false;
	}

	if (!CreateTexture(device, texture, texturSRV, filePath))
	{
		std::cerr << "Error creating texture!" << std::endl;
		return false;
	}
	if (!CreateSamplerState(device, sampler))
	{
		std::cerr << "Error creating sampler state!" << std::endl;
		return false;
	}

	return true;
}