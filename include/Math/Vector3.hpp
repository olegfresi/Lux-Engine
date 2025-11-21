/*
 * Project: TestProject
 * File: Vector3.hpp
 * Author: olegfresi
 * Created: 02/02/25 19:53
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
#include <array>
#include "Vector2.hpp"

namespace lux::math
{
    template <typename T> requires std::is_arithmetic_v<T>
    class Vector3
    {
    public:
        constexpr Vector3() : m_comp { std::array<T, N> {}}
        {}

        explicit constexpr Vector3(T x, T y, T z) noexcept: x( x ), y( y ), z( z )
        {}

        explicit constexpr Vector3(const Vector2<T> &vec, T scalar) noexcept: x( vec.GetX()), y( vec.GetY()),
                                                                              z( scalar )
        {}

        explicit constexpr Vector3(const std::initializer_list<T> &comps)
        {
            if (comps.size() != N)
                throw std::invalid_argument( "Invalid number of components" );

            std::copy( comps.begin(), comps.end(), m_comp.begin());
        }

        Vector3(std::initializer_list<T> &&comps)
        {
            if (comps.size() != N)
                throw std::invalid_argument( "Invalid number of components" );

            std::move( comps.begin(), comps.end(), m_comp.begin());
        }

        constexpr Vector3(const Vector3 &other) noexcept: m_comp { other.m_comp }
        {}

        constexpr Vector3(Vector3 &&other) noexcept: m_comp { std::move( other.m_comp ) }
        {}

        constexpr Vector3 &operator=(const Vector3 &other) noexcept
        {
            if (this != &other)
                m_comp = other.m_comp;

            return *this;
        }

        constexpr Vector3 &operator=(Vector3 &&other) noexcept
        {
            if (this != &other)
                m_comp = std::move( other.m_comp );

            return *this;
        }

        constexpr T& operator[](size_t index)
        {
            if (index >= N)
            {
                CORE_ERROR( "Out of range access: index {0} for Vector3", index );
                throw std::out_of_range( "Index out of range in vector3 access" );
            }
            else
                return m_comp[index];
        }

        constexpr const T& operator[](size_t index) const
        {
            if (index >= N)
            {
                CORE_ERROR( "Out of range access: index {0} for Vector3", index );
                throw std::out_of_range( "Index out of range in vector3 access" );
            }
            else
                return m_comp[index];
        }

        static constexpr Vector3 Normalize(Vector3& vec)
        {
            T length = vec.Length();
            for (size_t i { 0 }; i < N; ++i)
                vec[i] /= length;

            return vec;
        }

        constexpr Vector3 Normalize()
        {
            T length = Length();
            for (size_t i { 0 }; i < N; ++i)
                m_comp[i] /= length;
            return *this;
        }

        [[nodiscard]] constexpr T Length() const noexcept
        {
            return std::sqrt( m_comp[0] * m_comp[0] + m_comp[1] * m_comp[1] + m_comp[2] * m_comp[2] );
        }

        [[nodiscard]] constexpr static T Length(const Vector3 &vec) noexcept
        {
            return vec.Length();
        }

        [[nodiscard]] constexpr T Dot(const Vector3 &other) const noexcept
        {
            return m_comp[0] * other.m_comp[0] + m_comp[1] * other.m_comp[1] + m_comp[2] * other.m_comp[2];
        }

        [[nodiscard]] constexpr bool operator==(const Vector3 &other) const
        {
            return m_comp[0] == other.m_comp[0] && m_comp[1] == other.m_comp[1] && m_comp[2] == other.m_comp[2];
        }

        [[nodiscard]] constexpr bool operator!=(const Vector3 &other) const
        {
            return !(*this == other);
        }

        [[nodiscard]] constexpr Vector3 operator-() const noexcept
        {
            return Vector3 { -m_comp[0], -m_comp[1], -m_comp[2] };
        }

        [[nodiscard]] constexpr Vector3 operator+(const Vector3 &other) const noexcept
        {
            return Vector3 { m_comp[0] + other.m_comp[0], m_comp[1] + other.m_comp[1], m_comp[2] + other.m_comp[2] };
        }

        [[nodiscard]] constexpr Vector3 operator-(const Vector3 &other) const noexcept
        {
            return Vector3 { m_comp[0] - other.m_comp[0], m_comp[1] - other.m_comp[1], m_comp[2] - other.m_comp[2] };
        }

        [[nodiscard]] constexpr Vector3 operator*(T scalar) const noexcept
        {
            return Vector3 { m_comp[0] * scalar, m_comp[1] * scalar, m_comp[2] * scalar };
        }

        [[nodiscard]] constexpr Vector3 operator/(T scalar) const noexcept
        {
            CORE_ASSERT( scalar != 0, "Division by zero" )
            return Vector3 { m_comp[0] / scalar, m_comp[1] / scalar, m_comp[2] / scalar };
        }

        [[nodiscard]] constexpr Vector3 &operator+=(const Vector3 &other) noexcept
        {
            for (size_t i { 0 }; i < N; ++i)
                m_comp[i] += other.m_comp[i];

            return *this;
        }

        [[nodiscard]] constexpr Vector3 &operator-=(const Vector3 &other) noexcept
        {
            for (size_t i { 0 }; i < N; ++i)
                m_comp[i] -= other.m_comp[i];

            return *this;
        }

        [[nodiscard]] constexpr Vector3 &operator*=(T scalar) noexcept
        {
            for (size_t i { 0 }; i < N; ++i)
                m_comp[i] *= scalar;

            return *this;
        }

        [[nodiscard]] constexpr Vector3 &operator/=(T scalar) noexcept
        {
            CORE_ASSERT( scalar != 0, "Division by zero" )
            for (size_t i { 0 }; i < N; ++i)
                m_comp[i] /= scalar;

            return *this;
        }

        [[nodiscard]] constexpr T GetX() const
        {
            return m_comp[0];
        }

        [[nodiscard]] constexpr T GetY() const
        {
            return m_comp[1];
        }

        [[nodiscard]] constexpr T GetZ() const
        {
            return m_comp[2];
        }

        constexpr void SetX(T x) noexcept
        {
            m_comp[0] = x;
        }

        constexpr void SetY(T y) noexcept
        {
            m_comp[1] = y;
        }

        constexpr void SetZ(T z) noexcept
        {
            m_comp[2] = z;
        }

        Vector3 Proj(const Vector3& vec, const Vector3& normal) const
        {
            return (Dot(vec) / normal.Dot(normal)) * normal;
        }

        [[nodiscard]] static constexpr size_t GetVectorSize() noexcept
        {
            return N;
        }

        std::string ToString() const
        {
            return std::to_string( m_comp[0] ) + ", " + std::to_string( m_comp[1] ) + ", " + std::to_string( m_comp[2] );
        }

        template<typename U> friend constexpr U          Dot(const Vector3<U> &v1, const Vector3<U> &v2) noexcept;
        template<typename U> friend constexpr Vector3<U> Cross(const Vector3<U> &v1, const Vector3<U> &v2) noexcept;

    private:
        static constexpr size_t N = 3;

        union
        {
            struct
            {
                T x;
                T y;
                T z;
            };

            std::array<T, N> m_comp;
        };
    };

    template <typename T>
    [[nodiscard]] constexpr Vector3<T> Cross(const Vector3<T> &v1, const Vector3<T> &v2) noexcept
    {
        return Vector3 { v1.m_comp[1] * v2.m_comp[2] - v1.m_comp[2] * v2.m_comp[1],
                         v1.m_comp[2] * v2.m_comp[0] - v1.m_comp[0] * v2.m_comp[2],
                         v1.m_comp[0] * v2.m_comp[1] - v1.m_comp[1] * v2.m_comp[0] };
    }

    template<typename T>
    [[nodiscard]] constexpr T Dot(const Vector3<T> &v1, const Vector3<T> &v2) noexcept
    {
        return v1.m_comp[0] * v2.m_comp[0] + v1.m_comp[1] * v2.m_comp[1] + v1.m_comp[2] * v2.m_comp[2];
    }
}