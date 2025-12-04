#include <fstream>
#include "../../include/Renderer/Shader/Shader.hpp"
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

            default:
                assert(false);
        }
    }

    Shader::Shader(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath)
        : m_program{nullptr}, m_shaderAPI(CreateShader(vertexPath, fragmentPath)){}

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

    void Shader::PreprocessShader() const noexcept
    {
        m_shaderAPI->PreprocessShader();
    }
}