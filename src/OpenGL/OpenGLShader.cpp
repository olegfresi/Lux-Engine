#include "../../include/OpenGL/OpenGLShader.hpp"
#include "../../include/OpenGL/OpenglError.hpp"
#include <fstream>

namespace lux
{
    OpenGLShader::OpenGLShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) : m_vertexPath{vertexPath}, m_fragmentPath{fragmentPath}, m_programId{0}
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        if (!std::filesystem::exists(vertexPath))
            throw std::runtime_error("Vertex shader file does not exist: " + vertexPath.generic_string());

        if (!std::filesystem::exists(fragmentPath))
            throw std::runtime_error("Fragment shader file does not exist: " + fragmentPath.generic_string());

        try
        {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch(std::ifstream::failure&)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        GLCheck(vertex = glCreateShader(GL_VERTEX_SHADER));
        GLCheck(glShaderSource(vertex, 1, &vShaderCode, nullptr));
        GLCheck(glCompileShader(vertex));

        GLCheck(fragment = glCreateShader(GL_FRAGMENT_SHADER));
        GLCheck(glShaderSource(fragment, 1, &fShaderCode, nullptr));
        GLCheck(glCompileShader(fragment));

        GLCheck(glGetShaderiv(vertex, GL_COMPILE_STATUS, &success));
        if(!success)
        {
            GLCheck(glGetShaderInfoLog(vertex, 512, nullptr, infoLog));
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        GLCheck(glGetShaderiv(fragment, GL_COMPILE_STATUS, &success));
        if(!success)
        {
            GLCheck(glGetShaderInfoLog(fragment, 512, nullptr, infoLog));
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        GLCheck(m_programId = glCreateProgram());
        GLCheck(glAttachShader(m_programId, vertex));
        GLCheck(glAttachShader(m_programId, fragment));
        GLCheck(glLinkProgram(m_programId));

        GLCheck(glGetProgramiv(m_programId, GL_LINK_STATUS, &success));
        if(!success)
        {
            GLCheck(glGetProgramInfoLog(m_programId, 512, nullptr, infoLog));
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        GLCheck(glDeleteShader(vertex));
        GLCheck(glDeleteShader(fragment));
    }

    OpenGLShader::~OpenGLShader()
    {
        GLCheck(glUseProgram(0));
        GLCheck(glDeleteProgram(m_programId));
    }

    void OpenGLShader::Bind() const noexcept
    {
        GLCheck(glUseProgram(m_programId));
    }

    void OpenGLShader::Unbind() const noexcept
    {
        GLCheck(glUseProgram(0));
    }

    [[nodiscard]] bool OpenGLShader::IsBound() const noexcept
    {
        int currentProgram;
        GLCheck(glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram));
        return currentProgram == static_cast<int>(m_programId);
    }

    int OpenGLShader::GetUniformLocation(const std::string& name)
    {
        if(!IsBound())
            CORE_ERROR("Shader is not bound. Cannot get uniform location");

        if (m_UniformLocation.contains( name ))
            return m_UniformLocation[name];

        GLCheck(int location = glGetUniformLocation(m_programId, name.c_str()));
        if (location == -1)
            throw std::runtime_error("Uniform " + name + " not found in shader");

        m_UniformLocation[name] = location;

        return location;
    }

    void OpenGLShader::SetUniform1i(const std::string &name, int value) noexcept
    {
        GLCheck(glUniform1i(GetUniformLocation(name), value));
    }

    void OpenGLShader::SetUniform1f(const std::string& name, float value) noexcept
    {
        GLCheck(glUniform1f(GetUniformLocation(name), value));
    }

    void OpenGLShader::SetUniform2i(const std::string &name, int v0, int v1) noexcept
    {
        GLCheck(glUniform2i(GetUniformLocation(name), v0, v1));
    }

    void OpenGLShader::SetUniform2f(const std::string &name, float v0, float v1) noexcept
    {
        GLCheck(glUniform2f(GetUniformLocation(name), v0, v1));
    }

    void OpenGLShader::SetUniform3i(const std::string &name, int v0, int v1, int v2) noexcept
    {
        GLCheck(glUniform3i(GetUniformLocation(name), v0, v1, v2));
    }

    void OpenGLShader::SetUniform3f(const std::string &name, float v0, float v1, float v2) noexcept
    {
        GLCheck(glUniform3f(GetUniformLocation(name), v0, v1, v2));
    }

    void OpenGLShader::SetUniform4i(const std::string &name, int v0, int v1, int v2, int v3) noexcept
    {
        GLCheck(glUniform4i(GetUniformLocation(name), v0, v1, v2, v3));
    }

    void OpenGLShader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3) noexcept
    {
        GLCheck(glUniform4f(GetUniformLocation(name), f0, f1, f2, f3));
    }

    void OpenGLShader::SetUniform1iv(const std::string &name, int *value, int count) noexcept
    {
        GLCheck(glUniform1iv(GetUniformLocation(name), count, value));
    }

    void OpenGLShader::SetUniform1fv(const std::string &name, float *value, int count) noexcept
    {
        GLCheck(glUniform1fv(GetUniformLocation(name), count, value));
    }

    void OpenGLShader::SetUniform2iv(const std::string &name, int *value, int count) noexcept
    {
        GLCheck(glUniform2iv(GetUniformLocation(name), count, value));
    }

    void OpenGLShader::SetUniform2fv(const std::string &name, float *value, int count) noexcept
    {
        GLCheck(glUniform2fv(GetUniformLocation(name), count, value));
    }

    void OpenGLShader::SetUniform3iv(const std::string &name, int *value, int count) noexcept
    {
        GLCheck(glUniform3iv(GetUniformLocation(name), count, value));
    }

    void OpenGLShader::SetUniform3fv(const std::string &name, float *value, int count) noexcept
    {
        GLCheck(glUniform3fv(GetUniformLocation(name), count, value));
    }

    void OpenGLShader::SetUniform4iv(const std::string &name, int *value, int count) noexcept
    {
        GLCheck(glUniform4iv(GetUniformLocation(name), count, value));
    }

    void OpenGLShader::SetUniform4fv(const std::string &name, float *value, int count) noexcept
    {
        GLCheck(glUniform4fv(GetUniformLocation(name), count, value));
    }

    void OpenGLShader::SetUniformMat3f(const std::string &name, const math::Matrix3f &mat) noexcept
    {
        GLCheck(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, mat.Data()));
    }

    void OpenGLShader::SetUniformMat4f(const std::string &name, const math::Matrix4f &mat) noexcept
    {
        GLCheck(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, mat.Data()));
    }

    void OpenGLShader::PreprocessShader() noexcept
    {

    }

    std::string ReadShaderFile(const std::filesystem::path& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            CORE_ERROR( "Could not open file: {0}", filename.c_str() );
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        return buffer.str();
    }
}