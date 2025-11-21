#include <fstream>
#include <sstream>
#include <iostream>
#include "../../include/Renderer/Shader/Shader.hpp"
#include "../../include/OpenGL/OpenglError.hpp"
#include "../../../include/Renderer/Common/GPU.hpp"
#include "../../include/Renderer/Shader/ShaderCompiler.hpp"
#include "../../include/Application/EngineSettings.hpp"
#include "../../include/OpenGL/OpenGLShader.hpp"

namespace lux
{
    Scope<ShaderImpl> Shader::CreateShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
    {
        switch (API)
        {
            case GraphicsAPI::OPENGL:
                return CreateScope<OpenGLShader>(vertexPath, fragmentPath);

            case GraphicsAPI::VULKAN:
                break;

            case GraphicsAPI::DIRECTX:
                break;

            case GraphicsAPI::METAL:
                break;
        }
    }

    Shader::Shader(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath)
        : m_shaderAPI(CreateShader(vertexPath, fragmentPath)){}

    void Shader::Bind() const noexcept
    {
        m_shaderAPI->Bind();
    }

    void Shader::Unbind() const noexcept
    {
        m_shaderAPI->Unbind();
    }

    bool Shader::IsBound() const noexcept
    {
        return m_shaderAPI->IsBound();
    }

    int Shader::GetUniformLocation(const std::string& name) noexcept
    {
        return m_shaderAPI->GetUniformLocation(name);
    }

    unsigned int Shader::GetId() const noexcept
    {
        return m_shaderAPI->GetId();
    }

    void Shader::PreprocessShader() noexcept
    {
        m_shaderAPI->PreprocessShader();
    }
}