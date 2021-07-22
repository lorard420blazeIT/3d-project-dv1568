#include "Camera.h"

Camera::Camera()
{
	startPos = pos;
	view = dx::XMMatrixLookAtLH(pos, target, up);
}

Camera::Camera(float width, float height)
	:height(height), width(width)
{
	startPos = pos;
	view = dx::XMMatrixLookAtLH(pos, target, up);
}

Camera::~Camera()
{
}

void Camera::Update()
{
	camRotationMatrix = dx::XMMatrixRotationRollPitchYaw(yawPitchRoll.y, yawPitchRoll.x, 0);
	target = dx::XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	target = dx::XMVector3Normalize(target);

	camRight = dx::XMVector3TransformCoord(DefaultRight, camRotationMatrix);
	camForward = dx::XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	up = dx::XMVector3Cross(camForward, camRight);
	
	sm::Vector3 camRightTemp(dx::XMVectorGetX(camRight), dx::XMVectorGetY(camRight), dx::XMVectorGetZ(camRight));
	sm::Vector3 camForwardTemp(dx::XMVectorGetX(camForward), dx::XMVectorGetY(camForward), dx::XMVectorGetZ(camForward));


	camRightTemp = camRightTemp *moveLeftRight;
	camForwardTemp = camForwardTemp* moveBackFoward;

	camRight = dx::XMVectorSet(camRightTemp.x, camRightTemp.y, camRightTemp.z, 1);
	camForward = dx::XMVectorSet(camForwardTemp.x, camForwardTemp.y, camForwardTemp.z, 1);

	pos = dx::XMVectorAdd(pos, camRight);
	pos = dx::XMVectorAdd(pos, camForward);
	
	moveLeftRight = 0;
	moveBackFoward = 0;

	target = dx::XMVectorAdd(pos, target);
	view = dx::XMMatrixLookAtLH(pos, target, up);
}

void Camera::SetPos(float x, float y, float z)
{
	pos = dx::XMVectorSet(x, y, z, 0);
	view = dx::XMMatrixLookAtLH(pos, target, up);
}

void Camera::SetStartPos()
{
	pos = startPos;
	moveBackFoward = 0;
	moveLeftRight = 0;
}

void Camera::moveLeft(float speed)
{
	moveLeftRight -= speed;
}

void Camera::moveRight(float speed)
{
	moveLeftRight += speed;
}

void Camera::movefoward(float speed)
{
	moveBackFoward += speed;
}

void Camera::moveback(float speed)
{
	moveBackFoward -= speed;
}

dx::XMFLOAT4X4 Camera::getView()
{
	dx::XMFLOAT4X4 saveMe;
	dx::XMStoreFloat4x4(&saveMe, view);
	return saveMe;
}

dx::XMFLOAT4X4 Camera::getPP()
{
	dx::XMFLOAT4X4 saveMe;
	dx::XMStoreFloat4x4(&saveMe, pp);
	return saveMe;
}

dx::XMFLOAT3 Camera::getPos()
{
	dx::XMFLOAT3 saveMe;
	dx::XMStoreFloat3(&saveMe, pos);
	return saveMe;
}

void Camera::setYawPitchRoll(sm::Vector3 newRollPichyawValue)
{	
	this->yawPitchRoll = newRollPichyawValue;
}

void Camera::subYawPitchRoll(sm::Vector3 vec)
{
	this->yawPitchRoll -= vec;
}

