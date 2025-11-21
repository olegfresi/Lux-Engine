/*
 * Project: TestProject
 * File: MathUtils.hpp
 * Author: olegfresi
 * Created: 07/02/25 18:57
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
#include <numbers>

namespace lux::math
{
    constexpr double EPSILON = 1e-8;

    [[nodiscard]] constexpr double DegToRad(float deg)
    {
        return deg * std::numbers::pi / 180;
    }

    [[nodiscard]]  constexpr double RadToDeg(float rad)
    {
        return rad * 180 / std::numbers::pi;
    }

    struct Angle
    {
        float angle;
    };

    template <typename T>
    T Clamp(T value, T min, T max)
    {
        return value < min ? min : value > max ? max : value;
    }

    template <typename T>
    T Lerp(T a, T b, T t)
    {
        return a + t * (b - a);
    }

    struct MathException
    {
        explicit MathException(const std::string& message)
        {
            throw std::runtime_error(message);
        }
    };
}