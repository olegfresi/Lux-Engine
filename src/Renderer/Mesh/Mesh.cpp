#include "../../include/Renderer/Mesh/Mesh.hpp"
#include "../../include/OpenGL/MeshRenderer.hpp"
#include "../../include/Application/EngineSettings.hpp"

namespace lux
{
    Mesh::Mesh(const MeshType type, NonOwnPtr<Shader> shader) : m_type{type}, m_layout{CreateVertexLayout()}, m_shader{shader}, m_materials{} {}
    Mesh::Mesh(const MeshType type, NonOwnPtr<Shader> shader, TextureSpecification textureSpecs) : Mesh{type, shader}
    {
        Texture2D tex{textureSpecs};
        m_samplerName = textureSpecs.samplerName;
        m_texture = std::move(tex);
    }

    Mesh::Mesh(const Mesh& other) : m_shader{other.m_shader}, m_type{other.m_type}, m_meshData{other.m_meshData},
                              m_ebo{other.m_ebo}, m_vbo{other.m_vbo}, m_layout{other.m_layout->Clone()} {}

    Mesh& Mesh::operator=(const Mesh& other)
    {
        if (this != &other)
        {
            m_meshData = other.m_meshData;
            m_vbo = other.m_vbo;
            m_ebo = other.m_ebo;
            m_layout = other.m_layout->Clone();
        }

        return *this;
    }

    void Mesh::SetupMesh() noexcept
    {
        m_vbo.SetData( m_meshData.vertices, BufferUsage::StaticDraw, m_layout);
        m_ebo.SetData( m_meshData.indices, BufferUsage::StaticDraw, m_layout);
        m_layout->SetupLayout(m_meshData.layout);
    }


    void Mesh::Draw(GPUDrawPrimitive primitive, GPUPrimitiveDataType type) const noexcept
    {
        //m_shader->Bind();

        //m_shader->SetUniform(m_samplerName, m_texture.GetTextureUnit());
        //m_texture.Draw(m_texture.GetTextureUnit());
        MeshRenderer::Draw(primitive, type, m_ebo.GetSize(), m_layout);
        //m_shader->Unbind();
    }
}