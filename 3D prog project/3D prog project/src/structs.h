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

struct  SimpleVertex
{
	dx::XMFLOAT3 pos;
	dx::XMFLOAT2 uv;
	dx::XMFLOAT3 norm;
	dx::XMFLOAT3 clr;
	/*float pos[3] = { 0 };
	float uv[2] = { 0 };
	float clr[3] = { 0 };
	float norm[3] = { 0 };*/

	SimpleVertex(const dx::XMFLOAT3 position = { 0,0,0 }, const dx::XMFLOAT2 uvCoords = { 0,0 }, dx::XMFLOAT3 normal = { 0,0,0 }, const dx::XMFLOAT3 color = { 1,0,1 })    //Ask teacher
	{
		for (int i = 0; i < 3; i++)
		{
			pos = position;
			norm = normal;
			clr = color;
		}
		uv = uvCoords;
		uv = uvCoords;
	}
};

ALIGN16 ////Saftey guard att man inte får fler på allginment, avslutar structen på 16 så nästa kan räknas in rätt
struct cbFrameObj//Använder XMFloat4x4 för XMMatrix lätt blir messy med minnes fel ect
{
	dx::XMFLOAT4X4 wvp;//Perform offsett and rotation 4 bits per
	dx::XMFLOAT4X4 world;

	//Pixel shader obj loader tutorial
	dx::XMFLOAT4 difColor;
	bool hasTexture;

	//Old things we never used anyway
	//dx::XMFLOAT4X4 scale;
	//dx::XMFLOAT4X4 rotation;
	//dx::XMFLOAT4X4 view;

	//aling all the way 
};
ALIGN16
struct cbFrameLight
{
	dx::XMFLOAT3 pos;
	float pad1;;

	dx::XMFLOAT3 ambient;
	float pad2;

	dx::XMFLOAT3 color;
	float pad3;

	dx::XMFLOAT3 camPos;
	float pad4;

	float specPower;
	float attConst;
	float attLin;
	float attQuad;
};

ALIGN16
struct SurfaceMaterial 
{
	std::wstring matName;
	dx::XMFLOAT4 difColor;
	int textArrayIndex;
	bool hasTexture;
};