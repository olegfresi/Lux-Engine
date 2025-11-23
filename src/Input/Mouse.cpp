#include "../../include/Input/Mouse.hpp"

namespace lux
{
    Mouse::Mouse(NonOwnPtr<EventDispatcher> dispatcher, NonOwnPtr<ActionMapper> mapper) : InputDevice{ dispatcher, mapper}
    {
        RegisterListeners();
    }

    Mouse::~Mouse()
    {
        m_dispatcher->EraseListener(m_listenerId);
    }

    void Mouse::RegisterListeners() const
    {
        m_dispatcher->RegisterListener<MouseDownEvent>(m_listenerId,
                                                       [](const MouseDownEvent& e) { OnMouseDown(e); }
        );

        m_dispatcher->RegisterListener<MouseReleaseEvent>(m_listenerId,
                                                          [](const MouseReleaseEvent& e) { OnMouseRelease(e); }
        );

        m_dispatcher->RegisterListener<MouseMotionEvent>(m_listenerId,
                                                         [this](const MouseMotionEvent& e) { OnMouseMotion(e); }
        );
    }

    void Mouse::OnMouseMotion(const MouseMotionEvent& e) const
    {
        InputKey key = { InputType::Mouse, static_cast<int>(e.m_button) };
        auto action = m_mapper->GetAction({key});
        if (action.has_value())
            m_dispatcher->PostEvent<ActionEvent>(action.value(), e.m_targetX, e.m_targetY);
    }

    void Mouse::OnMouseScroll(const MouseScrollEvent& e) const
    {
        InputKey key = { InputType::Mouse, static_cast<int>(e.m_button) };
        auto action = m_mapper->GetAction({key});

        if (action.has_value())
            m_dispatcher->PostEvent<ActionEvent>(action.value(), e.m_scrollX, e.m_scrollY);
    }

    bool Mouse::IsActionActive(const InputAction& action)
    {
        return s_activeActions.test(static_cast<size_t>(action));
    }

    bool Mouse::IsButtonDown(MouseButtons button)
    {
        return m_buttonsDown.test(static_cast<size_t>(button));
    }

    bool Mouse::IsButtonDown(int key)
    {
        return m_buttonsDown.test(static_cast<size_t>(key));
    }

    bool Mouse::IsButtonUp(MouseButtons button)
    {
        return m_buttonsUp.test(static_cast<size_t>(button));
    }

    bool Mouse::IsButtonUp(int key)
    {
        return m_buttonsUp.test(static_cast<size_t>(key));
    }

    std::vector<uint8_t> Mouse::ButtonsDown()
    {
        std::vector<uint8_t> buttons;
        for (size_t i = 0; i < m_buttonsDown.size(); ++i)
            if (m_buttonsDown.test(i))
                buttons.push_back(static_cast<uint8_t>(i));

        return buttons;
    }

    std::vector<uint8_t> Mouse::ButtonsUp()
    {
        std::vector<uint8_t> buttons;
        for (size_t i = 0; i < m_buttonsDown.size(); ++i)
            if (m_buttonsUp.test(i))
                buttons.push_back(static_cast<uint8_t>(i));

        return buttons;
    }
}