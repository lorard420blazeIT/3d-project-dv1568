#include "Camera.h"

Camera::Camera()
{
	startPos = pos;
	view = dx::XMMatrixLookAtLH(pos, target, up);
}

Camera::Camera(HWND win, float width, float height)
	:win(win), height(height), width(width)
{
	startPos = pos;
	view = dx::XMMatrixLookAtLH(pos, target, up);
}

Camera::~Camera()
{
}

void Camera::Update()
{
	////Keyboard and mouse stuff
	//auto kb = keyboard->GetState();
	////auto m = mouse->GetState();

	///*if (input.KeyDown(dx::Keyboard::Home))
	//{
	//	pos = startPos;
	//	pitch = yaw = 0;
	//	std::cout << "Home pressed" << std::endl;

	//}*/

	//dx::XMFLOAT3 move = { 0,0,0 };

	//if (kb.Up || kb.W)
	//{
	//	move.y += speed;
	//	std::cout << "up" << std::endl;
	//}

	//if (kb.Down || kb.S)
	//{
	//	move.y -= speed;
	//}

	//if (kb.Left || kb.A)
	//{
	//	move.x += speed;
	//}

	//if (kb.Right || kb.D)
	//{
	//	move.x -= speed;
	//}
	//
	////sm::Quaternion q = dx::XMQuaternionRotationRollPitchYaw(roll, pitch, yaw);
	//
	//pos = dx::XMVectorSet(move.x, move.y, move.z, 0);

	//view = dx::XMMatrixLookAtLH(pos, target, up);
}

void Camera::SetPos(float x, float y, float z)
{
	pos = dx::XMVectorSet(x, y, z, 0);
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