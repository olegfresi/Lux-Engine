/*
 * Project: TestProject
 * File: Action.hpp
 * Author: olegfresi
 * Created: 03/03/25 19:01
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
#include <unordered_set>
#include <set>
#include <map>
#include <ranges>

namespace lux
{
    enum class InputAction : uint8_t
    {
        MoveForward = 0,
        MoveBackward,
        MoveLeft,
        MoveRight,
        MoveUpDown,
        MoveAround,
        SprintForward,
        Pan,
        Zoom,
        ActionLast = Zoom + 1
    };

    enum class InputType : uint8_t
    {
        Keyboard,
        Mouse,
        Joystick,
        Gamepad
    };


    struct InputKey
    {
        InputType type;
        int key;

        bool operator==(const InputKey& other) const
        {
            return type == other.type && key == other.key;
        }

        bool operator<(const InputKey& other) const
        {
            return std::tie(type, key) < std::tie(other.type, other.key);
        }
    };

    class ActionMapper
    {
    public:

        void BindKeyCombo(InputAction action, const std::vector<InputType>& types, const std::vector<int>& keys)
        {
            if (types.size() != keys.size())
                throw std::invalid_argument("Types and keys must match in size");

            std::set<InputKey> combo;
            for (size_t i = 0; i < types.size(); ++i)
                combo.insert({ types[i], keys[i] });

            m_comboToAction[combo] = action;
        }

        std::optional<InputAction> GetAction(const std::vector<InputKey>& pressedKeys) const
        {
            std::set<InputKey> keySet(pressedKeys.begin(), pressedKeys.end());

            auto it = m_comboToAction.find(keySet);
            if (it != m_comboToAction.end())
                return it->second;

            return std::nullopt;
        }

        std::vector<InputAction> GetAllActions() const noexcept
        {
            auto values = m_comboToAction | std::views::values;
            return {values.begin(), values.end()};
        }

        bool IsComboActive(InputAction action, const std::set<InputKey>& activeKeys) const
        {
            for (const auto& [combo, mappedAction] : m_comboToAction)
            {
                if (mappedAction != action)
                    continue;

                bool allKeysActive = std::ranges::all_of(combo.begin(), combo.end(),
                                                 [&](const InputKey& k) {
                                                     return activeKeys.contains(k);
                                                 });

                if (allKeysActive)
                    return true;
            }

            return false;
        }

        std::vector<std::pair<std::set<InputKey>, InputAction>> GetAllMappings() const
        {
            std::vector<std::pair<std::set<InputKey>, InputAction>> mappings;
            for (const auto& pair : m_comboToAction)
                mappings.push_back(pair);

            return mappings;
        }

        InputKey GetKeyCombo(InputAction action, InputType type) const
        {
            for (const auto& [combo, mappedAction] : m_comboToAction)
            {
                if (mappedAction != action)
                    continue;

                for (const auto& key : combo)
                {
                    if (key.type == type)
                        return key;
                }
            }

            return {};
        }

    private:
        std::map<std::set<InputKey>, InputAction> m_comboToAction;
    };
}
