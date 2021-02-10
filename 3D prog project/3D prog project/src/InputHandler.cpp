#include "InputHandler.h"

InputHandler::InputHandler()
{
	width = 10;
	height = 10;
}

InputHandler::InputHandler(int width, int height)
	:width(width), height(height)
{
	keyboard = std::make_unique<dx::Keyboard>();
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
