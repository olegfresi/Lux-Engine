/*
 * Project: LuxEngine
 * File: ShaderLocation.hpp
 * Author: olegfresi
 * Created: 26/11/25 11:32
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
#include <string>
#include <map>


class ShaderLocationBinding
{
public:

    ShaderLocationBinding() = default;

    static void Bind(uint32_t location, std::string name = "")
    {
       m_locations.insert({name, location});
    }

    static bool ContainsBindingByName(const std::string& name)
    {
        return m_locations.contains(name);
    }

    static std::optional<std::string> IsLocationBound(uint32_t location)
    {
        if (m_bindings.contains(location))
            return std::optional(m_bindings[location]);

        for (const auto& [key, val] : m_locations)
            if (val == location)
            {
                m_bindings.insert({location, key});
                return std::optional(key);
            }

        return  std::nullopt;
    }

private:

    static inline std::map<std::string, uint32_t> m_locations;
    static inline std::map<uint32_t, std::string> m_bindings;
};