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
	//std::cout << "CamRight:  " << dx::XMVectorGetX(camRight) << dx::XMVectorGetY(camRight) << dx::XMVectorGetZ(camRight) << std::endl;
	//std::cout << "CamFoward: " << dx::XMVectorGetX(camForward) << dx::XMVectorGetY(camForward) << dx::XMVectorGetZ(camForward) << std::endl;

	camRotationMatrix = dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	target = dx::XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	target = dx::XMVector3Normalize(target);

	dx::XMMATRIX rotateYTTempMAtrix;
	rotateYTTempMAtrix = dx::XMMatrixRotationY(yaw);

	camRight = dx::XMVector3TransformCoord(DefaultRight, rotateYTTempMAtrix);
	camForward = dx::XMVector3TransformCoord(DefaultForward, rotateYTTempMAtrix);
	up = dx::XMVector3Cross(camForward, camRight);
	
	sm::Vector3 camRightTemp(dx::XMVectorGetX(camRight), dx::XMVectorGetY(camRight), dx::XMVectorGetZ(camRight));
	sm::Vector3 camForwardTemp(dx::XMVectorGetX(camForward), dx::XMVectorGetY(camForward), dx::XMVectorGetZ(camForward));

	//std::cout << "Camright temp: " << camRightTemp.x << camRightTemp.y << camRightTemp.z << std::endl;
	//std::cout << "Camfoward temp: " << camForwardTemp.x << camForwardTemp.y << camForwardTemp.z << std::endl;

	camRightTemp = camRightTemp *moveLeftRight;
	camForwardTemp = camForwardTemp* moveBackFoward;

	//std::cout << "Camright temp: " << camRightTemp.x << camRightTemp.y << camRightTemp.z << std::endl;
	//std::cout << "Camfoward temp: " << camForwardTemp.x << camForwardTemp.y << camForwardTemp.z << std::endl;

	camRight = dx::XMVectorSet(camRightTemp.x, camRightTemp.y, camRightTemp.z, 1);
	camForward = dx::XMVectorSet(camForwardTemp.x, camForwardTemp.y, camForwardTemp.z, 1);

	pos = dx::XMVectorAdd(pos, camRight);
	pos = dx::XMVectorAdd(pos, camForward);
	
	moveLeftRight = 0;
	moveBackFoward = 0;

	target = dx::XMVectorAdd(pos, target);
	view = dx::XMMatrixLookAtLH(pos, target, up);

	//std::cout << "CamRight:  " << dx::XMVectorGetX(camRight) << dx::XMVectorGetY(camRight) << dx::XMVectorGetZ(camRight) << std::endl;
	//std::cout << "CamFoward: " << dx::XMVectorGetX(camForward) << dx::XMVectorGetY(camForward) << dx::XMVectorGetZ(camForward) << std::endl;
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