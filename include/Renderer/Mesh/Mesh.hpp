/*
 * Project: TestProject
 * File: Mesh.hpp
 * Author: olegfresi
 * Created: 13/02/25 18:55
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
#include "../Texture/Texture2D.hpp"
#include "../Shader/Shader.hpp"
#include "../Buffer/Buffer.hpp"
#include "MeshParsers/ObjParser.hpp"
#include "../../OpenGL/MeshRenderer.hpp"
#include "../../Math/Transform.hpp"

namespace lux
{
    enum class MeshType : uint8_t
    {
        STATIC = 0,
        DYNAMIC = 1
    };


    // TODO: change mesh rapresentation buffer-layout
    class Mesh
    {
    public:
        Mesh(const MeshType type, NonOwnPtr<Shader> shader);
        Mesh(const MeshType type, NonOwnPtr<Shader> shader, const TextureSpecification& textureSpecs);
        Mesh(const Mesh& other);
        Mesh& operator=(const Mesh& other);

        void LoadMeshFromFile(const std::filesystem::path& filePath, const std::filesystem::path& materialPath)
        {
            if (filePath.extension() == ".obj")
            {
                MaterialParser materialParser;
                OBJParser parser;
                m_meshData = parser.ParseMesh(filePath);
                m_materials = materialParser.ParseMaterial(materialPath);
            }
            else if (filePath.extension() == ".fbx")
            {
                //FBXParser parser;
                // m_meshData = parser.ParseMesh(filePath);
            }
        }

        // TODO: implement other mesh file parsers
        void LoadMeshFromFile(const std::filesystem::path& filePath)
        {
            if (filePath.extension() == ".obj")
            {
                OBJParser parser;
                m_meshData = parser.ParseMesh(filePath);
            }
        }

        constexpr bool operator==(const Mesh& m) const noexcept
        {
            return this == &m || (m_shader == m.m_shader && m_meshData == m.m_meshData && m_type == m.m_type);
        }

        MeshData GetMeshData() const noexcept { return m_meshData; }
        MeshType GetMeshType() const noexcept { return m_type; }

        void SetInstanceMatrices(const std::vector<Matrix4f>& matrices) noexcept { m_instanceMatrices = matrices; }

        void SetupMesh() noexcept;
        void Draw(GPUDrawPrimitive primitive, GPUPrimitiveDataType type, uint32_t instances = 0, bool instanced = false) const noexcept;

        NonOwnPtr<Shader> GetShader() const noexcept { return m_shader; }
        void SetShader(NonOwnPtr<Shader> shader) { m_shader = shader; }

        Material m_material;
        std::unordered_map<std::string, Material> m_materials;
        std::vector<Texture2D> m_materialTextures;
        Transform m_modelMatrix;

    private:
        MeshType m_type;
        MeshData m_meshData;
        Buffer m_vbo{BufferType::VertexBuffer};
        Buffer m_ebo{BufferType::IndexBuffer};
        Buffer m_instanceVBO{BufferType::VertexBuffer};
        Scope<IVertexLayout> m_layout;
        Texture2D m_texture{};
        std::vector<Matrix4f> m_instanceMatrices;
        std::string m_samplerName{};
        NonOwnPtr<Shader> m_shader;
    };

    struct MeshInstance
    {
        Ref<Mesh> mesh;
        std::vector<Transform> transforms;
    };

    static void HashCombine(std::size_t& seed, std::size_t h)
    {
        seed ^= h + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
    }

    struct MeshDataHash
    {
        std::size_t operator()(const MeshData& data) const noexcept
        {
            std::size_t h = 0;

            for (uint32_t i : data.indices)
                HashCombine(h, std::hash<uint32_t>()(i));

            for (uint32_t i : data.vertices)
                HashCombine(h, std::hash<uint32_t>()(i));

            return h;
        }
    };

    struct MeshHash
    {
        std::size_t operator()(const Mesh& m) const noexcept
        {
            std::size_t seed = 0;
            HashCombine(seed, std::hash<int>()(static_cast<int>(m.GetMeshType())));

            MeshDataHash meshDataHash;
            HashCombine(seed, meshDataHash(m.GetMeshData()));

            return seed;
        }
    };

    struct MeshPtrHash
    {
        size_t operator()(const NonOwnPtr<Mesh>& ptr) const noexcept
        {
            return std::hash<Mesh*>()(ptr);
        }
    };

    struct MeshPtrEq
    {
        bool operator()(const NonOwnPtr<Mesh>& a, const NonOwnPtr<Mesh>& b) const noexcept
        {
            return a == b;
        }
    };
}