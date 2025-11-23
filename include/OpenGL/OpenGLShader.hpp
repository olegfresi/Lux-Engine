/*
 * Project: TestProject
 * File: OpenGLShader.hpp
 * Author: olegfresi
 * Created: 28/03/25 19:26
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
#include "../Math/Matrix.hpp"
#include "../Renderer/Shader/Shader.hpp"

namespace lux
{
    class OpenGLShader : public ShaderImpl
    {
    public:
        OpenGLShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
        ~OpenGLShader() override;

        void Bind() const noexcept override;
        void Unbind() const noexcept override;

        [[nodiscard]] bool IsBound() const noexcept override;
        void SetUniform1f(const std::string& name, float value) noexcept override;
        void SetUniform1i(const std::string& name, int value) noexcept override;

        void SetUniform2i(const std::string& name, int v0, int v1) noexcept override;
        void SetUniform2f(const std::string& name, float v0, float v1) noexcept override;

        void SetUniform3i(const std::string& name, int v0, int v1, int v2) noexcept override;
        void SetUniform3f(const std::string& name, float v0, float v1, float v2) noexcept override;

        void SetUniform4i(const std::string& name, int v0, int v1, int v2, int v3) noexcept override;
        void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) noexcept override;

        void SetUniform1iv(const std::string& name, int* value, int count) noexcept override;
        void SetUniform1fv(const std::string& name, float* value, int count) noexcept override;

        void SetUniform2iv(const std::string& name, int* value, int count) noexcept override;
        void SetUniform2fv(const std::string& name, float* value, int count) noexcept override;

        void SetUniform3iv(const std::string& name, int* value, int count) noexcept override;
        void SetUniform3fv(const std::string& name, float* value, int count) noexcept override;

        void SetUniform4iv(const std::string& name, int* value, int count) noexcept override;
        void SetUniform4fv(const std::string& name, float* value, int count) noexcept override;

        void SetUniformMat3f(const std::string &name, const math::Matrix3f &mat) noexcept override;
        void SetUniformMat4f(const std::string &name, const math::Matrix4f &mat) noexcept override;

        int GetUniformLocation(const std::string& name) override;

        unsigned int GetId() const noexcept override { return m_programId; }

        void PreprocessShader() const noexcept override;

    private:
        unsigned int m_programId;
        std::string m_vertexSource;
        std::string m_fragmentSource;
        std::filesystem::path m_vertexPath;
        std::filesystem::path m_fragmentPath;
        std::unordered_map<std::string, int> m_UniformLocation;
    };
}