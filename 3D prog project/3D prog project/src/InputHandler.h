#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include "Keyboard.h"
#include "Mouse.h"
#include "Window.h"
#include "SimpleMath.h"
//#include <winrt\Windows.UI.Core.h>

namespace dx = DirectX;
namespace sm = dx::SimpleMath;
#define KEY(X) dx::Keyboard::X


class InputHandler
{
private:
    HWND win;
    int width;
    int height;
    std::unique_ptr<dx::Keyboard> keyboard;
    dx::Keyboard::KeyboardStateTracker keyTracker;
    std::unique_ptr<dx::Mouse> mouse;
    sm::Vector3 delta;
public:
    InputHandler();
    ~InputHandler();
    void SetUp(Window* window);
    void Update();
    bool KeyPressed(dx::Keyboard::Keys key);
    void test();
    sm::Vector3 getdelta();
   

};

/* void test() {
        if (InputHandler::Instance().GetKey(DirectX::Keyboard::E)) {
            std::cout << "TEST";
        }
    }*/