/*
 * Project: TestProject
 * File: MeshLoader.hpp
 * Author: olegfresi
 * Created: 28/02/25 13:46
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
#include <filesystem>
#include <vector>
#include <fstream>
#include "../Buffer/Layout.hpp"
#include "../../Math/Vector.hpp"

namespace lux
{
    struct MeshData
    {
        std::vector<uint32_t> indices;
        std::vector<float> vertices;
        Layout layout;

        bool operator==(const MeshData& m) const
        {
            return this == &m || (indices == m.indices && vertices == m.vertices);
        }
    };


    class IMeshParser
    {
    public:
        virtual ~IMeshParser() = default;
        virtual MeshData ParseMesh(const std::filesystem::path& filePath) = 0;
    };

    template<typename Vector>
    Vector ReadVec(const std::vector<std::string>& words)
    {
        Vector vec;
        for(size_t i = 0; i < Vector::GetVectorSize(); i++)
            vec[i] = std::stof(words[i + 1]);

        return vec;
    }
}