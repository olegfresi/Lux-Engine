/*
 * Project: TestProject
 * File: Mouse.hpp
 * Author: olegfresi
 * Created: 08/03/25 19:57
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
#include <map>
#include <vector>
#include "../Math/Vector.hpp"
#include "../Event/EventSystem.hpp"
#include "../include/Event/Events.hpp"
#include "InputDevice.hpp"

namespace lux
{
    using namespace math;
    using namespace event;

    class Mouse : public InputDevice
    {
    public:

        Mouse(NonOwnPtr<EventDispatcher> dispatcher, NonOwnPtr<ActionMapper> mapper);
        ~Mouse();

        Mouse(const Mouse&) = delete;
        Mouse& operator=(Mouse&) = delete;

        [[nodiscard]] static bool IsButtonDown(MouseButtons button);
        [[nodiscard]] static bool IsButtonDown(int key);
        [[nodiscard]] static bool IsButtonUp(MouseButtons button);
        [[nodiscard]] static bool IsButtonUp(int key);
        [[nodiscard]] static std::vector<uint8_t> ButtonsDown();
        [[nodiscard]] static std::vector<uint8_t> ButtonsUp();

        void static SetMouseDisplacement(const Vector2f& displacement = Vector2f{}) {}
        void static SetMouseScroll(const Vector2f& scroll = Vector2f{}) {}

        [[nodiscard]] static bool IsActionActive(const InputAction& action);
        void RegisterListeners() const;

        static void OnMouseDown(const MouseDownEvent& e) {}
        static void OnMouseRelease(const MouseReleaseEvent& e) {}
        void OnMouseMotion(const MouseMotionEvent& e) const;
        void OnMouseScroll(const MouseScrollEvent& e) const;

    private:
        static inline std::bitset<static_cast<size_t>(MouseButtons::MouseButtonLast)> m_buttonsDown;
        static inline std::bitset<static_cast<size_t>(MouseButtons::MouseButtonLast)> m_buttonsUp;

        uint32_t m_listenerId = TypeID<Mouse>();
        static inline std::bitset<static_cast<size_t>(InputAction::ActionLast)> s_activeActions;
    };
}