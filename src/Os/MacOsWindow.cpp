#include "../../include/Os/MacOsWindow.hpp"
#include "../../include/Application/Assertion.hpp"
#include "../../include/Application/EngineSettings.hpp"
#include "../../include/Utils/ImageLoad.hpp"
#include "../../include/Event/WindowEvent.hpp"
#include "../../include/Event/Events.hpp"

#include <cmath>


namespace lux
{
    using namespace event;

    MacOsWindow::MacOsWindow(const WindowSettings &settings, NonOwnPtr<EventDispatcher> dispatcher) : m_dispatcher {dispatcher }, m_settings { settings }
    {
        if (!glfwInit())
        {
            CORE_CRITICAL( "Failed to initialize GLFW");
            glfwTerminate();
        }

        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        glfwWindowHint( GLFW_SAMPLES, WindowSamples );
        glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_TRUE);

        m_window = glfwCreateWindow( m_settings.width, m_settings.height, m_settings.title.c_str(), nullptr, nullptr );
        if (!m_window)
        {
            CORE_CRITICAL( "Failed to create GLFW window");
            glfwTerminate();
        }

        m_renderContext = GraphicsContext::Create( m_window );
        m_renderContext->Init();

        if (glewInit() != GLEW_OK)
        {
            CORE_CRITICAL( "Failed to initialize GLEW" );
            glfwTerminate();
        }

        int width, height;
        glfwSetWindowUserPointer( m_window, this );
        glfwSetKeyCallback( m_window, KeyCallback );
        glfwSetMouseButtonCallback( m_window, MouseButtonCallback );
        glfwSetCursorPosCallback( m_window, CursorPosCallback );
        glfwSetScrollCallback( m_window, ScrollCallback );
        glfwSetFramebufferSizeCallback( m_window, FramebufferSizeCallback );
        glfwSetWindowCloseCallback( m_window, WindowCloseCallback );
        glfwSetWindowFocusCallback( m_window, WindowFocusCallback );
        glfwSetWindowSizeCallback( m_window, WindowSizeCallback );
        glfwSetWindowIconifyCallback( m_window, WindowIconifyCallback );
        glfwSetJoystickCallback( JoystickCallback );
        glfwSetInputMode( m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
        glfwGetFramebufferSize( m_window, &width, &height );

        m_dispatcher->RegisterListener<WindowCloseEvent>( m_listenerId, [this](auto) { Close(); } );
        m_dispatcher->RegisterListener<WindowResizeEvent>( m_listenerId, [this](auto event)
        {
            Resize( event.m_width, event.m_height);
        });

        for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++)
        {
            glfwSetJoystickUserPointer(jid, this);

            if (glfwJoystickPresent(jid))
            {
                auto& state = m_gamepadStates[jid];
                state.connected = true;
                state.isGamepad = glfwJoystickIsGamepad(jid);

                if (state.isGamepad)
                {
                    INFO("Gamepad detected: {}", glfwGetGamepadName(jid));
                    m_dispatcher->PostEvent<GamepadConnectedEvent>(jid);
                }
                else
                {
                    INFO("Joystick detected: {}", glfwGetJoystickName(jid));
                    m_dispatcher->PostEvent<JoystickConnectedEvent>(jid);
                }
            }
        }

        INFO( "Window created" );
    }

    NonOwnPtr<EventDispatcher> MacOsWindow::GetEventDispatcher() const
    {
        if (!m_dispatcher)
            throw std::runtime_error( "Dispatcher is null" );
        else
            return m_dispatcher;
    }

    void MacOsWindow::ProcessEvents() const
    {
        glfwPollEvents();
    }

    void MacOsWindow::Resize(unsigned int newWidth, unsigned int newHeight) const noexcept
    {
        glfwSetWindowSize( m_window, newWidth, newHeight );
    }

    void MacOsWindow::Close() const noexcept
    {
        glfwSetWindowShouldClose( m_window, true );
    }

    void MacOsWindow::SetFullScreen(bool enabled) const noexcept
    {
        if (enabled)
        {
            const GLFWvidmode *mode = glfwGetVideoMode( glfwGetPrimaryMonitor());
            glfwSetWindowMonitor( m_window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate );
        }
        else
            glfwSetWindowMonitor( m_window, nullptr, 0, 0, m_settings.width, m_settings.height, 0 );
    }

    bool MacOsWindow::IsFullScreen() const noexcept
    {
        return false;
    }

    int MacOsWindow::GetWidth() const noexcept
    {
        return m_settings.width;
    }

    int MacOsWindow::GetHeight() const noexcept
    {
        return m_settings.height;
    }

    std::string MacOsWindow::GetTitle() const noexcept
    {
        return m_settings.title;
    }

    void MacOsWindow::SetCursorVisibility(bool visible) const noexcept
    {
        if (visible)
            glfwSetInputMode( m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
        else
            glfwSetInputMode( m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
    }

    const Scope<GraphicsContext>& MacOsWindow::GetRenderContext() const noexcept
    {
        return m_renderContext;
    }

    void* MacOsWindow::GetNativeWindowHandle() const noexcept
    {
        return static_cast<GLFWwindow *>(m_window);
    }

    void MacOsWindow::Update() const noexcept
    {
        PollGamepads();
    }

    void MacOsWindow::PollGamepads() const
    {
        //std::cout << "Gamepad Name: " << glfwGetGamepadName(0) << std::endl;
        for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++)
        {
            auto& state = m_gamepadStates[jid];

            if (!state.connected) continue;

            if (state.isGamepad)
            {
                GLFWgamepadstate currentState;
                if (glfwGetGamepadState(jid, &currentState))
                {
                    for (int button = 0; button <= GLFW_GAMEPAD_BUTTON_LAST; button++)
                    {
                        bool currentPressed = currentState.buttons[button] == GLFW_PRESS;
                        bool previousPressed = state.buttonStates[button];

                        if (currentPressed && !previousPressed)
                            m_dispatcher->PostEvent<GamepadButtonPressEvent>(jid, static_cast<GamepadKeys>(button));
                        else if (!currentPressed && previousPressed)
                            m_dispatcher->PostEvent<GamepadButtonReleaseEvent>(jid, static_cast<GamepadKeys>(button));

                        state.buttonStates[button] = currentPressed;
                    }

                    constexpr float deadzone = 0.1f;
                    for (int axis = 0; axis <= GLFW_GAMEPAD_AXIS_LAST; axis++)
                    {
                        float currentValue = currentState.axes[axis];
                        float previousValue = state.axisValues[axis];

                        if (std::abs(currentValue) < deadzone)
                            currentValue = 0.0f;

                        if (std::abs(currentValue - previousValue) > 0.05f)
                        {
                            m_dispatcher->PostEvent<GamepadAxisEvent>(jid, axis, currentValue);
                            state.axisValues[axis] = currentValue;
                        }
                    }
                }
            }
            else  /* Case of joystick */
            {
                int buttonCount;
                const unsigned char* buttons = glfwGetJoystickButtons(jid, &buttonCount);

                for (int button = 0; button < buttonCount && button < JOYSTICK_BUTTON_MAX; button++)
                {
                    bool currentPressed = buttons[button] == GLFW_PRESS;
                    bool previousPressed = state.buttonStates[button];

                    if (currentPressed && !previousPressed)
                        m_dispatcher->PostEvent<JoystickButtonPressEvent>(jid, button);
                    else if (!currentPressed && previousPressed)
                        m_dispatcher->PostEvent<JoystickButtonReleaseEvent>(jid, button);

                    state.buttonStates[button] = currentPressed;
                }

                int axisCount;
                const float* axes = glfwGetJoystickAxes(jid, &axisCount);

                constexpr float deadzone = 0.1f;
                for (int axis = 0; axis < axisCount && axis < JOYSTICK_AXIS_MAX; axis++)
                {
                    float currentValue = axes[axis];
                    float previousValue = state.axisValues[axis];

                    if (std::abs(currentValue) < deadzone)
                        currentValue = 0.0f;

                    if (std::abs(currentValue - previousValue) > 0.05f)
                    {
                        m_dispatcher->PostEvent<JoystickAxisMotionEvent>(jid, axis, currentValue);
                        state.axisValues[axis] = currentValue;
                    }
                }
            }
        }
    }

    bool MacOsWindow::ShouldClose() const noexcept
    {
        return glfwWindowShouldClose( m_window );
    }

    void MacOsWindow::KeyCallback(GLFWwindow *window, int key, int, int action, int)
    {
        MacOsWindow *self = static_cast<MacOsWindow *>(glfwGetWindowUserPointer( window ));

        if (key >= 0 && key <= GLFW_KEY_LAST)
        {
            switch (action)
            {
                case GLFW_RELEASE:
                    self->m_dispatcher->PostEvent<KeyReleaseEvent>( static_cast<KeyboardKeys>(key));
                    break;

                case GLFW_PRESS:
                    self->m_dispatcher->PostEvent<KeyPressEvent>( static_cast<KeyboardKeys>(key), action );
                    break;

                case GLFW_REPEAT:
                    self->m_dispatcher->PostEvent<KeyRepeatEvent>( static_cast<KeyboardKeys>(key));
                    break;

                default:
                    assert(false);
            }
        }
    }

    void MacOsWindow::MouseButtonCallback(GLFWwindow *window, int button, int action, int)
    {
        if (auto *win = static_cast<MacOsWindow *>(glfwGetWindowUserPointer(window)))
        {
            if (action == GLFW_PRESS)
                win->m_dispatcher->PostEvent<MouseDownEvent>( static_cast<MouseButtons>(button));

            else if (action == GLFW_RELEASE)
                win->m_dispatcher->PostEvent<MouseReleaseEvent>( static_cast<MouseButtons>(button));
        }
    }

    void MacOsWindow::CursorPosCallback(GLFWwindow *window, double xPos, double yPos)
    {
        if (auto *win = static_cast<MacOsWindow*>(glfwGetWindowUserPointer(window)))
            win->m_dispatcher->PostEvent<MouseMotionEvent>( xPos, yPos );
    }

    void MacOsWindow::ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
    {
        if (auto *win = static_cast<MacOsWindow*>(glfwGetWindowUserPointer(window)))
        {
            if (glfwGetInputMode(window, GLFW_RAW_MOUSE_MOTION) == GLFW_TRUE)
            {
                double scale = 1.0 + (yOffset * 0.1);
                win->m_dispatcher->PostEvent<TrackpadPanEvent>(xOffset, yOffset, scale);
            }
            else
                win->m_dispatcher->PostEvent<MouseScrollEvent>(xOffset, yOffset, MouseButtons::MouseCursorUpDown);
        }
    }

    void MacOsWindow::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
        if (auto *win = static_cast<MacOsWindow*>(glfwGetWindowUserPointer(window)))
            win->m_dispatcher->PostEvent<WindowResizeEvent>( width, height );
    }

    void MacOsWindow::WindowCloseCallback(GLFWwindow *window)
    {
        if (auto *win = static_cast<MacOsWindow*>(glfwGetWindowUserPointer(window)))
            win->m_dispatcher->PostEvent<WindowCloseEvent>();
    }

    void MacOsWindow::JoystickCallback(int jid, int event)
    {
        auto win = static_cast<MacOsWindow*>(glfwGetJoystickUserPointer(jid));

        if (win == nullptr)
        {
            GLFWwindow* currentWindow = glfwGetCurrentContext();
            if (currentWindow != nullptr)
            {
                win = static_cast<MacOsWindow*>(glfwGetWindowUserPointer(currentWindow));
                if (win != nullptr)
                    glfwSetJoystickUserPointer(jid, win);
            }

            if (win == nullptr)
            {
                ERROR("Impossibile to handle joystick event");
                return;
            }
        }

        if (event == GLFW_CONNECTED)
        {
            if (glfwJoystickIsGamepad(jid))
            {
                INFO("Gamepad connected: {}", glfwGetGamepadName(jid));
                win->m_gamepadStates[jid].connected = true;
                win->m_gamepadStates[jid].isGamepad = true;

                win->m_dispatcher->PostEvent<JoystickConnectedEvent>(jid);
            }
            else
            {
                INFO("Joystick connected: {}", glfwGetJoystickName(jid));
                win->m_gamepadStates[jid].connected = true;
                win->m_gamepadStates[jid].isGamepad = false;

                win->m_dispatcher->PostEvent<JoystickConnectedEvent>(jid);
            }
        }
        else if (event == GLFW_DISCONNECTED)
        {
            INFO("Joystick disconnected: {}", jid);
            win->m_gamepadStates[jid].connected = false;
            win->m_gamepadStates[jid].isGamepad = false;
            win->m_dispatcher->PostEvent<JoystickDisconnectedEvent>(jid);
        }
    }

    void MacOsWindow::WindowFocusCallback(GLFWwindow*, int) {}

    void MacOsWindow::WindowIconifyCallback(GLFWwindow*, int) {}

    void MacOsWindow::WindowSizeCallback(GLFWwindow *window, int width, int height)
    {

        if (auto *win = static_cast<MacOsWindow*>(glfwGetWindowUserPointer(window)))
            win->m_dispatcher->PostEvent<WindowResizeEvent>( width, height );
    }

    void MacOsWindow::VSync(bool enabled) const noexcept
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
    }

    void MacOsWindow::SetWindowIcon(std::filesystem::path iconPath)
    {
        int width, height, channels;
        if (auto* data = LoadImageFromPath(iconPath, &width, &height, &channels, 4))
        {
            GLFWimage images[1];
            images[0].width = width;
            images[0].height = height;
            images[0].pixels = data;

            glfwSetWindowIcon(m_window, 1, images);

            FreeImage(data);
        }
    }
}
