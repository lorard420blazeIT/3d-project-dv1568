#include "InputHandler.h"

InputHandler::InputHandler()
{

}

InputHandler::~InputHandler()
{
}

void InputHandler::Update()
{
	auto state = keyboard->GetState();
	keyTracker.Update(state);

	float rotationMouse = 0.004f;
	auto mouseState = mouse->GetState();
	if(mouseState.positionMode== dx::Mouse::MODE_RELATIVE)
	{
		delta = sm::Vector3(float(mouseState.x), float(mouseState.y), 0.f) * rotationMouse;
	}
	mouse->SetMode(mouseState.leftButton ? dx::Mouse::MODE_RELATIVE : dx::Mouse::MODE_ABSOLUTE);
}

bool InputHandler::KeyPressed(dx::Keyboard::Keys key)
{
	auto kb = keyboard->GetState();

	if (kb.IsKeyDown(key))
	{
		return true;
	}
	else return false;
}

void InputHandler::test()
{
	auto kb = keyboard->GetState();

	if (kb.Space)
	{
		std::cout << "TEST input" << std::endl;
	}

	/*if (keyboard->GetState().IsKeyDown(dx::Keyboard::Space))
	{
		std::cout << "TEST input" << std::endl;
	}*/
}

sm::Vector3 InputHandler::getdelta()
{
	return delta;
}





void InputHandler::SetUp(Window* window)
{
	this->height = window->getHeight();
	this->width = window->getWidth();

	keyboard = std::make_unique<dx::Keyboard>();
	mouse = std::make_unique<dx::Mouse>();
	mouse->SetWindow(window->getHWND());
	
}

