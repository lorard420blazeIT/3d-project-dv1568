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

}

void Camera::SetPos(float x, float y, float z)
{
	pos = dx::XMVectorSet(x, y, z, 0);
	view = dx::XMMatrixLookAtLH(pos, target, up);
}

void Camera::move(float x, float y, float z)
{
	pos = dx::XMVectorSet(this->x + x, this->y + y , this->z + z, 0);
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