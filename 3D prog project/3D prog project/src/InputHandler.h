#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include "Keyboard.h"
#include "Mouse.h"
#include "Window.h"
#include "SimpleMath.h"

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
    bool UpdateMouse();
    bool KeyPressed(dx::Keyboard::Keys key);
    sm::Vector3 getdelta() const { return this->delta; }
};