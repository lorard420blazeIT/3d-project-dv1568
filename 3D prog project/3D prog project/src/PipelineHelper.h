#pragma once
#define ALIGN16 __declspec(align(16))
#include <DirectXMath.h>
namespace dx = DirectX;

#include <array>
#include <d3d11.h>
#include <Windows.h>
#include <WinBase.h>
#include <stdio.h>
#include <WinnT.h>
#include <string>
#include "Camera.h"
#include "structs.h"

//struct  SimpleVertex 
//{
//	float pos[3] = { 0 };
//	float uv[2] = { 0 };
//	float clr[3] = { 0 };
//	float norm[3] = { 0 };
//
//	SimpleVertex(const std::array<float, 3> position = { 0,0,0 }, const std::array<float, 2> uvCoords = { 0,0 }, const std::array<float, 3> color = { 1,0,1 }, const std::array<float, 3> normal = { 0,0,0 })    //Ask teacher
//	{
//		for (int i = 0; i < 3; i++)
//		{
//			pos[i] = position[i];
//			clr[i] = color[i];
//			norm[i] = normal[i];
//		}
//		uv[0] = uvCoords[0];
//		uv[1] = uvCoords[1];
//	}
//};
//
//ALIGN16 ////Saftey guard att man inte får fler på allginment, avslutar structen på 16 så nästa kan räknas in rätt
//struct cbFrameObj//Använder XMFloat4x4 för XMMatrix lätt blir messy med minnes fel ect
//{
//	dx::XMFLOAT4X4 wvp;//Perform offsett and rotation 4 bits per
//	dx::XMFLOAT4X4 world;
//	dx::XMFLOAT4X4 scale;
//	dx::XMFLOAT4X4 rotation;
//	dx::XMFLOAT4X4 view;
//
//	//aling all the way 
//};
//ALIGN16 
//struct cbFrameLight
//{
//	dx::XMFLOAT3 pos;
//	float pad1;
//
//	dx::XMFLOAT3 ambient;
//	float pad2;
//
//	dx::XMFLOAT3 color;
//	float pad3;
//
//	dx::XMFLOAT3 camPos;
//	float pad4;
//
//	float specPower;
//	dx::XMFLOAT3 pad5;
//};

void UpdateBuffer(ID3D11Buffer*& cbPerObj, ID3D11Buffer*& cbLight,  ID3D11DeviceContext* immediateConxtex, cbFrameObj* frameBuffer, float &offsetX, cbFrameLight *lightBuffer, Camera &cam);
bool SetupPipeline(ID3D11Device* device, ID3D11Buffer*& vertexBuffer, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, ID3D11InputLayout*& inputLayer,
	ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& textureRSV, ID3D11SamplerState*& sampler, std::string filePath, ID3D11Buffer*& constantBufferObj, ID3D11Buffer*& constantBufferLight, ID3D11Buffer*& indexBuffer, ID3D11DeviceContext*& immidateContext);