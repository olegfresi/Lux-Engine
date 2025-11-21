/*
 * Project: TestProject
 * File: ObjParser.hpp
 * Author: olegfresi
 * Created: 28/02/25 14:47
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
#include "../MeshLoader.hpp"

namespace  lux
{
    class OBJParser : public IMeshParser
    {
    public:

        MeshData ParseMesh(const std::filesystem::path& filename) override;

        void ReadCorner(std::string &description, std::vector<Vector3f> &v,
                 std::vector<Vector2f> &vt, std::vector<Vector3f> &vn, std::vector<float> &vertices,
         std::unordered_map<std::string, uint32_t> &history, std::vector<uint32_t> &indices);

        void ReadFace(std::vector<std::string> &words, std::vector<Vector3f> &v, std::vector<Vector2f> &vt,
                                  std::vector<Vector3f> &vn, std::vector<float> &vertices,
                                  std::unordered_map<std::string, uint32_t> &history, std::vector<uint32_t> &indices);

    private:
        std::vector<float> m_vertices;
        std::vector<uint32_t > m_indices;
        std::unordered_map<std::string, uint32_t> m_history;
        uint32_t m_elementCount;
    };

    struct Material
    {
        std::string name;
        std::string ambientTexture;
        std::string diffuseTexture;
        std::string specularTexture;
        std::string normalTexture;
        Vector3f ambient;
        Vector3f diffuse;
        Vector3f specular;
        float shininess = 0.0f;
        float transparency = 1.0f;
        float roughness = 0.0f;
        int illum = 0;
    };

    class MaterialParser
    {
    public:
        MaterialParser() = default;
        std::unordered_map<std::string, Material> ParseMaterial(const std::filesystem::path& filePath);
    };
}