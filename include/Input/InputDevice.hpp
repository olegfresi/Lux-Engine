/*
 * Project: TestProject
 * File: InputEvent.hpp
 * Author: olegfresi
 * Created: 03/03/25 18:16
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
#include "../include/Event/EventSystem.hpp"
#include "../include/Application/Pointers.hpp"
#include "../include/Input/ActionMapper.hpp"
#include <set>
#include <bitset>

namespace lux
{
    class InputDevice
    {
    public:
        InputDevice(NonOwnPtr<EventDispatcher> dispatcher, NonOwnPtr<ActionMapper> mapper);
        ~InputDevice() = default;

        static bool IsActionActive(InputAction action) noexcept;

        static bool IsKeyDown(InputType input, int key) noexcept;
        static bool IsKeyUp(InputType input, int key) noexcept;

    protected:
        NonOwnPtr<EventDispatcher> m_dispatcher;
        NonOwnPtr<ActionMapper> m_mapper;

        static inline std::set<InputKey> s_currentlyActiveKeys;
        static inline std::bitset<static_cast<size_t>(InputAction::ActionLast)> s_activeActions;

    private:
        static inline std::vector<NonOwnPtr<InputDevice>> s_devices;
    };
}