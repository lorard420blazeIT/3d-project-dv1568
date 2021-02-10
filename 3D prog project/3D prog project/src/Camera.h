#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <string>
#include <iostream>
//#include "InputHandler.h"
//#include "SimpleMath.h"

namespace dx = DirectX; 
//namespace sm = DirectX::SimpleMath;


class Camera
{
private:
	float x = 0.f;
	float y = 0.f;
	float z = -3.f;

	dx::XMVECTOR pos = dx::XMVectorSet(0.0f, 0.0f, -3, 0.0f);
	dx::XMVECTOR startPos;
	dx::XMVECTOR target = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	dx::XMVECTOR up = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	dx::XMMATRIX view = dx::XMMatrixIdentity();
	dx::XMMATRIX pp = dx::XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)1000 / 800, 1.0f, 1000.0f);
	float yaw = 0;
	float pitch = 0;
	float roll = 0;

	//std::unique_ptr<dx::Keyboard> keyboard;
	//std::unique_ptr<dx::Mouse> mouse;
	float speed = 1.f;
	float width, height;

public:
	Camera();
	Camera(float width, float height);
	~Camera();
	void Update();
	void SetPos(float x, float y, float z);
	void move(float x, float y, float z);
	dx::XMFLOAT4X4 getView();
	dx::XMFLOAT4X4 getPP();
	dx::XMFLOAT3 getPos();
};
