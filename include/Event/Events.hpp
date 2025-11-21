/*
 * Project: TestProject
 * File: Events.hpp
 * Author: olegfresi
 * Created: 03/03/25 19:36
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
#include "../Input/InputDeviceKeys.hpp"
#include "../Input/ActionMapper.hpp"

namespace lux::event
{
    // ========== Keyboard Events ================ //
    struct KeyReleaseEvent
    {
        KeyReleaseEvent(KeyboardKeys key) : m_key { key } {}

        KeyboardKeys m_key = KeyboardKeys::KeyUnknown;
    };

    struct KeyPressEvent
    {
        KeyPressEvent(KeyboardKeys key, int32_t action) : m_key { key }, m_action { action } {}

        KeyboardKeys m_key = KeyboardKeys::KeyUnknown;
        int32_t m_action = -1;
    };

    struct KeyRepeatEvent
    {
        KeyRepeatEvent(KeyboardKeys key) : m_key { key } {}

        KeyboardKeys m_key = KeyboardKeys::KeyUnknown;
    };

    // ========== Mouse Events ================ //
    struct MouseReleaseEvent
    {
        MouseReleaseEvent(MouseButtons button) : m_button { button } {}

        MouseButtons m_button = MouseButtons::Unknown;
    };

    struct MouseDownEvent
    {
        MouseDownEvent(MouseButtons button) : m_button { button } {}

        MouseButtons m_button = MouseButtons::Unknown;
    };

    struct MouseDragEvent
    {
        MouseDragEvent(double dx, double dy) : m_dx { dx }, m_dy { dy } {}

        double m_dx {}, m_dy {};
    };

    struct MouseMotionEvent
    {
        MouseMotionEvent(double x, double y) : m_targetX { x }, m_targetY { y } {}

        double m_targetX {}, m_targetY {};
        MouseButtons m_button = MouseButtons::MouseCursorMoveAround;
    };

    struct MouseScrollEvent
    {
        MouseScrollEvent(double sx, double sy, MouseButtons button) : m_scrollX { sx }, m_scrollY { sy }, m_button { button } {}

        double m_scrollX {}, m_scrollY {};
        MouseButtons m_button = MouseButtons::Unknown;
    };

    struct TrackpadPanEvent
    {
        TrackpadPanEvent(double dx, double dy, double scale) : m_dx { dx }, m_dy { dy }, m_scale { scale } {}

        double m_dx {};
        double m_dy {};
        double m_scale = 1.0;
    };

    // ========== Joystick Events ================ //
    struct JoystickConnectedEvent
    {
        JoystickConnectedEvent(int joystickId) : m_joystickId { joystickId } {}

        int m_joystickId;
    };

    struct JoystickDisconnectedEvent
    {
        JoystickDisconnectedEvent(int joystickId) : m_joystickId { joystickId } {}

        int m_joystickId;
    };

    struct JoystickButtonPressEvent
    {
        JoystickButtonPressEvent(int joystick, int button) : m_joystick(joystick), m_button(button) {}

        int m_joystick;
        int m_button;
    };

    struct JoystickButtonReleaseEvent
    {
        JoystickButtonReleaseEvent(int joystick, int button) : m_joystick(joystick), m_button(button) {}

        int m_joystick;
        int m_button;
    };

    struct JoystickAxisMotionEvent
    {
        JoystickAxisMotionEvent(int joystick, int axis, float value) : m_joystick(joystick), m_axis(axis), m_value(value) {}

        int m_joystick;
        int m_axis;
        float m_value;
    };

    // ========== Gamepad Events ================ //
    struct GamepadConnectedEvent
    {
        explicit GamepadConnectedEvent(int gamepadId) : m_gamepadId { gamepadId } {}

        int m_gamepadId;
    };

    struct GamepadDisconnectedEvent
    {
        explicit GamepadDisconnectedEvent(int gamepadId) : m_gamepadId { gamepadId } {}

        int m_gamepadId;
    };
    struct GamepadButtonPressEvent
    {
        GamepadButtonPressEvent(int gamepad, GamepadKeys button) : m_gamepad(gamepad), m_button(button) {}

        int m_gamepad;
        GamepadKeys m_button;
    };

    struct GamepadButtonReleaseEvent
    {
        GamepadButtonReleaseEvent(int gamepad, GamepadKeys button) : m_gamepad(gamepad), m_button(button) {}

        int m_gamepad;
        GamepadKeys m_button;
    };

    struct GamepadAxisEvent
    {
        GamepadAxisEvent(int gamepad, int axis, float value) : m_gamepad(gamepad), m_axis(axis), m_value(value) {}

        int m_gamepad;
        int m_axis;
        float m_value;
    };

    struct GamepadLookEvent
    {
        GamepadLookEvent(float rightX, float rightY, double deltaTime) : m_rightX(rightX), m_rightY(rightY), m_deltaTime(deltaTime) {}

        float m_rightX;
        float m_rightY;
        double m_deltaTime;
    };

    struct GamepadMoveEvent
    {
        GamepadMoveEvent(float leftX, float leftY, double deltaTime) : m_leftX(leftX), m_leftY(leftY), m_deltaTime(deltaTime) {}

        float m_leftX;
        float m_leftY;
        double m_deltaTime;
    };

    struct GamepadZoomEvent
    {
        GamepadZoomEvent(float triggerValue, double deltaTime) : m_triggerValue(triggerValue), m_deltaTime(deltaTime) {}

        float m_triggerValue;
        double m_deltaTime;
    };

    struct ActionEvent
    {
        explicit ActionEvent(InputAction action, bool active = false) : m_action{action}, m_xOffset{0.0f}, m_yOffset{0.0f}, m_active{active} {}

        ActionEvent(InputAction action, float xOffset, float yOffset) : m_action(action), m_xOffset(xOffset), m_yOffset(yOffset) {}

        InputAction m_action;
        float m_xOffset;
        float m_yOffset;
        bool m_active = false;
    };
}