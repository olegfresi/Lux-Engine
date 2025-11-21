/*
* Project: TestProject
 * File: Matrix4.hpp
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
#include "Vector.hpp"
#include "MathUtils.hpp"

namespace lux::math
{
    enum class MatOrder : uint8_t
    {
        ROW_MAJOR,
        COLUMN_MAJOR
    };

    /**
     * @brief 4x4 matrix class for 3D transformations
     * 
     * This class represents a 4x4 matrix used for 3D transformations such as
     * translation, rotation, scaling, and perspective projection.
     * 
     * @tparam T The type of matrix elements (must be arithmetic)
     */
    template<typename T> requires std::is_arithmetic_v<T>
    class Matrix4
    {
    public:

        Matrix4() : m_order(MatOrder::COLUMN_MAJOR)
        {
            SetIdentity();
            det = 1.0f;
        }

        explicit Matrix4(T value, MatOrder order = MatOrder::COLUMN_MAJOR) : m_order{order}
        {
            if (m_order == MatOrder::COLUMN_MAJOR)
                for (auto& col : m_data)
                    col = Vector4<T>(value);
            else
                for (auto& row : m_data)
                    row = Vector4<T>(value);

            det = Determinant();
        }

        Matrix4(const Vector4<T>& c0,
                const Vector4<T>& c1,
                const Vector4<T>& c2,
                const Vector4<T>& c3,
                MatOrder order = MatOrder::COLUMN_MAJOR) : m_order{order}
       {
            if (m_order == MatOrder::COLUMN_MAJOR)
                m_data = {c0, c1, c2, c3};
            else
            {
                m_data =
                {
                    Vector4<T>(c0.x, c1.x, c2.x, c3.x),
                    Vector4<T>(c0.y, c1.y, c2.y, c3.y),
                    Vector4<T>(c0.z, c1.z, c2.z, c3.z),
                    Vector4<T>(c0.w, c1.w, c2.w, c3.w)
                };
            }

            det = Determinant();
        }


        constexpr Matrix4(T m1, T m2, T m3, T m4,
                     T m5, T m6, T m7, T m8,
                     T m9, T m10, T m11, T m12,
                     T m13, T m14, T m15, T m16,
                     MatOrder order = MatOrder::COLUMN_MAJOR) noexcept : m_order {order}
        {
            if (order == MatOrder::ROW_MAJOR)
                m_data =
                {
                    Vector4<T> {m1, m2, m3, m4},
                    Vector4<T> {m5, m6, m7, m8},
                    Vector4<T> {m9, m10, m11, m12},
                    Vector4<T> {m13, m14, m15, m16}
                };
            else
                m_data =
                {
                     Vector4<T> {m1, m5, m9, m13},
                     Vector4<T> {m2, m6, m10, m14},
                     Vector4<T> {m3, m7, m11, m15},
                     Vector4<T> {m4, m8, m12, m16}
                };

            det = Determinant();
        }

        explicit Matrix4(const NonOwnPtr<T> values, MatOrder order = MatOrder::COLUMN_MAJOR) : m_order(order)
        {
            if (m_order == MatOrder::COLUMN_MAJOR)
                for (size_t col = 0; col < 4; ++col)
                    for (size_t row = 0; row < 4; ++row)
                        m_data[col][row] = values[col * 4 + row];
            else
                for (size_t row = 0; row < 4; ++row)
                    for (size_t col = 0; col < 4; ++col)
                        m_data[row][col] = values[row * 4 + col];

            det = Determinant();
        }


        explicit Matrix4(const std::array<T,16>& values,
                         MatOrder order = MatOrder::COLUMN_MAJOR) : Matrix4{values.data(), order, Determinant()} {}

        constexpr Matrix4(const Matrix4 &m) noexcept: m_data {m.m_data}, m_order {m.m_order}, det {m.det} {}

        constexpr Matrix4(Matrix4 &&m) noexcept: m_data {std::move(m.m_data)}, m_order {std::move(m.m_order)}, det {std::move(m.det)}  {}

        static constexpr Matrix4 Identity() noexcept
        {
            return Matrix4
            {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1,
                MatOrder::COLUMN_MAJOR
            };
        }

        constexpr Matrix4 &operator=(const Matrix4& other) noexcept
        {
            if (this != &other)
                m_data = other.m_data;

            return *this;
        }

        constexpr Matrix4 &operator=(const Matrix4&& other) noexcept
        {
            if (this != &other)
                m_data = std::move(other.m_data);

            return *this;
        }

        [[nodiscard]] constexpr const Vector4<T>& operator[](size_t index) const
        {
            if (index >= N)
                throw std::out_of_range("Index out of range in matrix access");

            return m_data[index];
        }

        [[nodiscard]] constexpr Vector4<T>& operator[](size_t index)
        {
            if (index >= N)
                throw std::out_of_range("Index out of range in matrix access");

            return m_data[index];
        }

        [[nodiscard]] constexpr T At(size_t row, size_t col) const
        {
            if (row >= N || col >= N)
                throw std::out_of_range("Index out of range in matrix access");

            return m_data[row][col];
        }

        [[nodiscard]] constexpr T At(const Vector2<T>& pos) const
        {
            return m_data[pos.GetX()][pos.GetY()];
        }


        [[nodiscard]] constexpr Matrix4 Inverse() const
        {
            if (abs(det) <= EPSILON)
                throw std::runtime_error("Matrix is singular");

            float invDet = 1 / det;
            Matrix4 result;

            result.m_data[0][0] = Cofactor(0, 0) * invDet;
            result.m_data[0][1] = Cofactor(1, 0) * invDet;
            result.m_data[0][2] = Cofactor(2, 0) * invDet;
            result.m_data[0][3] = Cofactor(3, 0) * invDet;
            result.m_data[1][0] = Cofactor(0, 1) * invDet;
            result.m_data[1][1] = Cofactor(1, 1) * invDet;
            result.m_data[1][2] = Cofactor(2, 1) * invDet;
            result.m_data[1][3] = Cofactor(3, 1) * invDet;
            result.m_data[2][0] = Cofactor(0, 2) * invDet;
            result.m_data[2][1] = Cofactor(1, 2) * invDet;
            result.m_data[2][2] = Cofactor(2, 2) * invDet;
            result.m_data[2][3] = Cofactor(3, 2) * invDet;
            result.m_data[3][0] = Cofactor(0, 3) * invDet;
            result.m_data[3][1] = Cofactor(1, 3) * invDet;
            result.m_data[3][2] = Cofactor(2, 3) * invDet;
            result.m_data[3][3] = Cofactor(3, 3) * invDet;

            return result;
        }

        [[nodiscard]] constexpr Matrix4 Transpose() const noexcept
        {
            return Matrix4(m_data[0][0], m_data[0][1], m_data[0][2], m_data[0][3],
                                 m_data[1][0], m_data[1][1], m_data[1][2], m_data[1][3],
                                 m_data[2][0], m_data[2][1], m_data[2][2], m_data[2][3],
                                 m_data[3][0], m_data[3][1], m_data[3][2], m_data[3][3]);
        }

        [[nodiscard]] constexpr MatOrder GetMatOrder() const noexcept { return m_order; }

        [[nodiscard]] static constexpr size_t GetRows() noexcept { return N; }

        [[nodiscard]] static constexpr size_t GetCols() noexcept { return N; }

        [[nodiscard]] constexpr Vector4<T> GetDiagonal() const noexcept
        {
            return Vector4<T> {m_data[0][0], m_data[1][1], m_data[2][2], m_data[3][3]};
        }

        [[nodiscard]] constexpr T GetTrace() const noexcept
        {
            return m_data[0][0] + m_data[1][1] + m_data[2][2] + m_data[3][3];
        }

        [[nodiscard]] constexpr Vector4<T> GetRow(size_t index) const
        {
            if (index >= N)
                throw std::out_of_range("Index out of range in matrix access");

            return m_data[index];
        }

        [[nodiscard]] constexpr Vector4<T> GetCol(size_t index) const
        {
            if (index >= N)
                throw std::out_of_range("Index out of range in matrix access");

            return Vector4<T> {m_data[0][index], m_data[1][index], m_data[2][index], m_data[3][index]};
        }

        [[nodiscard]] constexpr Matrix4 operator+(const Matrix4 &other) const noexcept
        {
            return Matrix4(m_data[0] + other.m_data[0], m_data[1] + other.m_data[1], m_data[2] + other.m_data[2],
                           m_data[3] + other.m_data[3]);
        }

        [[nodiscard]] constexpr Matrix4 operator-(const Matrix4 &other) const noexcept
        {
            return Matrix4(m_data[0] - other.m_data[0], m_data[1] - other.m_data[1], m_data[2] - other.m_data[2],
                           m_data[3] - other.m_data[3]);
        }

        [[nodiscard]] constexpr Matrix4 operator*(const Matrix4 &other) const
        {
            return Matrix4(m_data[0].Dot(other.GetCol(0)),
                           m_data[0].Dot(other.GetCol(1)),
                           m_data[0].Dot(other.GetCol(2)),
                           m_data[0].Dot(other.GetCol(3)),
                           m_data[1].Dot(other.GetCol(0)),
                           m_data[1].Dot(other.GetCol(1)),
                           m_data[1].Dot(other.GetCol(2)),
                           m_data[1].Dot(other.GetCol(3)),
                           m_data[2].Dot(other.GetCol(0)),
                           m_data[2].Dot(other.GetCol(1)),
                           m_data[2].Dot(other.GetCol(2)),
                           m_data[2].Dot(other.GetCol(3)),
                           m_data[3].Dot(other.GetCol(0)),
                           m_data[3].Dot(other.GetCol(1)),
                           m_data[3].Dot(other.GetCol(2)),
                           m_data[3].Dot(other.GetCol(3)));
        }

        [[nodiscard]] constexpr T Determinant() const noexcept
                {
                    return m_data[0][0] * (
                            m_data[1][1] * (m_data[2][2] * m_data[3][3] - m_data[2][3] * m_data[3][2]) -
                            m_data[1][2] * (m_data[2][1] * m_data[3][3] - m_data[2][3] * m_data[3][1]) +
                            m_data[1][3] * (m_data[2][1] * m_data[3][2] - m_data[2][2] * m_data[3][1])
                    )
                           - m_data[0][1] * (
                            m_data[1][0] * (m_data[2][2] * m_data[3][3] - m_data[2][3] * m_data[3][2]) -
                            m_data[1][2] * (m_data[2][0] * m_data[3][3] - m_data[2][3] * m_data[3][0]) +
                            m_data[1][3] * (m_data[2][0] * m_data[3][2] - m_data[2][2] * m_data[3][0])
                    )
                           + m_data[0][2] * (
                            m_data[1][0] * (m_data[2][1] * m_data[3][3] - m_data[2][3] * m_data[3][1]) -
                            m_data[1][1] * (m_data[2][0] * m_data[3][3] - m_data[2][3] * m_data[3][0]) +
                            m_data[1][3] * (m_data[2][0] * m_data[3][1] - m_data[2][1] * m_data[3][0])
                    )
                           - m_data[0][3] * (
                            m_data[1][0] * (m_data[2][1] * m_data[3][2] - m_data[2][2] * m_data[3][1]) -
                            m_data[1][1] * (m_data[2][0] * m_data[3][2] - m_data[2][2] * m_data[3][0]) +
                            m_data[1][2] * (m_data[2][0] * m_data[3][1] - m_data[2][1] * m_data[3][0])
                    );
                }

        [[nodiscard]] constexpr Matrix4 operator*(T scalar) const noexcept
        {
            return Matrix4(m_data[0] * scalar, m_data[1] * scalar, m_data[2] * scalar, m_data[3] * scalar);
        }

        [[nodiscard]] constexpr Vector4<T> operator*(const Vector4<T>& v) const noexcept
        {
            return Vector4<T> {m_data[0].Dot(v), m_data[1].Dot(v), m_data[2].Dot(v), m_data[3].Dot(v)};
        }

        [[nodiscard]] constexpr const Matrix4& operator/(T scalar) const noexcept
        {
            CORE_ASSERT(scalar != 0, "Division by zero")
            return (1 / scalar) * this;
        }

        [[nodiscard]] constexpr bool operator==(const Matrix4 &other) const noexcept
        {
            return m_data[0] == other.m_data[0] && m_data[1] == other.m_data[1] &&
                   m_data[2] == other.m_data[2] && m_data[3] == other.m_data[3];
        }

        [[nodiscard]] static constexpr Matrix4 Translate(const Vector3<T> &v) noexcept
        {
            return Matrix4
            {
                1, 0, 0, v[0],
                0, 1, 0, v[1],
                0, 0, 1, v[2],
                0, 0, 0, 1,
                MatOrder::ROW_MAJOR
            };
        }

        [[nodiscard]] static constexpr Matrix4 Scale(T scalar) noexcept
        {
            return Matrix4 {scalar, 0, 0, 0, 0, scalar, 0, 0, 0, 0, scalar, 0, 0, 0, 0, 1, MatOrder::ROW_MAJOR};
        }

        [[nodiscard]] static constexpr Matrix4 Scale(const Vector3<T> &v) noexcept
        {
            return Matrix4
            {
                v[0], 0,    0,    0,
                0,    v[1], 0,    0,
                0,    0,    v[2], 0,
                0,    0,    0,    1,
                MatOrder::ROW_MAJOR
            };
        }

        [[nodiscard]] static constexpr Matrix4 Rotate(const T angle, const Vector3<T> &axis = {0, 0, 1}) noexcept
        {
            T cosA = std::cos(angle);
            T sinA = std::sin(angle);
            T oneMinusCos = 1 - cosA;

            T len = axis.Length();
            T ux = axis.GetX() / len;
            T uy = axis.GetY() / len;
            T uz = axis.GetZ() / len;

            return Matrix4
            {
                cosA + ux * ux * oneMinusCos, ux * uy * oneMinusCos - uz * sinA,
                ux * uz * oneMinusCos + uy * sinA, 0,

                uy * ux * oneMinusCos + uz * sinA, cosA + uy * uy * oneMinusCos,
                uy * uz * oneMinusCos - ux * sinA, 0,

                uz * ux * oneMinusCos - uy * sinA, uz * uy * oneMinusCos + ux * sinA,
                cosA + uz * uz * oneMinusCos, 0,

                0, 0, 0, 1,
                MatOrder::ROW_MAJOR
            };
        }

        [[nodiscard]] static constexpr Matrix4 Scale(const Matrix4& m, T scalar) noexcept
        {
            return m * scalar;
        }

        [[nodiscard]] static constexpr Matrix4 Scale(const Matrix4& m, const Vector3<T>& v) noexcept
        {
            return m * Scale(v) ;
        }

        [[nodiscard]] static constexpr Matrix4 Translate(const Matrix4& m, const Vector3<T> &v) noexcept
        {
            return m * Translate(v);
        }

        [[nodiscard]] static constexpr Matrix4 Rotate(const Matrix4& m, const T angle, const Vector3<T>& axis) noexcept
        {
            return m * Rotate(angle, axis);
        }

        void Print(bool transpose = false) const noexcept
        {
            for(int i = 0; i < 4; ++i)
            {
                for(int j = 0; j < 4; ++j)
                    transpose ? std::cout << m_data[j][i] << " " : std::cout << m_data[i][j] << " ";
                std::cout << std::endl;
            }
        }

        static constexpr Matrix4 Perspective(T fov, T aspect, T near, T far)
        {
            T tanHalfFov = std::tan(DegToRad(fov) / 2);
            T range = far - near;
            if (abs(far - near) < EPSILON)
                throw std::runtime_error("Far plane equal to near plane");

            return Matrix4
                    {
                        1 / (aspect * tanHalfFov), 0, 0, 0,
                        0, 1 / tanHalfFov, 0, 0,
                        0, 0, -(near + far) / range, -2 * far * near / range,
                        0, 0, -1, 0,
                        MatOrder::ROW_MAJOR
                    };
        }

        static constexpr Matrix4 Orthographic(T left, T right, T bottom, T top, T near, T far)
        {
            T rl = right - left;
            T tb = top - bottom;
            T fn = far - near;

            if (abs(rl) < EPSILON || abs(tb) < EPSILON || abs(fn) < EPSILON)
                throw std::runtime_error("Invalid frustum dimensions");

            return Matrix4
           {
               2 / rl, 0, 0, -(right + left) / rl,
               0, 2 / tb, 0, -(top + bottom) / tb,
               0, 0, -2 / fn, -(far + near) / fn,
               0, 0, 0, 1,
               MatOrder::ROW_MAJOR
          };
        }

        static constexpr Matrix4 LookAt(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& upDir)
        {
            Vector3 forward = (target - eye).Normalize();
            Vector3 right   = Cross(forward, upDir).Normalize();
            Vector3 up      = Cross(right, forward);

            return Matrix4
           {
               right.GetX(), right.GetY(), right.GetZ(), -Dot(right, eye),
               up.GetX(), up.GetY(), up.GetZ(), -Dot(up, eye),
               -forward.GetX(), -forward.GetY(), -forward.GetZ(), Dot(forward, eye),
               0.0f, 0.0f, 0.0f, 1.0f,
               MatOrder::ROW_MAJOR
           };
        }

        const float* Data() const { return reinterpret_cast<const float*>(m_data.data()); }

        [[nodiscard]] constexpr bool IsValid() const noexcept
        {
            for (size_t i = 0; i < 4; ++i)
                for (size_t j = 0; j < 4; ++j)
                    if (std::isnan(m_data[i][j]) || std::isinf(m_data[i][j]))
                        return false;

            if (m_data[3][0] != 0.0f || m_data[3][1] != 0.0f || m_data[3][2] != 0.0f || m_data[3][3] != 1.0f)
                return false;

            Matrix4 transpose = Transpose();
            Matrix4 product = *this * transpose;
            
            for (size_t i = 0; i < 4; ++i)
                for (size_t j = 0; j < 4; ++j)
                    if (i == j)
                    {
                        if (std::abs( product.m_data[i][j] - 1.0f ) > 1e-6f)
                            return false;
                        if (std::abs( product.m_data[i][j] ) > 1e-6f)
                            return false;
                    }

            return true;
        }

    private:
        static constexpr size_t N = 4;
        std::array<Vector4<T>, N> m_data;
        MatOrder m_order;
        float det;

        void SetIdentity() noexcept
        {
            for (size_t i = 0; i < N; ++i)
                for (size_t j = 0; j < N; ++j)
                    m_data[i][j] = (i == j) ? 1.0f : 0.0f;
        }

        [[nodiscard]] constexpr float Cofactor(int row, int col) const noexcept
        {
            float minor[3][3];
            int m = 0, n = 0;

            for (int i = 0; i < 4; ++i) {
                if (i == row) continue;
                n = 0;
                for (int j = 0; j < 4; ++j) {
                    if (j == col) continue;
                    minor[m][n] = m_data[i][j];
                    n++;
                }
                m++;
            }

            float detMinor = minor[0][0] * (minor[1][1] * minor[2][2] - minor[1][2] * minor[2][1]) -
                             minor[0][1] * (minor[1][0] * minor[2][2] - minor[1][2] * minor[2][0]) +
                             minor[0][2] * (minor[1][0] * minor[2][1] - minor[1][1] * minor[2][0]);

            return ((row + col) % 2 == 0 ? 1.0f : -1.0f) * detMinor;
        }
    };
}
