#include "../../include/Input/Keyboard.hpp"
#include "../../include/Event/Events.hpp"
#include "../../include/Event/WindowEvent.hpp"

namespace lux
{
    using namespace event;

    Keyboard::Keyboard(NonOwnPtr<EventDispatcher> dispatcher, NonOwnPtr<ActionMapper> mapper) : InputDevice(dispatcher, mapper)
    {
        RegisterListeners();
    }

    Keyboard::~Keyboard()
    {
        m_dispatcher->EraseListener(m_listenerId);
    }

    void Keyboard::RegisterListeners() const
    {
        m_dispatcher->RegisterListener<KeyPressEvent>(m_listenerId,
            [this](const KeyPressEvent& e) { OnKeyPress(e); }
        );

        m_dispatcher->RegisterListener<KeyReleaseEvent>(m_listenerId,
            [this](const KeyReleaseEvent& e) { OnKeyRelease(e); }
        );
    }


    void Keyboard::OnKeyPress(const KeyPressEvent& e) const
    {
        if (e.m_key == KeyboardKeys::KeyEscape && static_cast<KeyboardKeys>(e.m_action) == KeyboardKeys::KeyPress)
            m_dispatcher->PostEvent<WindowCloseEvent>();

        InputKey key = { InputType::Keyboard, static_cast<int>(e.m_key) };
        s_currentlyActiveKeys.insert( key );

        std::vector pressedKeys(s_currentlyActiveKeys.begin(), s_currentlyActiveKeys.end());

        if (auto action = m_mapper->GetAction(pressedKeys))
        {
            m_dispatcher->PostEvent<ActionEvent>( action.value(), true );
            s_activeActions.set( static_cast<size_t>(action.value()));
        }
    }

    void Keyboard::OnKeyRelease(const KeyReleaseEvent& e) const
    {
        InputKey key = { InputType::Keyboard, static_cast<int>(e.m_key) };
        s_currentlyActiveKeys.erase(key);

        std::vector pressedKeys(s_currentlyActiveKeys.begin(), s_currentlyActiveKeys.end());

        if (auto action = m_mapper->GetAction(pressedKeys))
        {
            s_activeActions.reset(static_cast<size_t>(action.value()));
            m_dispatcher->PostEvent<ActionEvent>(action.value(), false);
        }
    }
}