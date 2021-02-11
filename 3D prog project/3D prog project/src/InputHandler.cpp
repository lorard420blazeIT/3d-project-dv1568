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

void InputHandler::SetUp(Window* window)
{
	this->height = window->getHeight();
	this->width = window->getWidth();

	keyboard = std::make_unique<dx::Keyboard>();
	mouse = std::make_unique<dx::Mouse>();
	mouse->SetWindow(window->getHWND());
}
