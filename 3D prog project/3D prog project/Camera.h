#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <string>
#include <iostream>
#include "SimpleMath.h"

namespace dx = DirectX; 
namespace sm = DirectX::SimpleMath;

class Camera
{
private:
	dx::XMVECTOR pos = dx::XMVectorSet(0.0f, 0.0f, -3, 0.0f);
	dx::XMVECTOR startPos;
	dx::XMVECTOR target = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	dx::XMVECTOR up = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	dx::XMMATRIX view = dx::XMMatrixIdentity();
	dx::XMMATRIX pp = dx::XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)1000 / 800, 1.0f, 1000.0f);
	dx::XMMATRIX camRotationMatrix;

	dx::XMVECTOR DefaultForward = dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	dx::XMVECTOR DefaultRight = dx::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	dx::XMVECTOR camForward = dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	dx::XMVECTOR camRight = dx::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	sm::Vector3 yawPitchRoll = { 0.0f,0.0f,0.0f };

	float moveLeftRight = 0.0f;
	float moveBackFoward = 0.0f;
	float speed = 1.f;
	float width, height;

public:
	Camera();
	Camera(float width, float height);
	~Camera();
	void Update();
	void SetPos(float x, float y, float z);
	void SetStartPos();
	void moveLeft(float speed);
	void moveRight(float speed);
	void movefoward(float speed);
	void moveback(float speed);
	dx::XMFLOAT4X4 getView();
	dx::XMFLOAT4X4 getPP();
	dx::XMFLOAT3 getPos();
	sm::Vector3 getYawPitchRoll()const { return this->yawPitchRoll; }
	void setYawPitchRoll(sm::Vector3 newRollPichyawValue);
	void subYawPitchRoll(sm::Vector3 rollpitchYaw);
};