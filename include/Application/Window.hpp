/*
 * Project: TestProject
 * File: Window.hpp
 * Author: olegfresi
 * Created: 11/02/25 21:46
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
#include <string>
#include "../Math/Vector.hpp"
#include "../Event/EventSystem.hpp"

namespace lux
{
    struct WindowSettings
    {
        unsigned int width;
        unsigned int height;
        std::string title;
        bool isFullScreen;
    };

    class OpenGLContext;
    class GraphicsContext;

    class WindowImpl
    {
    public:
        virtual ~WindowImpl() = default;

        virtual void Resize(unsigned int newWidth, unsigned int newHeight) const noexcept = 0;

        virtual void Close() const noexcept = 0;

        virtual void ProcessEvents() const = 0;

        virtual void SetFullScreen(bool enabled) const noexcept = 0;

        virtual void SetCursorVisibility(bool visible) const noexcept = 0;

        virtual void VSync(bool enabled) const noexcept = 0;

        virtual void* GetNativeWindowHandle() const noexcept = 0;

        virtual int GetWidth() const noexcept = 0;

        virtual int GetHeight() const noexcept = 0;

        virtual bool IsFullScreen() const noexcept = 0;

        virtual bool ShouldClose() const noexcept = 0;

        virtual const Scope <GraphicsContext> &GetRenderContext() const noexcept = 0;

        virtual std::string GetTitle() const noexcept = 0;

        virtual void SetWindowIcon(std::filesystem::path iconPath) = 0;
    };

    class Window
    {
    public:
        Window(const WindowSettings &settings, [[maybe_unused]] NonOwnPtr <EventDispatcher> dispatcher) :
        m_settings{ settings }, m_window{ CreateWindow(settings, dispatcher) } {}

        virtual ~Window() = default;

        unsigned int GetWidth() const { return m_window->GetWidth(); }
        unsigned int GetHeight() const { return m_window->GetHeight(); }
        int GetSize() const { return m_window->GetWidth() * m_window->GetHeight(); }
        std::string GetTitle() const { return m_window->GetTitle(); }

        void ProcessEvents() const;

        void Resize(unsigned int newWidth, unsigned int newHeight) const noexcept { m_window->Resize(newWidth, newHeight); }
        void Close() const noexcept { m_window->Close(); }
        void SetFullScreen(bool enabled) const noexcept { m_window->SetFullScreen(enabled); }
        void SetCursorVisibility(bool visible) const noexcept { m_window->SetCursorVisibility(visible); }
        void VSync(bool enabled) const noexcept { m_window->VSync(enabled); }
        void* GetNativeWindowHandle() const noexcept { return m_window->GetNativeWindowHandle(); }
        const Scope <GraphicsContext> &GetRenderContext() const noexcept { return m_window->GetRenderContext(); }
        [[nodiscard]] bool IsFullScreen() const noexcept { return m_window->IsFullScreen(); }
        [[nodiscard]] bool ShouldClose() const noexcept { return m_window->ShouldClose(); }
        void SetWindowIcon(const std::filesystem::path& iconPath) const noexcept{ m_window->SetWindowIcon(iconPath); }

    private:
        WindowSettings m_settings;
        Scope<WindowImpl> m_window = nullptr;

        Scope <WindowImpl> CreateWindow(const WindowSettings &settings, NonOwnPtr <EventDispatcher> dispatcher);
    };
}
