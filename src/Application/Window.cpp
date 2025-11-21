#include "../../include/Application/Window.hpp"
#include "../../include/Os/MacOsWindow.hpp"
#include "../../include/Application/Pointers.hpp"
#include "../../include/Application/EngineSettings.hpp"

namespace lux
{
    Scope<WindowImpl> Window::CreateWindow(const WindowSettings& settings, NonOwnPtr<EventDispatcher> dispatcher)
    {
        switch(OperatingSystem)
        {
            case Os::MACOS:
                return CreateScope<MacOsWindow>(settings, dispatcher);
            case Os::WINDOWS:
                return CreateScope<MacOsWindow>(settings, dispatcher);
            case Os::LINUX:
                return CreateScope<MacOsWindow>(settings, dispatcher);
        }
    }

    void Window::ProcessEvents() const
    {
        m_window->ProcessEvents();
    }
}