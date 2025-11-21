#include "../../include/Input/Keyboard.hpp"
#include "../../include/Event/Events.hpp"
#include "../../include/Event/WindowEvent.hpp"
#include "../../include/Application/Assertion.hpp"

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

    void Keyboard::RegisterListeners()
    {
        m_dispatcher->RegisterListener<KeyPressEvent>(m_listenerId,
            [this](const KeyPressEvent& e) { OnKeyPress(e); }
        );

        m_dispatcher->RegisterListener<KeyReleaseEvent>(m_listenerId,
            [this](const KeyReleaseEvent& e) { OnKeyRelease(e); }
        );
    }


    void Keyboard::OnKeyPress(const KeyPressEvent& e)
    {
        if (e.m_key == KeyboardKeys::KeyEscape && static_cast<KeyboardKeys>(e.m_action) == KeyboardKeys::KeyPress)
            m_dispatcher->PostEvent<WindowCloseEvent>();

        InputKey key = { InputType::Keyboard, static_cast<int>(e.m_key) };
        s_currentlyActiveKeys.insert( key );

        std::vector<InputKey> pressedKeys(s_currentlyActiveKeys.begin(), s_currentlyActiveKeys.end());

        auto action = m_mapper->GetAction( pressedKeys );
        if (action)
        {
            m_dispatcher->PostEvent<ActionEvent>( action.value(), true );
            s_activeActions.set( static_cast<size_t>(action.value()));
        }
    }

    void Keyboard::OnKeyRelease(const KeyReleaseEvent& e)
    {
        InputKey key = { InputType::Keyboard, static_cast<int>(e.m_key) };
        s_currentlyActiveKeys.erase(key);

        std::vector<InputKey> pressedKeys(s_currentlyActiveKeys.begin(), s_currentlyActiveKeys.end());

        auto action = m_mapper->GetAction(pressedKeys);
        if (action)
        {
            s_activeActions.reset(static_cast<size_t>(action.value()));
            m_dispatcher->PostEvent<ActionEvent>(action.value(), false);
        }
    }
}