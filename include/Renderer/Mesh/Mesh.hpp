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
        Mesh(const MeshType type, NonOwnPtr<Shader> shader, TextureSpecification textureSpecs);
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

        void SetupMesh() noexcept;
        void Draw(GPUDrawPrimitive primitive, GPUPrimitiveDataType type) const noexcept;

        std::vector<uint32_t> GetIndices() const noexcept { return m_meshData.indices; }

        NonOwnPtr<Shader> GetShader() const noexcept { return m_shader; }
        void SetShader(NonOwnPtr<Shader> shader) { m_shader = shader; }

        Material m_material;
        std::unordered_map<std::string, Material> m_materials;
        std::vector<Texture2D> m_materialTextures;

    private:
        MeshType m_type;
        MeshData m_meshData;
        Buffer m_vbo{BufferType::VertexBuffer};
        Buffer m_ebo{BufferType::IndexBuffer};
        Scope<IVertexLayout> m_layout;
        Texture2D m_texture;
        std::string m_samplerName{};
        NonOwnPtr<Shader> m_shader;
    };
}