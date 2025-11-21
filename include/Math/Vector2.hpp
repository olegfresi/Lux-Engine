/*
 * Project: TestProject
 * File: Vector2.hpp
 * Author: olegfresi
 * Created: 07/02/25 20:43
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
#include "../Application/Assertion.hpp"

namespace lux::math
{
    /**
     * @brief A class representing a 2D vector with various vector operations
     * 
     * The Vector2 class provides functionality for working with 2D vectors, including
     * basic vector operations, normalization, and dot product calculations.
     * 
     * @tparam T The type of the vector components (must be arithmetic)
     */
    template<typename T> requires std::is_arithmetic_v<T>
    class Vector2 {
    public:
        /**
         * @brief Default constructor, initializes all components to zero
         */
        constexpr Vector2() : m_comp {std::array<T, N> {}} {}

        /**
         * @brief Construct a vector with specified x and y components
         * @param x X component
         * @param y Y component
         */
        explicit constexpr Vector2(T x, T y) noexcept: m_comp {x, y} {}

        /**
         * @brief Construct a vector from an initializer list
         * @param comps Initializer list with exactly 2 components
         * @throw std::invalid_argument if the number of components is not 2
         */
        explicit constexpr Vector2(const std::initializer_list<T> &comps) {
            if (comps.size() != N)
                throw std::invalid_argument("Invalid number of components");

            std::copy(comps.begin(), comps.end(), m_comp.begin());
        }

        /**
         * @brief Move constructor
         * @param comps Initializer list to move from
         * @throw std::invalid_argument if the number of components is not 2
         */
        constexpr Vector2(std::initializer_list<T> &&comps) {
            if (comps.size() != N)
                throw std::invalid_argument("Invalid number of components");

            std::move(comps.begin(), comps.end(), m_comp.begin());
        }

        /**
         * @brief Copy constructor
         * @param other Vector to copy from
         */
        constexpr Vector2(const Vector2 &other) noexcept: m_comp {other.m_comp} {}

        /**
         * @brief Move constructor
         * @param other Vector to move from
         */
        constexpr Vector2(Vector2 &&other) noexcept: m_comp {std::move(other.m_comp)} {}

        /**
         * @brief Copy assignment operator
         * @param other Vector to copy from
         * @return Reference to this vector
         */
        constexpr Vector2 &operator=(const Vector2 &other) noexcept {
            if (this != &other)
                m_comp = other.m_comp;

            return *this;
        }

        /**
         * @brief Move assignment operator
         * @param other Vector to move from
         * @return Reference to this vector
         */
        constexpr Vector2 &operator=(Vector2 &&other) noexcept {
            if (this != &other)
                m_comp = std::move(other.m_comp);

            return *this;
        }

        /**
         * @brief Array-style access to vector components
         * @param index Index of the component (0=x, 1=y)
         * @return Reference to the component
         * @throw std::out_of_range if index is out of bounds
         */
        constexpr T &operator[](size_t index) {
            if (index >= 2)
                throw std::out_of_range("Index out of range in vector2 access");
            return m_comp[index];
        }

        /**
         * @brief Const array-style access to vector components
         * @param index Index of the component (0=x, 1=y)
         * @return Const reference to the component
         * @throw std::out_of_range if index is out of bounds
         */
        constexpr const T &operator[](size_t index) const {
            if (index >= 2)
                throw std::out_of_range("Index out of range in vector2 access");
            return m_comp[index];
        }

        /**
         * @brief Normalize the vector in place
         * @param vec Vector to normalize
         */
        static constexpr void Normalize(Vector2 &vec) {
            T length = vec.Length();
            for (size_t i {0}; i < N; ++i)
                vec[i] /= length;
        }

        /**
         * @brief Calculate the length of the vector
         * @return The length of the vector
         */
        [[nodiscard]] constexpr T Length() const noexcept {
            return std::sqrt(m_comp[0] * m_comp[0] + m_comp[1] * m_comp[1]);
        }

        /**
         * @brief Calculate the length of a vector
         * @param vec Vector to calculate length of
         * @return The length of the vector
         */
        [[nodiscard]] constexpr static T Length(const Vector2 &vec) noexcept {
            return vec.Length();
        }

        /**
         * @brief Calculate the dot product with another vector
         * @param other Vector to dot with
         * @return The dot product
         */
        [[nodiscard]] constexpr T Dot(const Vector2 &other) const noexcept {
            return m_comp[0] * other.m_comp[0] + m_comp[1] * other.m_comp[1];
        }

        /**
         * @brief Equality comparison operator
         * @param other Vector to compare with
         * @return true if vectors are equal, false otherwise
         */
        [[nodiscard]] constexpr bool operator==(const Vector2 &other) const noexcept {
            return m_comp[0] == other.m_comp[0] && m_comp[1] == other.m_comp[1];
        }

        /**
         * @brief Inequality comparison operator
         * @param other Vector to compare with
         * @return true if vectors are not equal, false otherwise
         */
        [[nodiscard]] constexpr bool operator!=(const Vector2 &other) const noexcept {
            return *this != other;
        }

        /**
         * @brief Unary negation operator
         * @return Negated vector
         */
        [[nodiscard]] constexpr Vector2 operator-() const noexcept {
            return Vector2 {-m_comp[0], -m_comp[1]};
        }

        /**
         * @brief Vector addition operator
         * @param other Vector to add
         * @return Sum of the vectors
         */
        [[nodiscard]] constexpr Vector2 operator+(const Vector2 &other) const noexcept {
            return Vector2 {m_comp[0] + other.m_comp[0], m_comp[1] + other.m_comp[1]};
        }

        /**
         * @brief Vector subtraction operator
         * @param other Vector to subtract
         * @return Difference of the vectors
         */
        [[nodiscard]] constexpr Vector2 operator-(const Vector2 &other) const noexcept {
            return Vector2 {m_comp[0] - other.m_comp[0], m_comp[1] - other.m_comp[1]};
        }

        /**
         * @brief Scalar multiplication operator
         * @param scalar Scalar to multiply by
         * @return Scaled vector
         */
        [[nodiscard]] constexpr Vector2 operator*(T scalar) const noexcept {
            return Vector2 {m_comp[0] * scalar, m_comp[1] * scalar};
        }

        /**
         * @brief Scalar division operator
         * @param scalar Scalar to divide by
         * @return Scaled vector
         * @throw std::runtime_error if scalar is zero
         */
        [[nodiscard]] constexpr Vector2 operator/(T scalar) const noexcept {
            CORE_ASSERT(scalar != 0, "Division by zero")
            return Vector2 {m_comp[0] / scalar, m_comp[1] / scalar};
        }

        /**
         * @brief Vector addition assignment operator
         * @param other Vector to add
         * @return Reference to this vector
         */
        [[nodiscard]] constexpr Vector2 &operator+=(const Vector2 &other) noexcept {
            for (size_t i {0}; i < N; ++i)
                m_comp[i] += other.m_comp[i];

            return *this;
        }

        /**
         * @brief Vector subtraction assignment operator
         * @param other Vector to subtract
         * @return Reference to this vector
         */
        [[nodiscard]] constexpr Vector2 &operator-=(const Vector2 &other) noexcept {
            for (size_t i {0}; i < N; ++i)
                m_comp[i] -= other.m_comp[i];

            return *this;
        }

        /**
         * @brief Scalar multiplication assignment operator
         * @param scalar Scalar to multiply by
         * @return Reference to this vector
         */
        [[nodiscard]] constexpr Vector2 &operator*=(T scalar) noexcept {
            for (size_t i {0}; i < N; ++i)
                m_comp[i] *= scalar;

            return *this;
        }

        /**
         * @brief Scalar division assignment operator
         * @param scalar Scalar to divide by
         * @return Reference to this vector
         * @throw std::runtime_error if scalar is zero
         */
        [[nodiscard]] constexpr Vector2 &operator/=(T scalar) noexcept {
            CORE_ASSERT(scalar != 0, "Division by zero")
            for (size_t i {0}; i < N; ++i)
                m_comp[i] /= scalar;

            return *this;
        }

        /**
         * @brief Get the x component
         * @return The x component
         */
        [[nodiscard]] constexpr T GetX() const {
            return m_comp[0];
        }

        /**
         * @brief Get the y component
         * @return The y component
         */
        [[nodiscard]] constexpr T GetY() const {
            return m_comp[1];
        }

        /**
         * @brief Get the u component (alias for x)
         * @return The u component
         */
        [[nodiscard]] constexpr T GetU() const {
            return u;
        }

        /**
         * @brief Get the v component (alias for y)
         * @return The v component
         */
        [[nodiscard]] constexpr T GetV() const {
            return v;
        }

        /**
         * @brief Get the size of the vector
         * @return The number of components (2)
         */
        [[nodiscard]] static constexpr size_t GetVectorSize() noexcept
        {
            return N;
        }

    private:
        static constexpr size_t N = 2;
        union {
            struct {
                T x;
                T y;
            };

            struct {
                T u;
                T v;
            };

            std::array<T, N> m_comp;
        };
    };
}