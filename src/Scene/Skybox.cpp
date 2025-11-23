#include "../../include/Scene/SkyBox.hpp"
#include "../../include/Renderer/Common/RenderCommand.hpp"

namespace lux
{
    SkyBox::SkyBox(const std::vector<std::filesystem::path>& paths,  NonOwnPtr<Shader> shader, const Vector3f& size, std::string samplerName) :
            m_filePaths{paths}, m_shader{shader}, m_cube{Vector3f(0.0f, 0.0f, 0.0f), size, Vector3f(0.0f, 0.0f, 0.0f), shader}
    {
        std::vector<std::filesystem::path> orderedFaces(6);

        for (const auto& path : paths)
        {
            const std::string filename = path.filename().string();
            if (filename.find("right") != std::string::npos)      orderedFaces[0] = path; // +X
            else if (filename.find("left") != std::string::npos)  orderedFaces[1] = path; // -X
            else if (filename.find("top") != std::string::npos)   orderedFaces[2] = path; // +Y
            else if (filename.find("bottom") != std::string::npos)orderedFaces[3] = path; // -Y
            else if (filename.find("front") != std::string::npos) orderedFaces[4] = path; // +Z
            else if (filename.find("back") != std::string::npos)  orderedFaces[5] = path; // -Z
        }

        for (size_t i = 0; i < orderedFaces.size(); ++i)
        {
            if (orderedFaces[i].empty())
                throw std::runtime_error("Missing cubemap face: index " + std::to_string(i));
        }
        m_filePaths = orderedFaces;
        m_cubeMap = CreateRef<CubeMap>(m_filePaths, samplerName);

        m_shader->Bind();
        uint32_t texUnit = m_cubeMap->GetTexture().GetTextureUnit();
        m_shader->SetUniform(samplerName, texUnit);
        m_shader->Unbind();
    }


    void SkyBox::DrawSkyBox(const Matrix4f& model, const Matrix4f& view, const Matrix4f& projection) noexcept
    {
        RenderCommand::EnableDepthTest(true, DepthTestFunction::LESS_OR_EQUAL);
        RenderCommand::EnableDepthWrite(false);
        RenderCommand::EnableFaceCulling(true, GPUCullFaceType::FRONT, GPUCullFaceOrder::COUNTER_CLOCKWISE);

        m_shader->Bind();
        m_shader->SetUniform("model", model);
        m_shader->SetUniform("view", view);
        m_shader->SetUniform("projection", projection);
        m_cubeMap->DrawCubeMap();
        m_cube.Draw(view, projection);
    }
}