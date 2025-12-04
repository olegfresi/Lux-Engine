#include "../../include/Renderer/Mesh/Mesh.hpp"
#include "../../include/OpenGL/MeshRenderer.hpp"


namespace lux
{
    Mesh::Mesh(const MeshType type, NonOwnPtr<Shader> shader) : m_type{type}, m_layout{CreateVertexLayout()}, m_shader{shader} {}
    Mesh::Mesh(const MeshType type, NonOwnPtr<Shader> shader, const TextureSpecification& textureSpecs) : Mesh{type, shader}
    {
        Texture2D tex{textureSpecs};
        m_samplerName = textureSpecs.samplerName;
        m_texture = std::move(tex);
    }

    Mesh::Mesh(const Mesh& other) : m_type{other.m_type}, m_meshData{other.m_meshData}, m_vbo{other.m_vbo},
            m_ebo{other.m_ebo}, m_layout{other.m_layout->Clone()}, m_shader{other.m_shader} {}
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
        m_vbo.SetData(m_meshData.vertices, BufferUsage::StaticDraw, m_layout);
        m_ebo.SetData(m_meshData.indices, BufferUsage::StaticDraw, m_layout);

        m_layout->SetupLayout({{m_meshData.layout, m_vbo}});
    }

    void Mesh::SetupMeshInstanced(const std::vector<Transform>& instanceMatrices) noexcept
    {
        Layout vertexLayout;
        vertexLayout.Push<Vector3f>(GPUPrimitiveDataType::FLOAT, false);
        vertexLayout.Push<Vector2f>(GPUPrimitiveDataType::FLOAT, false);
        vertexLayout.Push<Vector3f>(GPUPrimitiveDataType::FLOAT, false);
        vertexLayout.Finalize();

        Layout instanceLayout;
        instanceLayout.index = 3;
        instanceLayout.Push<Vector4f>(GPUPrimitiveDataType::FLOAT, true);
        instanceLayout.Push<Vector4f>(GPUPrimitiveDataType::FLOAT, true);
        instanceLayout.Push<Vector4f>(GPUPrimitiveDataType::FLOAT, true);
        instanceLayout.Push<Vector4f>(GPUPrimitiveDataType::FLOAT, true);
        instanceLayout.Finalize();

        std::vector<Matrix4f> matrices;
        matrices.reserve(instanceMatrices.size());

        for (const auto& t : instanceMatrices)
            matrices.push_back(t.ToMatrix4());

        auto matrixData = MatricesAsFloatVector(matrices);

        m_vbo.SetData(m_meshData.vertices, BufferUsage::StaticDraw, m_layout);
        m_ebo.SetData(m_meshData.indices,  BufferUsage::StaticDraw, m_layout);
        m_instanceVBO.SetData(matrixData,  BufferUsage::StaticDraw, m_layout);

        m_layout->SetupLayout({ { vertexLayout,  m_vbo },{ instanceLayout, m_instanceVBO }});
    }


    void Mesh::Draw(GPUDrawPrimitive primitive, GPUPrimitiveDataType type, uint32_t instances, bool instanced) const noexcept
    {
        m_shader->Bind();
        //m_shader->SetUniform(m_samplerName, m_texture.GetTextureUnit());
        //m_texture.Draw(m_texture.GetTextureUnit());
        uint32_t indexCount = static_cast<uint32_t>(m_meshData.indices.size());
        if (instanced)
        {
            m_shader->SetUniform("useInstancing", true);
            MeshRenderer::DrawInstanced(primitive, type, indexCount, m_layout, instances);
        }
        else
        {
            m_shader->SetUniform("useInstancing", false);
            MeshRenderer::Draw(primitive, type, indexCount, m_layout);
        }

        //m_shader->Unbind();
    }
}
