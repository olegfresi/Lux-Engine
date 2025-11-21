/*
 * Project: TestProject
 * File: Vector4.hpp
 * Author: olegfresi
 * Created: 07/02/25 18:40
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
#include <type_traits>
#include "Vector3.hpp"

namespace lux::math
{
    template<typename T> requires std::is_arithmetic_v <T>
    class Vector4
    {
    public:
        static constexpr size_t N = 4;

        constexpr Vector4() noexcept : x{}, y{}, z{}, w{} {}

        constexpr Vector4(T x, T y, T z, T w) noexcept : x{x}, y{y}, z{z}, w{w} {}

        explicit constexpr Vector4(const Vector3<T>& vec, T scalar) noexcept:
                                  x(vec.GetX()), y(vec.GetY()), z(vec.GetZ()), w(scalar) {}

        explicit constexpr Vector4(T value) noexcept : x{value}, y{value}, z{value}, w{value} {}

        explicit constexpr Vector4(const Vector2<T>& v1, const Vector2<T>& v2) noexcept :
                                    x(v1.GetX()), y(v1.GetY()), z(v2.GetX()), w(v2.GetY()) {}

        constexpr Vector4(const Vector4& other) noexcept : x(other.x), y(other.y), z(other.z), w(other.w) {}

        constexpr Vector4(Vector4&& other) noexcept : x(std::move(other.x)), y(std::move(other.y)),
                                                      z(std::move(other.z)), w(std::move(other.w)) {}

        constexpr Vector4 &operator=(const Vector4 &other) noexcept
        {
            if (this != &other)
            {
                x = other.x;
                y = other.y;
                z = other.z;
                w = other.w;
            }

            return *this;
        }

        constexpr Vector4& operator=(Vector4 &&other) noexcept
        {
            if (this != &other)
            {
                x = other.x;
                y = other.y;
                z = other.z;
                w = other.w;
            }

            return *this;
        }

        constexpr T& operator[](size_t index)
        {
            if(index >= N)
                throw std::out_of_range("Index out of range");

            return ((&x)[index]);
        }
        constexpr const T &operator[](size_t index) const
        {
            if(index >= N)
                throw std::out_of_range("Index out of range");

            return ((&x)[index]);
        }

        constexpr T GetX() const noexcept { return x; }
        constexpr T GetY() const noexcept { return y; }
        constexpr T GetZ() const noexcept { return z; }
        constexpr T GetW() const noexcept { return w; }

        constexpr Vector4 operator-() const noexcept { return Vector4{ -x, -y, -z, -w }; }

        constexpr Vector4 operator+(const Vector4 &other) const noexcept
        {
            return Vector4{ x + other.x, y + other.y, z + other.z, w + other.w };
        }

        constexpr Vector4 operator-(const Vector4 &other) const noexcept
        {
            return Vector4{ x - other.x, y - other.y, z - other.z, w - other.w };
        }

        constexpr Vector4 operator*(T scalar) const noexcept
        {
            return Vector4{ x * scalar, y * scalar, z * scalar, w * scalar };
        }

        constexpr Vector4 operator/(T scalar) const
        {
            if (scalar == T(0))
                throw std::runtime_error("Division by zero");

            return Vector4{ x / scalar, y / scalar, z / scalar, w / scalar };
        }

        constexpr Vector4 &operator+=(const Vector4 &other) noexcept
        {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;

            return *this;
        }

        constexpr Vector4 &operator-=(const Vector4 &other) noexcept
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;

            return *this;
        }

        constexpr Vector4 &operator*=(T scalar) noexcept
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;

            return *this;
        }

        constexpr Vector4 &operator/=(T scalar)
        {
            if (scalar == T(0))
                throw std::runtime_error("Division by zero");

            x /= scalar;
            y /= scalar;
            z /= scalar;
            w /= scalar;

            return *this;
        }

        [[nodiscard]] constexpr bool operator==(const Vector4& other) const noexcept
        {
            return x == other.x && y == other.y && z == other.z && w == other.w;
        }

        [[nodiscard]] constexpr bool operator!=(const Vector4& other) const noexcept
        {
            return *this != other;
        }

        constexpr T Dot(const Vector4& other) const noexcept
        {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        constexpr T Length() const noexcept { return std::sqrt(Dot(*this)); }

        static constexpr void Normalize(Vector4& vec)
        {
            T len = vec.Length();
            if (len == T(0)) return;
            vec /= len;
        }

    private:

        struct
        {
            T x, y, z, w;
        };
    };
}
