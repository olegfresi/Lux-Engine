#include "../../include/Input/Gamepad.hpp"
#include "../../include/Event/WindowEvent.hpp"
#include "../../include/Os/OsDevice.hpp"
#include <iostream>

namespace lux
{
    Gamepad::Gamepad(NonOwnPtr<EventDispatcher> dispatcher, NonOwnPtr<ActionMapper> mapper) : InputDevice(dispatcher, mapper)
    {
        RegisterListeners();
    }

    Gamepad::~Gamepad()
    {
        m_dispatcher->EraseListener(m_listenerId);
    }

    bool Gamepad::IsActionActive(const InputAction& action)
    {
        return s_activeActions.test(static_cast<size_t>(action));
    }

    void Gamepad::RegisterListeners()
    {
        m_dispatcher->RegisterListener<GamepadButtonPressEvent>(m_listenerId,
                                                                [this](const GamepadButtonPressEvent& e) {
                                                                    OnButtonPress( e ); });

        m_dispatcher->RegisterListener<GamepadButtonReleaseEvent>(m_listenerId,
                                                                  [this](const GamepadButtonReleaseEvent& e) { OnButtonRelease(e); });

        m_dispatcher->RegisterListener<GamepadAxisEvent>(m_listenerId,
                                                        [this](const GamepadAxisEvent& e) { OnGamepadAxis(e); });

        m_dispatcher->RegisterListener<GamepadConnectedEvent>(m_listenerId,
                                                              [this](const GamepadConnectedEvent& e) { OnGamepadConnected(e); });

        m_dispatcher->RegisterListener<GamepadDisconnectedEvent>(m_listenerId,
                                                                 [this](const GamepadDisconnectedEvent& e) { OnGamepadDisconnected(e); });
    }

    void Gamepad::Update(double deltaTime)
    {
        if (IsGamepadPresent())
        {
            GamepadState state;
            if (GetGamepadState(&state))
            {
                float rightX = state.axes[static_cast<int>(GamepadAxes::AxisRightX)];
                float rightY = state.axes[static_cast<int>(GamepadAxes::AxisRightY)];

                const float deadzone = 0.2f;
                if (abs(rightX) > deadzone || abs(rightY) > deadzone)
                    m_dispatcher->PostEvent<GamepadLookEvent>(rightX, rightY, deltaTime);

                float leftX = state.axes[static_cast<int>(GamepadAxes::AxisLeftX)];
                float leftY = state.axes[static_cast<int>(GamepadAxes::AxisLeftY)];

                if (abs(leftX) > deadzone || abs(leftY) > deadzone)
                    m_dispatcher->PostEvent<GamepadMoveEvent>(leftX, leftY, deltaTime);

                float rt = state.axes[static_cast<int>(GamepadAxes::AxisRT)];
                float lt = state.axes[static_cast<int>(GamepadAxes::AxisLT)];

                if (rt > deadzone)
                    m_dispatcher->PostEvent<GamepadZoomEvent>(rt, deltaTime);
                else if (lt > deadzone)
                    m_dispatcher->PostEvent<GamepadZoomEvent>(-lt, deltaTime);

                static unsigned char lastStdButtons[static_cast<int>(GamepadKeys::ButtonLast) + 1] = {0};

                if (state.buttons[static_cast<int>(GamepadKeys::ButtonRB)] == static_cast<int>(ButtonActionType::ButtonPress) && lastStdButtons[static_cast<int>(GamepadKeys::ButtonRB)] == static_cast<int>(ButtonActionType::ButtonRelease))
                    m_dispatcher->PostEvent<GamepadButtonPressEvent>(0, GamepadKeys::ButtonR1);

                if (state.buttons[static_cast<int>(GamepadKeys::ButtonLB)] == static_cast<int>(ButtonActionType::ButtonPress) && lastStdButtons[static_cast<int>(GamepadKeys::ButtonLB)] == static_cast<int>(ButtonActionType::ButtonRelease))
                    m_dispatcher->PostEvent<GamepadButtonPressEvent>(0, GamepadKeys::ButtonL1);

                if (state.buttons[static_cast<int>(GamepadKeys::ButtonCross)] == static_cast<int>(ButtonActionType::ButtonPress) && lastStdButtons[static_cast<int>(GamepadKeys::ButtonCross)] == static_cast<int>(ButtonActionType::ButtonRelease))
                    m_dispatcher->PostEvent<GamepadButtonPressEvent>(0, GamepadKeys::ButtonA);

                if (state.buttons[static_cast<int>(GamepadKeys::ButtonCircle)] == static_cast<int>(ButtonActionType::ButtonPress) && lastStdButtons[static_cast<int>(GamepadKeys::ButtonCircle)] == static_cast<int>(ButtonActionType::ButtonRelease))
                    m_dispatcher->PostEvent<GamepadButtonPressEvent>(0, GamepadKeys::ButtonB);

                if (state.buttons[static_cast<int>(GamepadKeys::ButtonSquare)] == static_cast<int>(ButtonActionType::ButtonPress) && lastStdButtons[static_cast<int>(GamepadKeys::ButtonSquare)] == static_cast<int>(ButtonActionType::ButtonRelease))
                    m_dispatcher->PostEvent<GamepadButtonPressEvent>(0, GamepadKeys::ButtonX);

                if (state.buttons[static_cast<int>(GamepadKeys::ButtonTriangle)] == static_cast<int>(ButtonActionType::ButtonPress) && lastStdButtons[static_cast<int>(GamepadKeys::ButtonTriangle)] == static_cast<int>(ButtonActionType::ButtonRelease))
                    m_dispatcher->PostEvent<GamepadButtonPressEvent>(0, GamepadKeys::ButtonY);

                if (state.buttons[static_cast<int>(GamepadKeys::ButtonStart)] == static_cast<int>(ButtonActionType::ButtonPress) && lastStdButtons[static_cast<int>(GamepadKeys::ButtonStart)] == static_cast<int>(ButtonActionType::ButtonRelease))
                    m_dispatcher->PostEvent<GamepadButtonPressEvent>(0, GamepadKeys::ButtonStart);

                if (state.buttons[static_cast<int>(GamepadKeys::ButtonBack)] == static_cast<int>(ButtonActionType::ButtonPress) && lastStdButtons[static_cast<int>(GamepadKeys::ButtonBack)] == static_cast<int>(ButtonActionType::ButtonRelease))
                    m_dispatcher->PostEvent<GamepadButtonPressEvent>(0, GamepadKeys::ButtonBack);

                if (state.buttons[static_cast<int>(GamepadKeys::ButtonLT)] == static_cast<int>(ButtonActionType::ButtonPress) && lastStdButtons[static_cast<int>(GamepadKeys::ButtonLT)] == static_cast<int>(ButtonActionType::ButtonRelease))
                    m_dispatcher->PostEvent<GamepadButtonPressEvent>(0, GamepadKeys::ButtonL3);

                if (state.buttons[static_cast<int>(GamepadKeys::ButtonRT)] == static_cast<int>(ButtonActionType::ButtonPress) && lastStdButtons[static_cast<int>(GamepadKeys::ButtonRT)] == static_cast<int>(ButtonActionType::ButtonRelease))
                    m_dispatcher->PostEvent<GamepadButtonPressEvent>(0, GamepadKeys::ButtonR3);

                if (state.buttons[static_cast<int>(GamepadKeys::ButtonDpadUp)] == static_cast<int>(ButtonActionType::ButtonPress) && lastStdButtons[static_cast<int>(GamepadKeys::ButtonDpadUp)] == static_cast<int>(ButtonActionType::ButtonRelease))
                    m_dispatcher->PostEvent<GamepadButtonPressEvent>(0, GamepadKeys::DPadUp);

                if (state.buttons[static_cast<int>(GamepadKeys::ButtonDpadDown)] == static_cast<int>(ButtonActionType::ButtonPress) && lastStdButtons[static_cast<int>(GamepadKeys::ButtonDpadDown)] == static_cast<int>(ButtonActionType::ButtonRelease))
                    m_dispatcher->PostEvent<GamepadButtonPressEvent>(0, GamepadKeys::DPadDown);

                if (state.buttons[static_cast<int>(GamepadKeys::ButtonDpadLeft)] == static_cast<int>(ButtonActionType::ButtonPress) && lastStdButtons[static_cast<int>(GamepadKeys::ButtonDpadLeft)] == static_cast<int>(ButtonActionType::ButtonRelease))
                    m_dispatcher->PostEvent<GamepadButtonPressEvent>(0, GamepadKeys::DPadLeft);

                if (state.buttons[static_cast<int>(GamepadKeys::ButtonDpadRight)] == static_cast<int>(ButtonActionType::ButtonPress) && lastStdButtons[static_cast<int>(GamepadKeys::ButtonDpadRight)] == static_cast<int>(ButtonActionType::ButtonRelease))
                    m_dispatcher->PostEvent<GamepadButtonPressEvent>(0, GamepadKeys::DPadRight);

                for (int i = 0; i <= static_cast<int>(GamepadKeys::ButtonLast); i++)
                {
                    if (static_cast<ButtonActionType>(state.buttons[i]) == ButtonActionType::ButtonRelease && static_cast<ButtonActionType>(lastStdButtons[i]) == ButtonActionType::ButtonPress)
                    {
                        GamepadKeys key;
                        switch (static_cast<GamepadKeys>(i))
                        {
                            case GamepadKeys::ButtonA: key = GamepadKeys::ButtonA; break;
                            case GamepadKeys::ButtonB: key = GamepadKeys::ButtonB; break;
                            case GamepadKeys::ButtonX: key = GamepadKeys::ButtonX; break;
                            case GamepadKeys::ButtonY: key = GamepadKeys::ButtonY; break;
                            case GamepadKeys::ButtonLB: key = GamepadKeys::ButtonL1; break;
                            case GamepadKeys::ButtonRB: key = GamepadKeys::ButtonR1; break;
                            case GamepadKeys::ButtonBack: key = GamepadKeys::ButtonBack; break;
                            case GamepadKeys::ButtonStart: key = GamepadKeys::ButtonStart; break;
                            case GamepadKeys::ButtonLT: key = GamepadKeys::ButtonL3; break;
                            case GamepadKeys::ButtonRT: key = GamepadKeys::ButtonR3; break;
                            case GamepadKeys::ButtonDpadUp: key = GamepadKeys::DPadUp; break;
                            case GamepadKeys::ButtonDpadRight: key = GamepadKeys::DPadRight; break;
                            case GamepadKeys::ButtonDpadDown: key = GamepadKeys::DPadDown; break;
                            case GamepadKeys::ButtonDpadLeft: key = GamepadKeys::DPadLeft; break;
                            default: key = GamepadKeys::Unknown; break;
                        }

                        m_dispatcher->PostEvent<GamepadButtonReleaseEvent>(0, key);
                    }
                }

                for (int i = 0; i <= static_cast<int>(GamepadKeys::ButtonLast); i++)
                    lastStdButtons[i] = state.buttons[i];
            }
        }
    }

    void Gamepad::OnButtonPress(const GamepadButtonPressEvent& e)
    {
        if (e.m_button == GamepadKeys::ButtonBack)
            m_dispatcher->PostEvent<WindowCloseEvent>();

        InputKey key = { InputType::Gamepad, static_cast<int>(e.m_button) };
        auto action = m_mapper->GetAction({key});

        if (action)
        {
            m_dispatcher->PostEvent<ActionEvent>( action.value(), true );
            s_activeActions.set( static_cast<size_t>(action.value()));
        }
    }

    void Gamepad::OnButtonRelease(const GamepadButtonReleaseEvent& e)
    {
        InputKey key = { InputType::Gamepad, static_cast<int>(e.m_button) };
        auto action = m_mapper->GetAction({key});

        if (action)
        {
            s_activeActions.reset( static_cast<size_t>(action.value()));
            m_dispatcher->PostEvent<ActionEvent>( action.value(), false );
        }
    }

    void Gamepad::OnGamepadAxis(const GamepadAxisEvent& e)
    {
        InputKey key = { InputType::Gamepad, e.m_axis };
        auto action = m_mapper->GetAction({key});

        if (action)
        {
            m_dispatcher->PostEvent<ActionEvent>( action.value(), true );
            s_activeActions.set( static_cast<size_t>(action.value()));
        }
    }

    void Gamepad::OnGamepadConnected(const GamepadConnectedEvent&)
    {
        std::cout << "Gamepad Connected ✅" << std::endl;
    }

    void Gamepad::OnGamepadDisconnected(const GamepadDisconnectedEvent&)
    {
        std::cout << "Gamepad Disconnected ❌" << std::endl;
    }
}