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

	//std::cout << mouseState.leftButton << std::endl;

	if(mouseState.positionMode== dx::Mouse::MODE_RELATIVE)
	{
		//std::cout << delta.x << delta.y << std::endl;
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

