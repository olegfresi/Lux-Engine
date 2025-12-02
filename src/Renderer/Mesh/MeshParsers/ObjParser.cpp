#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <string>
#include <list>
#include <vector>
#include <tuple>
#include <map>
#include <sys/stat.h>
#include <cassert>
#include "../../../include/Renderer/Mesh/MeshParsers/ObjParser.hpp"
#include "../../../include/FileSystem/FileSystem.hpp"
#include "../../../include/FileSystem/FileIO.hpp"
#include "../../../include/Application/Assertion.hpp"

namespace lux
{
    void OBJParser::ReadCorner(std::string &description, std::vector<Vector3f> &v,
                               std::vector<Vector2f> &vt, std::vector<Vector3f> &vn, std::vector<float> &vertices,
                               std::unordered_map<std::string, uint32_t> &history, std::vector<uint32_t> &indices)
    {
        if (!history.contains(description)) {
            history[description] = static_cast<uint32_t>(vertices.size() / 8);

            std::vector<std::string> v_vt_vn = FileSystem::Split(description, "/");
            Vector3f pos = v[std::stol(v_vt_vn[0]) - 1];
            vertices.push_back(pos[0]);
            vertices.push_back(pos[1]);
            vertices.push_back(pos[2]);

            if (v_vt_vn.size() > 1 && !v_vt_vn[1].empty())
            {
                Vector2f tex = vt[std::stol(v_vt_vn[1]) - 1];
                vertices.push_back(tex[0]);
                vertices.push_back(tex[1]);
            }
            else
            {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            if (v_vt_vn.size() > 2 && !v_vt_vn[2].empty()) {
                Vector3f normal = vn[std::stol(v_vt_vn[2]) - 1];
                vertices.push_back(normal[0]);
                vertices.push_back(normal[1]);
                vertices.push_back(normal[2]);
            } else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
        }
        indices.push_back(history[description]);
    }


    void OBJParser::ReadFace(std::vector<std::string> &words, std::vector<Vector3f> &v, std::vector<Vector2f> &vt,
                             std::vector<Vector3f> &vn, std::vector<float> &vertices,
                             std::unordered_map<std::string, uint32_t> &history, std::vector<uint32_t> &indices)
    {
        size_t triangleCount = words.size() - 3;
        for (size_t i = 0; i < triangleCount; ++i)
        {
            ReadCorner( words[1], v, vt, vn, vertices, history, indices );
            ReadCorner( words[i + 2], v, vt, vn, vertices, history, indices );
            ReadCorner( words[i + 3], v, vt, vn, vertices, history, indices );
        }
    }

    MeshData OBJParser::ParseMesh(const std::filesystem::path& filename)
    {
        if (!std::filesystem::exists(filename))
        {
            CORE_CRITICAL("File does not exist");
            throw std::runtime_error("File does not exist");
        }

        int lineCount = 0;

        std::vector<Vector3f> v;
        std::vector<Vector2f> vt;
        std::vector<Vector3f> vn;
        size_t vertexCount = 0, texCoordCount = 0, normalCount = 0, triangleCount = 0;

        filesys::FileReader fileReader(filename);

        while (fileReader.HasNextLine())
        {
            std::string line = fileReader.GetNextLine();

            std::vector<std::string> words = fileReader.SplitLine(line, " ");
            if (words[0] == "v")
                ++vertexCount;

            else if (words[0] == "vt")
                ++texCoordCount;

            else if (words[0] == "vn")
                ++normalCount;

            else if (words[0] == "f")
                triangleCount += words.size() - 3;
        }

        v.reserve(vertexCount);
        vt.reserve(texCoordCount);
        vn.reserve(normalCount);
        m_vertices.reserve(triangleCount * 3 * 8);
        m_indices.reserve(triangleCount * 3);

        fileReader.Reset();

        while (fileReader.HasNextLine())
        {
            std::string line = fileReader.GetNextLine();
            lineCount++;

            std::vector<std::string> words = fileReader.SplitLine(line, " ");
            if (words[0] == "v")
                v.push_back(ReadVec<Vector3f>(words));

            else if (words[0] == "vt")
                vt.push_back(ReadVec<Vector2f>(words));

            else if (words[0] == "vn")
                vn.push_back(ReadVec<Vector3f>(words));

            else if (words[0] == "f")
                ReadFace(words, v, vt, vn, m_vertices, m_history, m_indices);
        }

        m_elementCount = m_indices.size();
        Layout layout;
        return MeshData{ m_indices, m_vertices, layout };
    }

    std::unordered_map<std::string, Material> MaterialParser::ParseMaterial(const std::filesystem::path& filePath)
    {
        std::unordered_map<std::string, Material> materials;
        if (!std::filesystem::exists(filePath))
            throw std::runtime_error("MTL file does not exist");

        std::ifstream file(filePath, std::ios::in);
        std::string line;
        std::vector<std::string> words;
        Material *currentMaterial = nullptr;

        while (std::getline(file, line))
        {
            words = FileSystem::Split(line, " ");
            if (words.empty()) continue;

            if (words[0] == "newmtl" && words.size() > 1)
            {
                materials[words[1]] = Material{.name = words[1]};
                currentMaterial = &materials[words[1]];
            }
            else if (currentMaterial)
            {
                if (words[0] == "Ka" && words.size() >= 4)
                    currentMaterial->ambient = ReadVec<Vector3f>(words);

                else if (words[0] == "Kd" && words.size() >= 4)
                    currentMaterial->diffuse = ReadVec<Vector3f>(words);

                else if (words[0] == "Ks" && words.size() >= 4)
                    currentMaterial->specular = ReadVec<Vector3f>(words);

                else if (words[0] == "Ns" && words.size() >= 2)
                    currentMaterial->shininess = std::stof(words[1]);

                else if (words[0] == "d" && words.size() >= 2)
                    currentMaterial->transparency = std::stof(words[1]);

                else if (words[0] == "illum" && words.size() >= 2)
                    currentMaterial->illum = std::stoi(words[1]);

                else if (words[0] == "map_Ka" && words.size() >= 2)
                    currentMaterial->ambientTexture = words[1];

                else if (words[0] == "map_Kd" && words.size() >= 2)
                    currentMaterial->diffuseTexture = words[1];

                else if (words[0] == "map_Ks" && words.size() >= 2)
                    currentMaterial->specularTexture = words[1];

                else if ((words[0] == "map_bump" || words[0] == "bump") && words.size() >= 2)
                    currentMaterial->normalTexture = words[1];
            }
        }

        file.close();
        return materials;
    }
}