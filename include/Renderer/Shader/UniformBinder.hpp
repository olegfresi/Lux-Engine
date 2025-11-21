/*
 * Project: TestProject
 * File: UniformBinder.hpp
 * Author: olegfresi
 * Created: 12/09/25 17:09
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
#include <string>
#include <unordered_map>
#include <functional>
#include "Shader.hpp"
#include "../Mesh/Mesh.hpp"
#include "../../Scene/Scene.hpp"


enum class UniformSource : uint8_t
{
    CameraView,
    CameraProjection,
    CameraPosition,
    ModelMatrix,
    ViewMatrix,
    ProjectionMatrix,
    LightPosition,
    LightSpaceMatrix,
    ShadowMap,
    DiffuseTexture,
    SpecularTexture,
    NormalTexture
};


class UniformBinder
{
public:

    static void ApplyUniforms(Shader& shader, const Scene& scene, const Mesh& mesh)
    {
        for (auto& [name, source] : s_uniformSources)
            {
            if (!shader.HasUniform(name))
                continue;

            auto it = uniformRegistry.find(source);
            if (it != uniformRegistry.end()) {
                it->second(shader, scene, mesh);
            }
        }
    }

private:

    using UniformGenerator = std::function<void(Shader&, const Scene&, const Mesh&)>;

    static inline const std::unordered_map<std::string, UniformSource> s_uniformSources
    {
        { "model", UniformSource::ModelMatrix },
        { "view", UniformSource::CameraView },
        { "projection", UniformSource::CameraProjection },
        { "viewPos", UniformSource::CameraPosition },
        { "viewMatrix", UniformSource::ViewMatrix },
        { "projectionMatrix", UniformSource::ProjectionMatrix },
        { "lightPos", UniformSource::LightPosition },
        { "lightSpaceMatrix", UniformSource::LightSpaceMatrix },
        { "shadowMap", UniformSource::ShadowMap },
        { "diffuseTexture", UniformSource::DiffuseTexture },
        { "specularTexture", UniformSource::SpecularTexture },
        { "normalTexture", UniformSource::NormalTexture }
    };

    static inline const std::unordered_map<UniformSource, UniformGenerator> uniformRegistry
    {
    { UniformSource::ModelMatrix, [](Shader& s, const Scene&, const Mesh& m)
        {
            s.SetUniform("model", m.GetTransform());
        }},
    { UniformSource::CameraView, [](Shader& s, const Scene& sc, const Mesh&)
        {
            s.SetUniform("view", sc.GetCamera().GetView());
        }},
    { UniformSource::CameraProjection, [](Shader& s, const Scene& sc, const Mesh&)
        {
            s.SetUniform("projection", sc.GetCamera().GetProjection());
        }},
    { UniformSource::CameraPosition, [](Shader& s, const Scene& sc, const Mesh&)
        {
            s.SetUniform("viewPos", sc.GetCamera().GetPosition());
        }},
    { UniformSource::ViewMatrix, [](Shader& s, const Scene& sc, const Mesh&)
        {
            s.SetUniform("viewMatrix", sc.GetCamera().GetView());
        }},
    { UniformSource::ProjectionMatrix, [](Shader& s, const Scene& sc, const Mesh&)
        {
            s.SetUniform("projectionMatrix", sc.GetCamera().GetProjection());
        }},
    { UniformSource::LightPosition, [](Shader& s, const Scene& sc, const Mesh&)
        {
            s.SetUniform("lightPos", sc.GetLight().position);
        }},
    { UniformSource::LightSpaceMatrix, [](Shader& s, const Scene& scene, const Mesh&)
        {
            s.SetUniform("lightSpaceMatrix", scene.GetLightSpaceMatrix());
        }},
    { UniformSource::ShadowMap, [](Shader& s, const Scene&, const Mesh&)
        {
            s.SetUniform("shadowMap", shadowPass.GetShadowTextureUnit());
            shadowPass.BindDepthTexture();
        }},
    { UniformSource::DiffuseTexture, [](Shader& s, const Scene&, const Mesh&)
        {
            diffuseTexture.Bind();
            s.SetUniform("diffuseTexture", diffuseTexture.GetTextureUnit());
        }},
    { UniformSource::SpecularTexture, [](Shader& s, const Scene&, const Mesh&)
        {
            specularTexture.Bind();
            s.SetUniform("specularTexture", specularTexture.GetTextureUnit());
        }},
    { UniformSource::NormalTexture, [](Shader& s, const Scene&, const Mesh&)
        {
            normalTexture.Bind();
            s.SetUniform("normalTexture", normalTexture.GetTextureUnit());
        }}
    };
};


