/*
 * Project: TestProject
 * File: Shader.hpp
 * Author: olegfresi
 * Created: 31/01/25 18:16
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
#include "../../Math/Matrix.hpp"
#include "ShaderCompiler.hpp"
#include "../Texture/Texture.hpp"
#include "ShaderProgram.hpp"

namespace lux
{
    class ShaderImpl
    {
    public:
        virtual ~ShaderImpl() = default;

        virtual void Bind() const noexcept = 0;
        virtual void Unbind() const noexcept = 0;

        virtual void SetUniform1i(const std::string& name, int value) noexcept = 0;
        virtual void SetUniform1f(const std::string& name, float value) noexcept = 0;
        virtual void SetUniform1b(const std::string& name, bool value) noexcept = 0;

        virtual void SetUniform2i(const std::string& name, int v0, int v1) = 0;
        virtual void SetUniform2f(const std::string& name, float v0, float v1) = 0;

        virtual void SetUniform3i(const std::string& name, int v0, int v1, int v2) = 0;
        virtual void SetUniform3f(const std::string& name, float v0, float v1, float v2) = 0;

        virtual void SetUniform4i(const std::string& name, int v0, int v1, int v2, int v3) = 0;
        virtual void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) = 0;

        virtual void SetUniform1iv(const std::string& name, int* value, int count) noexcept = 0;
        virtual void SetUniform1fv(const std::string& name, float* value, int count) noexcept = 0;

        virtual void SetUniform2iv(const std::string& name, int* value, int count) noexcept = 0;
        virtual void SetUniform2fv(const std::string& name, float* value, int count) noexcept = 0;

        virtual void SetUniform3iv(const std::string& name, int* value, int count) noexcept = 0;
        virtual void SetUniform3fv(const std::string& name, float* value, int count) noexcept = 0;

        virtual void SetUniform4iv(const std::string& name, int* value, int count) noexcept = 0;
        virtual void SetUniform4fv(const std::string& name, float* value, int count) noexcept = 0;

        virtual void SetUniformMat3f(const std::string& name, const math::Matrix3f& mat) noexcept = 0;
        virtual void SetUniformMat4f(const std::string& name, const math::Matrix4f& mat) noexcept = 0;

        virtual void PreprocessShader() const noexcept = 0;

        virtual int GetUniformLocation(const std::string& name) = 0;
        virtual uint32_t GetId() const noexcept = 0;
        virtual bool IsBound() const noexcept = 0;
    };

    class Shader
    {
    public:

        Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);


        void Bind() const noexcept;
        void Unbind() const noexcept;

        template<typename T> requires std::is_fundamental_v<T>
        void SetUniform(const std::string& name, T value) noexcept;

        template<typename T> requires (!std::is_fundamental_v<T>)
        void SetUniform(const std::string& name, const T& value) noexcept;

        template<typename T>
        void SetUniform(const std::string& name, T* value, int count) noexcept;

        void PreprocessShader() const noexcept;

        int GetUniformLocation(const std::string& name) noexcept;
        uint32_t GetId() const noexcept;
        bool IsBound() const noexcept;

        std::string GetShaderName() const noexcept { return m_name; }

    protected:
        Shader() = default;
        explicit Shader(const std::string& shaderName) : m_program{nullptr}, m_name{shaderName} {}

    private:
        Scope<ShaderImpl> m_shaderAPI;
        Scope<ShaderImpl> CreateShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

        std::vector<std::string> m_textureImageNames;
        std::vector<NonOwnPtr<Texture>> m_textures;
        NonOwnPtr<ShaderProgram> m_program;
        std::string m_name;
    };

    template<>
    inline void Shader::SetUniform<uint32_t>(const std::string& name, uint32_t value) noexcept
    {
        m_shaderAPI->SetUniform1i(name, value);
    }

    template<>
    inline void Shader::SetUniform<int>(const std::string& name, int value) noexcept
    {
        m_shaderAPI->SetUniform1i(name, value);
    }

    template<>
    inline void Shader::SetUniform<bool>(const std::string& name, bool value) noexcept
    {
        m_shaderAPI->SetUniform1b(name, value);
    }

    template<>
    inline void Shader::SetUniform<float>(const std::string& name, float value) noexcept
    {
        m_shaderAPI->SetUniform1f(name, value);
    }

    template<>
    inline void Shader::SetUniform<int>(const std::string& name, int* value, int count) noexcept
    {
        m_shaderAPI->SetUniform1iv(name, value, count);
    }

    template<>
    inline void Shader::SetUniform<float>(const std::string& name, float* value, int count) noexcept
    {
        m_shaderAPI->SetUniform1fv(name, value, count);
    }

    template<>
    inline void Shader::SetUniform<math::Vector3f>(const std::string& name, const math::Vector3f& value) noexcept
    {
        m_shaderAPI->SetUniform3f(name, value.GetX(), value.GetY(), value.GetZ());
    }

    template<>
    inline void Shader::SetUniform<math::Vector4f>(const std::string& name, const math::Vector4f& value) noexcept
    {
        m_shaderAPI->SetUniform4f(name, value.GetX(), value.GetY(), value.GetZ(), value.GetW());
    }

    template<>
    inline void Shader::SetUniform<math::Matrix4f>(const std::string& name, const math::Matrix4f& value) noexcept
    {
        m_shaderAPI->SetUniformMat4f(name, std::cref(value));
    }

    std::string ReadShaderFile(const std::filesystem::path& filename);
}