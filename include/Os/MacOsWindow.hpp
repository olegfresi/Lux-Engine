/*
 * Project: TestProject
 * File: OpenGLWindow.hpp
 * Author: olegfresi
 * Created: 28/02/25 15:16
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
#include "../Renderer/Common/GraphicsContext.hpp"
#include "../Input/InputDevice.hpp"
#include "../OpenGL/OpenglError.hpp"
#include <unordered_map>
#include "GLFW/glfw3.h"

namespace lux
{
    class MacOsWindow : public WindowImpl
    {
    public:

        MacOsWindow(const WindowSettings& settings, NonOwnPtr<EventDispatcher> dispatcher);
        ~MacOsWindow() override = default;

        NonOwnPtr<EventDispatcher> GetEventDispatcher() const;
        void ProcessEvents() const override;
        void Resize(unsigned int newWidth, unsigned int newHeight) const noexcept override;
        void Close() const noexcept override;
        void SetFullScreen(bool enabled) const noexcept override;
        void SetCursorVisibility(bool visible) const noexcept override;
        void VSync(bool enabled) const noexcept override;
        void Update() const noexcept;
        void* GetNativeWindowHandle() const noexcept override;
        int GetWidth() const noexcept override;
        int GetHeight() const noexcept override;
        [[nodiscard]] bool IsFullScreen() const noexcept override;
        [[nodiscard]] bool ShouldClose() const noexcept override;
        std::string GetTitle() const noexcept override;
        const Scope<GraphicsContext>& GetRenderContext() const noexcept override;

    private:
        void PollGamepads() const;

        static void KeyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, [[maybe_unused]] int mods);
        static void CursorPosCallback(GLFWwindow* window, double xPos, double yPos);
        static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void WindowCloseCallback(GLFWwindow* window);
        static void JoystickCallback(int joystick, int event);
        static void WindowSizeCallback(GLFWwindow *window, int width, int height);
        static void WindowIconifyCallback(GLFWwindow *window, int iconified);
        static void WindowFocusCallback(GLFWwindow *window, int focused);
        void SetWindowIcon(std::filesystem::path iconPath) override;

        static constexpr int JOYSTICK_BUTTON_MAX = 32;
        static constexpr int JOYSTICK_AXIS_MAX = 16;

        struct GamepadState
        {
            bool connected = false;
            bool isGamepad = false;
            bool buttonStates[JOYSTICK_BUTTON_MAX] = {false};
            float axisValues[JOYSTICK_AXIS_MAX] = {0.0f};
        };

        NonOwnPtr<GLFWwindow> m_window = nullptr;
        NonOwnPtr<EventDispatcher> m_dispatcher;
        Scope<GraphicsContext> m_renderContext;
        WindowSettings m_settings;
        uint32_t m_listenerId = typeid(this).hash_code(); // TODO: change using hashed strings
        mutable std::unordered_map<int, GamepadState> m_gamepadStates;
    };
}
