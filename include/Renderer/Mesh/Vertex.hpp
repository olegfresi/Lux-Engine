/*
 * Project: TestProject
 * File: Vertex.hpp
 * Author: olegfresi
 * Created: 31/01/25 18:58
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
#include "../Common/GPU.hpp"

namespace lux
{
    using namespace math;

    // Struct to hold the vertex attributes to later use setup buffer layout
    struct VertexAttributeDescription
    {
        uint32_t index;
        int size;
        GPUPrimitiveDataType type;
        bool normalized;
        int stride;
        const void* pointer;
    };

    /*
    // Generic class for representing a vertex constructed from different type of components
    template <typename... Components>
    class Vertex
    {
    public:

        Vertex(const Vertex<Components...> &other) : data( other.data) {}

        Vertex(Components &&... args) : data( std::forward<Components>( args)...) {}
        Vertex(const Components &... args) : data( args...) {}

        operator std::tuple<Components...>() { return data; }

        static const std::array<VertexAttributeDescription, sizeof...(Components)> GetAttributeDescriptions()
        {
            std::size_t offset = 0, index = 0;
            size_t stride = sizeof(std::tuple<Components...>);

            return
            {
                VertexAttributeDescription
                {
                        uint32_t(index++), GetSizeFromType<Components>(), GetPrimitiveDataTypeFromType<Components>(), false,
                        static_cast<int>(stride), (void*)((offset += sizeof(Components)) - sizeof(Components))
                }...
            };
        }
    private:
        std::tuple<Components...> data;
    };
    */

    // Integrate fields of this struct into already existing mesh class
    struct NewMesh
    {
        std::vector<Vector3f> position;
        std::vector<Vector3f> normal;
        std::vector<Vector3f> tangent;
        std::vector<Vector3f> bitangent;
        std::vector<Vector3f> color;
        std::vector<Vector2f> uv;

        bool HasUv() const
        {
            return !uv.empty();
        }

        bool HasColor() const
        {
            return !color.empty();
        }

        bool HasNormal() const
        {
            return !normal.empty();
        }

        bool HasTangent() const
        {
            return !tangent.empty();
        }

        bool HasBitangent() const
        {
            return !bitangent.empty();
        }

        bool HasPosition() const
        {
            return !position.empty();
        }
    };
}