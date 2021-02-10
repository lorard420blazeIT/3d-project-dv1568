#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include "Keyboard.h"
#include "Mouse.h"
//#include <winrt\Windows.UI.Core.h>

namespace dx = DirectX;
#define KEY(X) dx::Keyboard::X

class InputHandler
{
private:
    int width;
    int height;
    std::unique_ptr<dx::Keyboard> keyboard;
    dx::Keyboard::KeyboardStateTracker keyTracker;

public:
    InputHandler();
    InputHandler(int width, int height);
    ~InputHandler();
    void Update();
    bool KeyPressed(dx::Keyboard::Keys key);
    void test();

};

/* void test() {
        if (InputHandler::Instance().GetKey(DirectX::Keyboard::E)) {
            std::cout << "TEST";
        }
    }*/