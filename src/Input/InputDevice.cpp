#include "../../include/Input/InputDevice.hpp"
#include "../../include/Input/Mouse.hpp"
#include "../../include/Input/Keyboard.hpp"
#include "../../include/Application/Assertion.hpp"

namespace lux
{
    InputDevice::InputDevice(NonOwnPtr<EventDispatcher> dispatcher, NonOwnPtr<ActionMapper> mapper)
        : m_dispatcher(dispatcher), m_mapper(mapper)
    {
        s_devices.push_back(this);
    }

    bool InputDevice::IsActionActive(InputAction action) noexcept
    {
        return s_activeActions.test(static_cast<size_t>(action));
    }

    bool InputDevice::IsKeyDown(InputType input, int key) noexcept
    {
        InputKey k {input, key};

        return s_currentlyActiveKeys.contains(k);
    }

    bool InputDevice::IsKeyUp(InputType input, int key) noexcept
    {
        InputKey k {input, key};
        return !s_currentlyActiveKeys.contains(k);
    }
}