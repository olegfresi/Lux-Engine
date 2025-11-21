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

    // Secondary mesh parser attempt
    struct NewVertex
    {
        Vector3f position;
        Vector3f color;
        Vector2f texCoord;
        Vector3f normal;
    };

    // take a vector of Newvertex ojects and return a big vector of type float
    // that contains all the vertex data in the format that OpenGL expects
    inline std::vector<float> VertexVectorToFloatVector(const std::vector<NewVertex>& vertices)
    {
        std::vector<float> vertexData;
        vertexData.reserve(vertices.size() * 11);

        for(const auto& vertex : vertices)
        {
            vertexData.push_back(vertex.position.GetX());
            vertexData.push_back(vertex.position.GetY());
            vertexData.push_back(vertex.position.GetZ());
            vertexData.push_back(vertex.color.GetX());
            vertexData.push_back(vertex.color.GetY());
            vertexData.push_back(vertex.color.GetZ());
            vertexData.push_back(vertex.texCoord.GetX());
            vertexData.push_back(vertex.texCoord.GetY());
            vertexData.push_back(vertex.normal.GetX());
            vertexData.push_back(vertex.normal.GetY());
            vertexData.push_back(vertex.normal.GetZ());
        }

        return vertexData;
    }


    /*
    static std::unordered_map<std::string, Material> LoadMtl(const std::filesystem::path& filePath)
    {
        std::unordered_map<std::string, Material> materials;
        std::ifstream file(filePath, std::ios::in);
        std::string line, prefix, currentMaterial;
        std::stringstream ss;

        if (!file.is_open())
            throw std::runtime_error("Could not open MTL file: " + filePath.string());

        while (std::getline(file, line))
        {
            ss.clear();
            ss.str(line);
            ss >> prefix;

            if (prefix == "newmtl")
            {
                ss >> currentMaterial;
                materials[currentMaterial] = Material();
            }
            else if (prefix == "Ka" && !currentMaterial.empty()) // Ambient color
            {
                float r, g, b;
                ss >> r >> g >> b;
                materials[currentMaterial].ambientColor = Vector3f(r, g, b);
            }
            else if (prefix == "Kd" && !currentMaterial.empty()) // Diffuse color
            {
                float r, g, b;
                ss >> r >> g >> b;
                materials[currentMaterial].diffuseColor = Vector3f(r, g, b);
            }
            else if (prefix == "Ks" && !currentMaterial.empty()) // Specular color
            {
                float r, g, b;
                ss >> r >> g >> b;
                materials[currentMaterial].specularColor = Vector3f(r, g, b);
            }
            else if (prefix == "Ns" && !currentMaterial.empty()) // Shininess
            {
                ss >> materials[currentMaterial].shininess;
            }
            else if (prefix == "map_Kd" && !currentMaterial.empty()) // Diffuse texture map
            {
                ss >> materials[currentMaterial].diffuseTexture;
            }
        }

        return materials;
    }
    */
}