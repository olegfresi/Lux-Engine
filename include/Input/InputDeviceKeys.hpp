/*
 * Project: TestProject
 * File: InputDevice.hpp
 * Author: olegfresi
 * Created: 02/03/25 19:52
 * 
 * Copyright © 2025 olegfresi
 * 
 * Licensed under the MIT License. You may obtain a copy of the License at:
 * 
 *     https://opensource.org/licenses/MIT
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once
#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"
#include <cstdint>
#include <string>

enum class MouseButtons : uint32_t
{
    MouseButton1 = GLFW_MOUSE_BUTTON_1,
    MouseButton2 = GLFW_MOUSE_BUTTON_2,
    MouseButton3 = GLFW_MOUSE_BUTTON_3,
    MouseButton4 = GLFW_MOUSE_BUTTON_4,
    MouseButton5 = GLFW_MOUSE_BUTTON_5,
    MouseButton6 = GLFW_MOUSE_BUTTON_6,
    MouseButton7 = GLFW_MOUSE_BUTTON_7,
    MouseButton8 = GLFW_MOUSE_BUTTON_8,
    MouseButtonLeft = GLFW_MOUSE_BUTTON_LEFT,
    MouseButtonRight = GLFW_MOUSE_BUTTON_RIGHT,
    MouseButtonMiddle = GLFW_MOUSE_BUTTON_MIDDLE,
    MouseButtonLast = GLFW_MOUSE_BUTTON_LAST + 4,
    MouseCursorMoveAround = GLFW_MOUSE_BUTTON_LAST + 1,
    MouseCursorUpDown = GLFW_MOUSE_BUTTON_LAST + 2,
    Unknown = GLFW_MOUSE_BUTTON_LAST + 3
};

enum class KeyboardKeys : int32_t
{
    KeyUnknown = GLFW_KEY_UNKNOWN,

    KeyA = GLFW_KEY_A,
    KeyB = GLFW_KEY_B,
    KeyC = GLFW_KEY_C,
    KeyD = GLFW_KEY_D,
    KeyE = GLFW_KEY_E,
    KeyF = GLFW_KEY_F,
    KeyG = GLFW_KEY_G,
    KeyH = GLFW_KEY_H,
    KeyI = GLFW_KEY_I,
    KeyJ = GLFW_KEY_J,
    KeyK = GLFW_KEY_K,
    KeyL = GLFW_KEY_L,
    KeyM = GLFW_KEY_M,
    KeyN = GLFW_KEY_N,
    KeyO = GLFW_KEY_O,
    KeyP = GLFW_KEY_P,
    KeyQ = GLFW_KEY_Q,
    KeyR = GLFW_KEY_R,
    KeyS = GLFW_KEY_S,
    KeyT = GLFW_KEY_T,
    KeyU = GLFW_KEY_U,
    KeyV = GLFW_KEY_V,
    KeyW = GLFW_KEY_W,
    KeyX = GLFW_KEY_X,
    KeyY = GLFW_KEY_Y,
    KeyZ = GLFW_KEY_Z,

    Key0 = GLFW_KEY_0,
    Key1 = GLFW_KEY_1,
    Key2 = GLFW_KEY_2,
    Key3 = GLFW_KEY_3,
    Key4 = GLFW_KEY_4,
    Key5 = GLFW_KEY_5,
    Key6 = GLFW_KEY_6,
    Key7 = GLFW_KEY_7,
    Key8 = GLFW_KEY_8,
    Key9 = GLFW_KEY_9,

    KeySpace = GLFW_KEY_SPACE,
    KeyApostrophe = GLFW_KEY_APOSTROPHE,
    KeyComma = GLFW_KEY_COMMA,
    KeyMinus = GLFW_KEY_MINUS,
    KeyPeriod = GLFW_KEY_PERIOD,
    KeySlash = GLFW_KEY_SLASH,
    KeySemicolon = GLFW_KEY_SEMICOLON,
    KeyEqual = GLFW_KEY_EQUAL,
    KeyLeftBracket = GLFW_KEY_LEFT_BRACKET,
    KeyBackslash = GLFW_KEY_BACKSLASH,
    KeyRightBracket = GLFW_KEY_RIGHT_BRACKET,
    KeyGraveAccent = GLFW_KEY_GRAVE_ACCENT,
    KeyWorld1 = GLFW_KEY_WORLD_1,
    KeyWorld2 = GLFW_KEY_WORLD_2,

    KeyEscape = GLFW_KEY_ESCAPE,
    KeyEnter = GLFW_KEY_ENTER,
    KeyPress = GLFW_PRESS,
    KeyTab = GLFW_KEY_TAB,
    KeyBackspace = GLFW_KEY_BACKSPACE,
    KeyInsert = GLFW_KEY_INSERT,
    KeyDelete = GLFW_KEY_DELETE,
    KeyRight = GLFW_KEY_RIGHT,
    KeyLeft = GLFW_KEY_LEFT,
    KeyDown = GLFW_KEY_DOWN,
    KeyUp = GLFW_KEY_UP,
    KeyPageUp = GLFW_KEY_PAGE_UP,
    KeyPageDown = GLFW_KEY_PAGE_DOWN,
    KeyHome = GLFW_KEY_HOME,
    KeyEnd = GLFW_KEY_END,
    KeyCapsLock = GLFW_KEY_CAPS_LOCK,
    KeyScrollLock = GLFW_KEY_SCROLL_LOCK,
    KeyNumLock = GLFW_KEY_NUM_LOCK,
    KeyPrintScreen = GLFW_KEY_PRINT_SCREEN,
    KeyPause = GLFW_KEY_PAUSE,

    KeyF1 = GLFW_KEY_F1,
    KeyF2 = GLFW_KEY_F2,
    KeyF3 = GLFW_KEY_F3,
    KeyF4 = GLFW_KEY_F4,
    KeyF5 = GLFW_KEY_F5,
    KeyF6 = GLFW_KEY_F6,
    KeyF7 = GLFW_KEY_F7,
    KeyF8 = GLFW_KEY_F8,
    KeyF9 = GLFW_KEY_F9,
    KeyF10 = GLFW_KEY_F10,
    KeyF11 = GLFW_KEY_F11,
    KeyF12 = GLFW_KEY_F12,
    KeyF13 = GLFW_KEY_F13,
    KeyF14 = GLFW_KEY_F14,
    KeyF15 = GLFW_KEY_F15,
    KeyF16 = GLFW_KEY_F16,
    KeyF17 = GLFW_KEY_F17,
    KeyF18 = GLFW_KEY_F18,
    KeyF19 = GLFW_KEY_F19,
    KeyF20 = GLFW_KEY_F20,
    KeyF21 = GLFW_KEY_F21,
    KeyF22 = GLFW_KEY_F22,
    KeyF23 = GLFW_KEY_F23,
    KeyF24 = GLFW_KEY_F24,
    KeyF25 = GLFW_KEY_F25,

    KeyKp0 = GLFW_KEY_KP_0,
    KeyKp1 = GLFW_KEY_KP_1,
    KeyKp2 = GLFW_KEY_KP_2,
    KeyKp3 = GLFW_KEY_KP_3,
    KeyKp4 = GLFW_KEY_KP_4,
    KeyKp5 = GLFW_KEY_KP_5,
    KeyKp6 = GLFW_KEY_KP_6,
    KeyKp7 = GLFW_KEY_KP_7,
    KeyKp8 = GLFW_KEY_KP_8,
    KeyKp9 = GLFW_KEY_KP_9,
    KeyKpDecimal = GLFW_KEY_KP_DECIMAL,
    KeyKpDivide = GLFW_KEY_KP_DIVIDE,
    KeyKpMultiply = GLFW_KEY_KP_MULTIPLY,
    KeyKpSubtract = GLFW_KEY_KP_SUBTRACT,
    KeyKpAdd = GLFW_KEY_KP_ADD,
    KeyKpEnter = GLFW_KEY_KP_ENTER,
    KeyKpEqual = GLFW_KEY_KP_EQUAL,

    KeyLeftShift = GLFW_KEY_LEFT_SHIFT,
    KeyLeftControl = GLFW_KEY_LEFT_CONTROL,
    KeyLeftAlt = GLFW_KEY_LEFT_ALT,
    KeyLeftSuper = GLFW_KEY_LEFT_SUPER,
    KeyRightShift = GLFW_KEY_RIGHT_SHIFT,
    KeyRightControl = GLFW_KEY_RIGHT_CONTROL,
    KeyRightAlt = GLFW_KEY_RIGHT_ALT,
    KeyRightSuper = GLFW_KEY_RIGHT_SUPER,
    KeyMenu = GLFW_KEY_MENU,

    KeyLast = GLFW_KEY_LAST
};

enum class JoystickKeys : uint32_t
{
    Joystick1 = GLFW_JOYSTICK_1,
    Joystick2 = GLFW_JOYSTICK_2,
    Joystick3 = GLFW_JOYSTICK_3,
    Joystick4 = GLFW_JOYSTICK_4,
    Joystick5 = GLFW_JOYSTICK_5,
    Joystick6 = GLFW_JOYSTICK_6,
    Joystick7 = GLFW_JOYSTICK_7,
    Joystick8 = GLFW_JOYSTICK_8,
    Joystick9 = GLFW_JOYSTICK_9,
    Joystick10 = GLFW_JOYSTICK_10,
    Joystick11 = GLFW_JOYSTICK_11,
    Joystick12 = GLFW_JOYSTICK_12,
    Joystick13 = GLFW_JOYSTICK_13,
    Joystick14 = GLFW_JOYSTICK_14,
    Joystick15 = GLFW_JOYSTICK_15,
    Joystick16 = GLFW_JOYSTICK_16,
    JoystickLast = GLFW_JOYSTICK_LAST
};

enum class GamepadKeys : uint32_t
{
    ButtonA = GLFW_GAMEPAD_BUTTON_A,
    ButtonB = GLFW_GAMEPAD_BUTTON_B,
    ButtonX = GLFW_GAMEPAD_BUTTON_X,
    ButtonY = GLFW_GAMEPAD_BUTTON_Y,
    ButtonLB = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
    ButtonRB = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
    ButtonBack = GLFW_GAMEPAD_BUTTON_BACK,
    ButtonStart = GLFW_GAMEPAD_BUTTON_START,
    ButtonGuide = GLFW_GAMEPAD_BUTTON_GUIDE,
    ButtonLT = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
    ButtonRT = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
    ButtonDpadUp = GLFW_GAMEPAD_BUTTON_DPAD_UP,
    ButtonDpadRight = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
    ButtonDpadDown = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
    ButtonDpadLeft = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,
    ButtonCross = ButtonA,
    ButtonCircle = ButtonB,
    ButtonSquare = ButtonX,
    ButtonTriangle = ButtonY,

    ButtonL1 = ButtonLB,
    ButtonR1 = ButtonRB,
    ButtonL3 = ButtonLT,
    ButtonR3 = ButtonRT,
    DPadUp = ButtonDpadUp,
    DPadDown = ButtonDpadDown,
    DPadLeft = ButtonDpadLeft,
    DPadRight = ButtonDpadRight,

    ButtonLast = GLFW_GAMEPAD_BUTTON_LAST,
    Unknown = ButtonLast + 1
};

enum class ButtonActionType : uint32_t
{
    ButtonPress = GLFW_PRESS,
    ButtonRelease = GLFW_RELEASE,
    ButtonRepeat = GLFW_REPEAT,
    ButtonUnknown = GLFW_RELEASE + 1
};

inline std::string GamepadKeyToString(GamepadKeys key)
{
    switch (key)
    {
        case GamepadKeys::ButtonA: return "A/Cross";
        case GamepadKeys::ButtonB: return "B/Circle";
        case GamepadKeys::ButtonX: return "X/Square";
        case GamepadKeys::ButtonY: return "Y/Triangle";
        case GamepadKeys::ButtonLB: return "LB/L1";
        case GamepadKeys::ButtonRB: return "RB/R1";
        case GamepadKeys::ButtonBack: return "Back/Share";
        case GamepadKeys::ButtonStart: return "Start/Options";
        case GamepadKeys::ButtonGuide: return "Guide/PS Button";
        case GamepadKeys::ButtonLT: return "Left Thumb/L3";
        case GamepadKeys::ButtonRT: return "Right Thumb/R3";
        case GamepadKeys::ButtonDpadUp: return "D-Pad Up";
        case GamepadKeys::ButtonDpadRight: return "D-Pad Right";
        case GamepadKeys::ButtonDpadDown: return "D-Pad Down";
        case GamepadKeys::ButtonDpadLeft: return "D-Pad Left";
        case GamepadKeys::Unknown: return "Unknown";

        default: return "Invalid Button (" + std::to_string(static_cast<int>(key)) + ")";
    }
}

enum class GamepadAxes : uint32_t
{
    AxisLeftX = GLFW_GAMEPAD_AXIS_LEFT_X,
    AxisLeftY = GLFW_GAMEPAD_AXIS_LEFT_Y,
    AxisRightX = GLFW_GAMEPAD_AXIS_RIGHT_X,
    AxisRightY = GLFW_GAMEPAD_AXIS_RIGHT_Y,
    AxisLT = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
    AxisRT = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,
    AxisLast = GLFW_GAMEPAD_AXIS_LAST
};

inline std::string GamepadAxisToString(GamepadAxes axis)
{
    switch (axis)
    {
        case GamepadAxes::AxisLeftX: return "Left Stick X";
        case GamepadAxes::AxisLeftY: return "Left Stick Y";
        case GamepadAxes::AxisRightX: return "Right Stick X";
        case GamepadAxes::AxisRightY: return "Right Stick Y";
        case GamepadAxes::AxisLT: return "Left Trigger";
        case GamepadAxes::AxisRT: return "Right Trigger";
        default: return "Invalid Axis (" + std::to_string(static_cast<int>(axis)) + ")";
    }
}