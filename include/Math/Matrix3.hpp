/*
 * Project: TestProject
 * File: Matrix3.hpp
 * Author: olegfresi
 * Created: 07/02/25 18:53
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
#include "Matrix4.hpp"
#include "MathUtils.hpp"


namespace lux::math
{
    template<typename T> requires std::is_arithmetic_v<T>
    class Matrix3
    {
    public:

        Matrix3(MatOrder order = MatOrder::COLUMN_MAJOR) : m_order{order} { SetIdentity(); }

        explicit Matrix3(T value, MatOrder order = MatOrder::COLUMN_MAJOR) : m_order{order}
        {
            for (auto &col : m_data)
                col = Vector3 <T>(value);
        }

        Matrix3(const Vector3 <T>& c0,
                const Vector3 <T>& c1,
                const Vector3 <T>& c2,
                MatOrder order = MatOrder::COLUMN_MAJOR) : m_order{order}
        {
            if (m_order == MatOrder::COLUMN_MAJOR)
                m_data = { c0, c1, c2 };
            else
                m_data =
                {
                    Vector3 <T>(c0.x, c1.x, c2.x),
                    Vector3 <T>(c0.y, c1.y, c2.y),
                    Vector3 <T>(c0.z, c1.z, c2.z)
                };
        }

        Matrix3(T m00, T m01, T m02,
                T m10, T m11, T m12,
                T m20, T m21, T m22,
                MatOrder order = MatOrder::COLUMN_MAJOR) : m_order{order}
        {
            if (m_order == MatOrder::ROW_MAJOR)
            {
                m_data[0] = { m00, m01, m02 };
                m_data[1] = { m10, m11, m12 };
                m_data[2] = { m20, m21, m22 };
            }
            else
            {
                m_data[0] = { m00, m10, m20 };
                m_data[1] = { m01, m11, m21 };
                m_data[2] = { m02, m12, m22 };
            }
        }

        explicit Matrix3(const std::array <T, 9> &values,
                         MatOrder order = MatOrder::COLUMN_MAJOR) : Matrix3{values.data(), order} {}

        explicit Matrix3(const NonOwnPtr <T> values, MatOrder order = MatOrder::COLUMN_MAJOR) : m_order{order}
        {
            if (m_order == MatOrder::COLUMN_MAJOR)
                for (size_t col = 0; col < 3; ++col)
                    for (size_t row = 0; row < 3; ++row)
                        m_data[col][row] = values[col * 3 + row];
            else
                for (size_t row = 0; row < 3; ++row)
                    for (size_t col = 0; col < 3; ++col)
                        m_data[row][col] = values[row * 3 + col];
        }


        constexpr Matrix3(const Matrix3 &m) noexcept : m_order{m.m_order}, m_data{m.m_data} {}

        constexpr Matrix3(Matrix3 &&m) noexcept : m_order{std::move(m.m_order)}, m_data{std::move(m.m_data)} {}

        static constexpr Matrix3 Identity() noexcept
        {
            return Matrix3
            {
                1, 0, 0,
                0, 1, 0,
                0, 0, 1,
                MatOrder::COLUMN_MAJOR
            };
        }


        [[nodiscard]] constexpr Matrix3 &operator=(const Matrix3 &other) noexcept
        {
            if (this != &other)
                m_data = other.m_data;

            return *this;
        }

        [[nodiscard]] constexpr Matrix3 &operator=(const Matrix3 &&other) noexcept
        {
            if (this != &other)
                m_data = std::move(other.m_data);

            return *this;
        }

        [[nodiscard]] constexpr const Vector3 <T> &operator[](size_t index) const
        {
            if (index >= N)
                throw std::out_of_range("Index out of range in matrix access");

            return m_data[index];
        }

        [[nodiscard]] constexpr T Determinant() const noexcept
        {
            return m_data[0][0] * (m_data[1][1] * m_data[2][2] - m_data[1][2] * m_data[2][1]) -
                   m_data[0][1] * (m_data[1][0] * m_data[2][2] - m_data[1][2] * m_data[2][0]) +
                   m_data[0][2] * (m_data[1][0] * m_data[2][1] - m_data[1][1] * m_data[2][0]);
        }

        [[nodiscard]] constexpr Matrix3 Inverse() const
        {
            if (*this == Matrix3::Identity())
                return *this;

            if (det <= EPSILON)
                new MathException("Matrix is Singular");

            float invDet = 1 / det;
            return Matrix3((m_data[1][1] * m_data[2][2] - m_data[1][2] * m_data[2][1]) * invDet,
                           (m_data[0][2] * m_data[2][1] - m_data[0][1] * m_data[2][2]) * invDet,
                           (m_data[0][1] * m_data[1][2] - m_data[0][2] * m_data[1][1]) * invDet,
                           (m_data[1][2] * m_data[2][0] - m_data[1][0] * m_data[2][2]) * invDet,
                           (m_data[0][0] * m_data[2][2] - m_data[0][2] * m_data[2][0]) * invDet,
                           (m_data[1][0] * m_data[0][2] - m_data[0][0] * m_data[1][2]) * invDet,
                           (m_data[1][0] * m_data[2][1] - m_data[2][0] * m_data[1][1]) * invDet,
                           (m_data[2][0] * m_data[0][1] - m_data[0][0] * m_data[2][1]) * invDet,
                           (m_data[0][0] * m_data[1][1] - m_data[1][0] * m_data[0][1]) * invDet);
        }

        [[nodiscard]] constexpr Matrix3 Transpose() const noexcept
        {
            return Matrix3(m_data[0][0], m_data[1][0], m_data[2][0],
                           m_data[0][1], m_data[1][1], m_data[2][1],
                           m_data[0][2], m_data[1][2], m_data[2][2]);
        }

        [[nodiscard]] constexpr MatOrder GetMatOrder() const noexcept { return m_order; }

        [[nodiscard]] static constexpr size_t GetRows() noexcept { return N; }

        [[nodiscard]] static constexpr size_t GetCols() noexcept { return N; }

        [[nodiscard]] constexpr Vector3<T> GetDiagonal() const noexcept
        {
            return Vector3<T>{ m_data[0][0], m_data[1][1], m_data[2][2] };
        }

        [[nodiscard]] constexpr Vector3<T>& GetRow(unsigned int index) const
        {
            if (index >= N)
                throw std::out_of_range("Index out of range in matrix access");

            return m_data[index];
        }

        [[nodiscard]] constexpr Vector3<T> GetCol(unsigned int index) const
        {
            if (index >= N)
                throw std::out_of_range("Index out of range in matrix access");

            return Vector3<T>{ m_data[0][index], m_data[1][index], m_data[2][index] };
        }

        [[nodiscard]] constexpr T GetTrace() const noexcept { return m_data[0][0] + m_data[1][1] + m_data[2][2]; }

        [[nodiscard]] constexpr Matrix3 operator+(const Matrix3 &other) const noexcept
        {
            return Matrix(m_data[0] + other.m_data[0], m_data[1] + other.m_data[1], m_data[2] + other.m_data[2]);
        }

        [[nodiscard]] constexpr Matrix3 operator-(const Matrix3 &other) const noexcept
        {
            return Matrix(m_data[0] - other.m_data[0], m_data[1] - other.m_data[1], m_data[2] - other.m_data[2]);
        }

        [[nodiscard]] constexpr Matrix3 operator*(const Matrix3 &other) const noexcept
        {
            return Matrix3(m_data[0].Dot(other.GetCol(0)), m_data[0].Dot(other.GetCol(1)),
                           m_data[0].Dot(other.GetCol(2)),
                           m_data[1].Dot(other.GetCol(0)), m_data[1].Dot(other.GetCol(1)),
                           m_data[1].Dot(other.GetCol(2)),
                           m_data[2].Dot(other.GetCol(0)), m_data[2].Dot(other.GetCol(1)),
                           m_data[2].Dot(other.GetCol(2)));
        }

        [[nodiscard]] constexpr Matrix3 operator*(T scalar) const noexcept
        {
            return Matrix(m_data[0] * scalar, m_data[1] * scalar, m_data[2] * scalar);
        }

        [[nodiscard]] constexpr Vector3 <T> operator*(Vector3 <T> vec) const noexcept
        {
            return Vector3 <T>{ m_data[0].Dot(vec), m_data[1].Dot(vec), m_data[2].Dot(vec) };
        }

        [[nodiscard]] constexpr const Matrix3 &operator/(T scalar) const noexcept
        {
            CORE_ASSERT(scalar != 0, "Division by zero");
            return (1 / scalar) * this;
        }

        [[nodiscard]] constexpr bool operator==(const Matrix3 &other) const noexcept
        {
            return m_data[0] == other.m_data[0] && m_data[1] == other.m_data[1] && m_data[2] == other.m_data[2];
        }

        [[nodiscard]] constexpr Matrix3 Orthogonalize() const noexcept
        {
            Vector3 <T> col0(m_data[0][0], m_data[1][0], m_data[2][0]);
            Vector3 <T> col1(m_data[0][1], m_data[1][1], m_data[2][1]);
            Vector3 <T> col2(m_data[0][2], m_data[1][2], m_data[2][2]);

            col0 = col0.Normalized();
            col1 -= col0 * col0.Dot(col1);
            col1 = col1.Normalized();
            col2 -= col0 * col0.Dot(col2) + col1 * col1.Dot(col2);
            col2 = col2.Normalized();

            return Matrix3
            {
                col0.x, col1.x, col2.x,
                col0.y, col1.y, col2.y,
                col0.z, col1.z, col2.z
            };
        }

        [[nodiscard]] constexpr Matrix3 Diagonalize() const noexcept
        {
            T eigenvalues[3];
            Vector3 <T> eigenvectors[3];
            ComputeEigenDecomposition(eigenvalues, eigenvectors);

            return Matrix3
            {
                eigenvalues[0], 0, 0,
                0, eigenvalues[1], 0,
                0, 0, eigenvalues[2]
            };
        }

        [[nodiscard]] static constexpr Matrix3 Translate(const Vector2 <T> &v) noexcept
        {
            return Matrix3{ 1, 0, v[0], 0, 1, v[1], 0, 0, 1 };
        }

        [[nodiscard]] static constexpr Matrix3 Scale(T scalar) noexcept
        {
            return Matrix3{ scalar, 0, 0, 0, scalar, 0, 0, 0, scalar };
        }

        [[nodiscard]] static constexpr Matrix3 Scale(const Vector2 <T> &v) noexcept
        {
            return Matrix3 { v[0], 0, 0, 0, v[1], 0, 0, 0, 1 };
        }

        [[nodiscard]] static constexpr Matrix3 Rotate(const T angle) noexcept
        {
            T cosA = std::cos(angle);
            T sinA = std::sin(angle);

            return Matrix3
            {
                cosA, -sinA, 0,
                sinA, cosA, 0,
                0, 0, 1,
                MatOrder::ROW_MAJOR
            };
        }

        [[nodiscard]] static constexpr Matrix3 Scale(const Matrix3 &m, T scalar) noexcept
        {
            return m * scalar;
        }

        [[nodiscard]] static constexpr Matrix3 Scale(const Matrix3 &m, const Vector2 <T> &v) noexcept
        {
            return m * Scale(v);
        }

        [[nodiscard]] static constexpr Matrix3 Translate(const Matrix3 &m, const Vector2 <T> &v) noexcept
        {
            return m * Translate(v);
        }

        [[nodiscard]] static constexpr Matrix3 Rotate(const Matrix3 &m, const T angle) noexcept
        {
            return m * Rotate(angle);
        }

        void Print(bool transpose = false) const noexcept
        {
            for(int i = 0; i < 3; ++i)
            {
                for(int j = 0; j < 3; ++j)
                    transpose ? std::cout << m_data[j][i] << " " : std::cout << m_data[i][j] << " ";
                std::cout << std::endl;
            }
        }

        const float *Data() const { return reinterpret_cast <const float *>(m_data.data()); }

    private:
        static constexpr size_t N = 3;
        MatOrder m_order;
        float det = Determinant();
        std::array <Vector3 <T>, N> m_data;

        constexpr void ComputeEigenDecomposition(T eigenvalues[3], Vector3 <T> eigenvectors[3]) const noexcept
        {
            T a = -1;
            T b = m_data[0][0] + m_data[1][1] + m_data[2][2];
            T c = m_data[0][1] * m_data[1][0] + m_data[0][2] * m_data[2][0] + m_data[1][2] * m_data[2][1] -
                  (m_data[0][0] * m_data[1][1] + m_data[0][0] * m_data[2][2] + m_data[1][1] * m_data[2][2]);
            T d = Determinant();

            eigenvalues[0] = b / 3;
            eigenvalues[1] = (b + sqrt(b * b - 3 * c)) / 3;
            eigenvalues[2] = (b - sqrt(b * b - 3 * c)) / 3;

            for (int i = 0; i < 3; i++)
            {
                Vector3 <T> v(m_data[0][0] - eigenvalues[i], m_data[0][1], m_data[0][2]);
                eigenvectors[i] = v.Normalized();
            }
        }

        void SetIdentity() noexcept
        {
            for (size_t i = 0; i < N; ++i)
                for (size_t j = 0; j < N; ++j)
                    m_data[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    };
}