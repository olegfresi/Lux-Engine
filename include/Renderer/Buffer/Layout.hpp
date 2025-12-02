/*
 * Project: TestProject
 * File: Layout.hpp
 * Author: olegfresi
 * Created: 27/02/25 21:27
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
#include <vector>
#include <unordered_map>
#include <iostream>
#include "../Mesh/Vertex.hpp"

namespace lux
{
    /*
    struct Layout
    {
        std::vector<VertexAttributeDescription> attributes;


        template <typename... Components>
        static Layout Create()
        {
            Layout l;
            std::size_t offset = 0;
            std::size_t index = 0;
            constexpr std::size_t stride = (sizeof(Components) + ...);

            ([&] {
                l.attributes.push_back({
                    static_cast<uint32_t>(index++),
                                               GPUDataType::GetSizeFromType<Components>(),
                                               GPUDataType::GetPrimitiveDataTypeFromType<Components>(),
                                               false,
                                               static_cast<int>(stride),
                                               reinterpret_cast<void*>(offset),
                    0
                                       });
                offset += sizeof(Components);
            }(), ...);

            return l;
        }
        */


    struct Layout
    {
        template<typename T>
        void Push(GPUPrimitiveDataType dataType, bool instanced)
        {
            attributes.push_back({
                index++,
                GPUDataType::GetSizeFromType<T>(),
                dataType,
                false,
                0, // Stride will be update later by Finalize
                reinterpret_cast <void *>(offset),
                instanced ? 1u : 0u
            });

            offset += sizeof(T);
            stride += sizeof(T);
        }

        void Finalize()
        {
            for (auto &e : attributes)
                e.stride = stride;
        }

        uint32_t index = 0;
        uint32_t stride = 0;
        std::size_t offset = 0;

        std::vector <VertexAttributeDescription> attributes;
    };
};

enum class AttributeType
{
    Float, Vec2, Vec3, Vec4
};

enum class VertexAttributeType
{
    Position,
    Normal,
    TexCoord,
    Color,
    Tangent,
    Bitangent
};

struct VertexAttribute
{
    AttributeType type;
    uint32_t offset;
};


class VertexLayout
{
public:
    void AddAttribute(VertexAttributeType semantic, AttributeType type)
    {
        uint32_t size = GetSize(type);
        attributes[semantic] = { type, stride };
        stride += size;
    }

    uint32_t GetStride() const { return stride; }
    const std::unordered_map <VertexAttributeType, VertexAttribute> &GetAttributes() const { return attributes; }

private:
    uint32_t stride = 0;
    std::unordered_map <VertexAttributeType, VertexAttribute> attributes;

    static uint32_t GetSize(AttributeType type)
    {
        switch (type)
        {
            case AttributeType::Float: return 4;
            case AttributeType::Vec2: return 8;
            case AttributeType::Vec3: return 12;
            case AttributeType::Vec4: return 16;
        }
        return 0;
    }
};

struct MeshDataNew
{
    std::vector <uint32_t> indices;
    std::vector <float> vertices;
    VertexLayout layout;
};

template<typename... Attributes>
MeshDataNew CreateMeshData(const std::vector <float> &vertexData, const std::vector <uint32_t> &indexData,
                           Attributes... attrs)
{
    MeshDataNew mesh;
    mesh.vertices = vertexData;
    mesh.indices = indexData;

    (mesh.layout.AddAttribute(attrs.first, attrs.second), ...);

    return mesh;
}
