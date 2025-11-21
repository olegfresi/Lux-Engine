/*
 * Project: TestProject
 * File: Color.hpp
 * Author: olegfresi
 * Created: 15/02/25 21:12
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
/**
 * @file Color.hpp
 * @brief Defines the Color class for handling RGBA colors
 * @author olegfresi
 * @date 15/02/25
 */
#pragma once
#include "Vector.hpp"

namespace lux
{
    using namespace math;
    /**
     * @brief A class representing an RGBA color with various color operations
     *
     * The Color class provides functionality for working with RGBA colors, including
     * basic color operations, predefined colors, and color transformations.
     */
    class Color
    {
    public:
        /**
         * @brief Color default constructor
         */
        Color() : m_r(0.f), m_g(0.f), m_b(0.f), m_a(1.f) {}

        /**
         * @brief Construct a new Color object
         * @param r Red component (0.0 to 1.0)
         * @param g Green component (0.0 to 1.0)
         * @param b Blue component (0.0 to 1.0)
         * @param a Alpha component (0.0 to 1.0)
         */
        explicit Color(const float r,
                       const float g,
                       const float b,
                       const float a=1.0) : m_r(r), m_g(g), m_b(b), m_a(a) {}

        /**
         * @brief Static factory methods for common colors
         * @return A Color object with the specified color
         */
        static Color Black() { return Color( 0, 0, 0); }
        static Color White() { return Color( 1, 1, 1); }
        static Color Red    (const float lum = 1.f) { return Color( lum * 1.f, 0.f, 0.f); }
        static Color Green  (const float lum = 1.f) { return Color( 0.f, lum * 1.f, 0.f); }
        static Color Blue   (const float lum = 1.f) { return Color( 0.f, 0.f, lum * 1.f); }
        static Color Yellow (const float lum = 1.f) { return Color( lum * 1.f, lum * 1.f, 0.f); }
        static Color Magenta(const float lum = 1.f) { return Color( lum * 1.f, 0.f, lum * 1.f); }
        static Color Cyan   (const float lum = 1.f) { return Color( 0.f, lum * 1.f, lum * 1.f); }
        static Color Gray   (const float lum = 1.f) { return Color( lum * .5f, lum * .5f, lum * .5f); }


        /**
         * @brief Comparison operators
         * @param c Color to compare with
         * @return true if the condition is met, false otherwise
         */
        bool    operator==(const Color & c) const;
        bool    operator!=(const Color & c) const;
        bool    operator< (const Color & c) const;
        bool    operator<=(const Color & c) const;

        /**
         * @brief Array-style access to color components
         * @param i Index of the component (0=R, 1=G, 2=B, 3=A)
         * @return Reference to the color component
         */
        float & operator[](const uint    i);
        const float & operator[](const uint    i) const;

        /**
         * @brief Arithmetic operators for color operations
         * @return Result of the operation
         */
        Color & operator*=(float f);
        Color & operator+=(const Color& c);
        Color & operator-=(const Color& c);
        Color operator+(const Color& c) const;
        Color operator-(const Color& c) const;
        Color operator*(const float f) const;
        Color operator*(const Color& c) const;

        /**
         * @brief Clamp color components to valid range [0,1]
         */
        void Clamp()
        {
            m_r = std::clamp(m_r, 0.0f, 1.0f);
            m_g = std::clamp(m_g, 0.0f, 1.0f);
            m_b = std::clamp(m_b, 0.0f, 1.0f);
            m_a = std::clamp(m_a, 0.0f, 1.0f);
        }

        /**
         * @brief Create a new color with clamped components
         * @return New Color object with clamped components
         */
        Color Clamp(int) const
        {
            return Color(std::clamp(m_r, 0.0f, 1.0f),
                         std::clamp(m_g, 0.0f, 1.0f),
                         std::clamp(m_b, 0.0f, 1.0f),
                         std::clamp(m_a, 0.0f, 1.0f));
        }

        /**
         * @brief Linear interpolation between two colors
         * @param a First color
         * @param b Second color
         * @param t Interpolation factor (0.0 to 1.0)
         * @return Interpolated color
         */
        static Color Lerp(const Color & a, const Color & b, const float t) { return a + (b - a) * t; }

        /**
         * @brief Get color components
         * @return The respective color component
         */
        float GetR() const { return m_r; }
        float GetG() const { return m_g; }
        float GetB() const { return m_b; }
        float GetA() const { return m_a; }

        /**
         * @brief Set color components
         */
        void SetR(float r) { m_r = r; }
        void SetG(float g) { m_g = g; }
        void SetB(float b) { m_b = b; }
        void SetA(float a) { m_a = a; }

        Vector3f AsVector3() const
        {
            return Vector3f (m_r, m_g, m_b);
        }

        Vector4f AsVector4() const
        {
            return Vector4f(m_r, m_g, m_b, m_a);
        }

    private:
        union
        {
            struct
            {
                float m_r, m_g, m_b, m_a;
            };
            float rgba[4];
        };
    };

    /**
     * @brief Pack a color into a single float value
     * @param color Color to pack
     * @return Packed color as float
     */
    inline float PackColorToFloat(const Color& color)
    {
        uint32_t ri = static_cast<uint32_t>(color.GetR() * 255.0f) & 0xFF;
        uint32_t gi = static_cast<uint32_t>(color.GetG() * 255.0f) & 0xFF;
        uint32_t bi = static_cast<uint32_t>(color.GetB() * 255.0f) & 0xFF;
        uint32_t ai = static_cast<uint32_t>(color.GetA() * 255.0f) & 0xFF;

        uint32_t packed;
        if constexpr (std::endian::native == std::endian::little)
            packed = (ai << 24) | (bi << 16) | (gi << 8) | ri;
        else
            packed = (ri << 24) | (gi << 16) | (bi << 8) | ai;

        return std::bit_cast<float>(packed);
    }


    /**
     * @brief Unpack a color from a single float value
     * @param packed Packed color as float
     * @return Unpacked Color object
     */
    inline Color UnpackColorFromFloat(float packed)
    {
        uint32_t val = std::bit_cast<uint32_t>(packed);

        uint32_t ri, gi, bi, ai;
        if constexpr (std::endian::native == std::endian::little)
        {
            ai = ( val >> 24 ) & 0xFF;
            bi = ( val >> 16 ) & 0xFF;
            gi = ( val >> 8 ) & 0xFF;
            ri = val & 0xFF;
        }
        else
        {
            ri = ( val >> 24 ) & 0xFF;
            gi = ( val >> 16 ) & 0xFF;
            bi = ( val >> 8 ) & 0xFF;
            ai = val & 0xFF;
        }

        return Color(ri / 255.0f, gi / 255.0f, bi / 255.0f, ai / 255.0f);
    }


    /**
     * @brief Implementation of color addition
     */
    inline Color Color::operator+(const Color& c) const
    {
        return Color(m_r + c.m_r, m_g + c.m_g, m_b + c.m_b, m_a + c.m_a);
    }

    /**
     * @brief Implementation of color subtraction
     */
    inline Color Color::operator-(const Color& c) const
    {
        return Color(m_r - c.m_r, m_g - c.m_g, m_b - c.m_b, m_a - c.m_a);
    }

    /**
     * @brief Implementation of color multiplication by scalar
     */
    inline Color Color::operator*(const float f) const
    {
        return Color(m_r * f, m_g * f, m_b * f, m_a * f);
    }

    /**
     * @brief Implementation of color multiplication
     */
    inline Color Color::operator*(const Color & c) const
    {
        return Color(m_r * c.m_r, m_g * c.m_g, m_b * c.m_b, m_a * c.m_a);
    }

    /**
     * @brief Implementation of color addition assignment
     */
    inline Color& Color::operator+=(const Color & c)
    {
        m_r += c.m_r;
        m_g += c.m_g;
        m_b += c.m_b;
        m_a += c.m_a;
        return *this;
    }

    /**
     * @brief Implementation of color subtraction assignment
     */
    inline Color& Color::operator-=(const Color & c)
    {
        m_r -= c.m_r;
        m_g -= c.m_g;
        m_b -= c.m_b;
        m_a -= c.m_a;
        return *this;
    }

    /**
     * @brief Implementation of color multiplication assignment by scalar
     */
    inline bool Color::operator ==(const Color &c) const
    {
        return m_r == c.m_r && m_g == c.m_g && m_b == c.m_b && m_a == c.m_a;
    }

    /**
     * @brief Implementation of color inequality operator
     */
    inline bool Color::operator !=(const Color &c) const
    {
        return !(*this == c);
    }
}