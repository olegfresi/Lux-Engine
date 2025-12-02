/*
 * Project: TestProject
 * File: Matrix.hpp
 * Author: olegfresi
 * Created: 05/02/25 14:08
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
#include "Matrix3.hpp"

namespace lux::math
{
    using Matrix3i = Matrix3<int>;
    using Matrix4i = Matrix4<int>;

    using Matrix3f = Matrix3<float>;
    using Matrix4f = Matrix4<float>;

    using Matrix3d = Matrix3<double>;
    using Matrix4d = Matrix4<double>;

    inline const Matrix3i Identity3i = Matrix3i::Identity();

    inline const Matrix4i Identity4i = Matrix4i::Identity();

    inline const Matrix3f Identity3f = Matrix3f::Identity();

    inline const Matrix4f Identity4f = Matrix4f::Identity();

    inline const Matrix3d Identity3d = Matrix3d::Identity();

    inline const Matrix4d Identity4d = Matrix4d::Identity();

    static constexpr std::vector<float> MatricesAsFloatVector(const std::vector<Matrix4f>& matrices)
    {
        std::vector<float> matrixData;
        matrixData.reserve(matrices.size() * 16);
        for (size_t i = 0; i < matrices.size(); ++i)
        {
            const auto& matrix = matrices[i];
            for (int col = 0; col < 4; ++col)
            {
                const auto& column = matrix.GetCol(col);
                matrixData.push_back(column.GetX());
                matrixData.push_back(column.GetY());
                matrixData.push_back(column.GetZ());
                matrixData.push_back(column.GetW());
            }
        }

        return matrixData;
    }
}