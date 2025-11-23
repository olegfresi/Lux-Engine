#include "../../../include/OpenGL/MeshRenderer.hpp"
#include "../../../include/Renderer/Primitives/Plane.hpp"

namespace lux
{
    const std::vector<float> planeVertices =
    {
        // Pos               // UV         // Normal (top face points up)
        -1.0f, 0.0f, -1.0f,   0.0f, 1.0f,   0.0f,   1.0f, 0.0f,  // v0
         1.0f, 0.0f, -1.0f,   1.0f, 1.0f,   0.0f,   1.0f, 0.0f,  // v1
         1.0f, 0.0f,  1.0f,   1.0f, 0.0f,   0.0f,   1.0f, 0.0f,  // v2
        -1.0f, 0.0f,  1.0f,   0.0f, 0.0f,   0.0f,   1.0f, 0.0f,  // v3

        // Pos               // UV         // Normal (bottom face points down)
        -1.0f, 0.0f, -1.0f,   0.0f, 1.0f,   0.0f,  -1.0f, 0.0f,  // v4
         1.0f, 0.0f, -1.0f,   1.0f, 1.0f,   0.0f,  -1.0f, 0.0f,  // v5
         1.0f, 0.0f,  1.0f,   1.0f, 0.0f,   0.0f,  -1.0f, 0.0f,  // v6
        -1.0f, 0.0f,  1.0f,   0.0f, 0.0f,   0.0f,  -1.0f, 0.0f   // v7
    };


    Plane::Plane(NonOwnPtr<Shader> shader, const Vector3f& position, const Vector2f& size, const Color& color) :
                 m_layoutScope(CreateVertexLayout()), m_position{position}, m_size{size}, m_color{color}, m_texture{}, m_shader { shader }
    {
        m_vertices = planeVertices;
        m_indices =
        {
            // Top face (CCW when seen from above -> normal +Y)
            0, 2, 1,
            0, 3, 2,

            // Bottom face (CCW when seen from below -> normal -Y)
            4, 5, 6,
            4, 6, 7
        };

        m_modelMatrix = Matrix4f::Translate(m_position) * Matrix4f::Scale(Vector3f{m_size.GetX(), 1.0f, m_size.GetY()});
        m_vbo.SetData(m_vertices, BufferUsage::StaticDraw, m_layoutScope);
        m_ebo.SetData(m_indices, BufferUsage::StaticDraw, m_layoutScope);
        m_layoutScope->SetupLayout(m_layout);
    }

    void Plane::Draw(const Matrix4f& view, const Matrix4f& projection)
    {
        m_shader->Bind();
        m_shader->SetUniform("model", m_modelMatrix);
        m_shader->SetUniform("view", view);
        m_shader->SetUniform("projection", projection);
        MeshRenderer::Draw(GPUDrawPrimitive::TRIANGLES, GPUPrimitiveDataType::UNSIGNED_INT, m_ebo.GetSize(), m_layoutScope);
    }
}