#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include <Windows.h>
#include <iostream>
#include <string>

namespace dx = DirectX;

class InputHandler
{
private:
    int width;
    int height;
    dx::Keyboard keyboard;
    dx::Keyboard::KeyboardStateTracker tracker;

public:
	InputHandler();
    InputHandler(int width, int height);
	~InputHandler();
    void Updater() {
        keyboard.GetState();
    }
    bool GetKeyDown(dx::Keyboard::Keys key) const 
    {
        return tracker.IsKeyPressed(key);
        
    }
    void test() {
        if (keyboard.GetState().IsKeyDown(dx::Keyboard::Space))
        {
            std::cout << "TEST input" << std::endl;
        }
    }

    //Singleton
    static InputHandler& Instance() 
    {
        static InputHandler instance;
        return instance;
    }

    InputHandler(InputHandler const&) = delete;
    void operator=(InputHandler const&) = delete;
};

/* void test() {
        if (InputHandler::Instance().GetKey(DirectX::Keyboard::E)) {
            std::cout << "TEST";
        }
    }*/