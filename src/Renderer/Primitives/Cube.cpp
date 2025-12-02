#include "../../../include/OpenGL/MeshRenderer.hpp"
#include "../../../include/Renderer/Primitives/Cube.hpp"

namespace lux
{
    std::vector<float> cubeVertices =
    {
        // Front face
        -0.5f, -0.5f,  0.5f,  0, 0,  0, 0, 1,  // 0
        0.5f, -0.5f,  0.5f,  1, 0,  0, 0, 1,  // 1
        0.5f,  0.5f,  0.5f,  1, 1,  0, 0, 1,  // 2
        -0.5f,  0.5f,  0.5f,  0, 1,  0, 0, 1,  // 3

        // Back face
        0.5f, -0.5f, -0.5f,  0, 0,  0, 0, -1, // 4
        -0.5f, -0.5f, -0.5f,  1, 0,  0, 0, -1, // 5
        -0.5f,  0.5f, -0.5f,  1, 1,  0, 0, -1, // 6
        0.5f,  0.5f, -0.5f,  0, 1,  0, 0, -1, // 7

        // Left face
        -0.5f, -0.5f, -0.5f,  0, 0, -1, 0, 0,  // 8
        -0.5f, -0.5f,  0.5f,  1, 0, -1, 0, 0,  // 9
        -0.5f,  0.5f,  0.5f,  1, 1, -1, 0, 0,  // 10
        -0.5f,  0.5f, -0.5f,  0, 1, -1, 0, 0,  // 11

        // Right face
        0.5f, -0.5f,  0.5f,  0, 0,  1, 0, 0,  // 12
        0.5f, -0.5f, -0.5f,  1, 0,  1, 0, 0,  // 13
        0.5f,  0.5f, -0.5f,  1, 1,  1, 0, 0,  // 14
        0.5f,  0.5f,  0.5f,  0, 1,  1, 0, 0,  // 15

        // Top face
        -0.5f,  0.5f,  0.5f,  0, 0,  0, 1, 0,  // 16
        0.5f,  0.5f,  0.5f,  1, 0,  0, 1, 0,  // 17
        0.5f,  0.5f, -0.5f,  1, 1,  0, 1, 0,  // 18
        -0.5f,  0.5f, -0.5f,  0, 1,  0, 1, 0,  // 19

        // Bottom face
        -0.5f, -0.5f, -0.5f,  0, 0,  0, -1, 0, // 20
        0.5f, -0.5f, -0.5f,  1, 0,  0, -1, 0, // 21
        0.5f, -0.5f,  0.5f,  1, 1,  0, -1, 0, // 22
        -0.5f, -0.5f,  0.5f,  0, 1,  0, -1, 0  // 23
};


    Cube::Cube(const Vector3f& position, const Vector3f& size, const Vector3f& color,  NonOwnPtr<Shader> shader) :
                m_layoutScope{CreateVertexLayout()}, m_texture{}, m_position{position},
                m_size{size}, m_color{color}, m_shader {shader}
    {
        m_vertices = cubeVertices;
        m_indices =
        {
            0, 1, 2, 0, 2, 3,       // front
            4, 5, 6, 4, 6, 7,       // back
            8, 9,10, 8,10,11,       // left
            12,13,14,12,14,15,      // right
            16,17,18,16,18,19,      // top
            20,21,22,20,22,23       // bottom
        };

        m_modelMatrix = Matrix4f::Translate(m_position) * Matrix4f::Scale(m_size);
        m_vbo.SetData(m_vertices, BufferUsage::StaticDraw, m_layoutScope);
        m_ebo.SetData(m_indices, BufferUsage::StaticDraw, m_layoutScope);


        m_layout.Push<Vector3f>(GPUPrimitiveDataType::FLOAT, false);
        m_layout.Push<Vector2f>(GPUPrimitiveDataType::FLOAT, false);
        m_layout.Push<Vector3f>(GPUPrimitiveDataType::FLOAT, false);
        m_layout.Finalize();
        std::vector<std::pair<const Layout&, const Buffer&>> vec{{m_layout, m_vbo}};
        m_layoutScope->SetupLayout(vec);
    }

    void Cube::Draw(const Matrix4f& view, const Matrix4f& projection)
    {
        m_shader->Bind();
        m_shader->SetUniform("model", m_modelMatrix);
        m_shader->SetUniform("view", view);
        m_shader->SetUniform("projection", projection);
        MeshRenderer::Draw(GPUDrawPrimitive::TRIANGLES, GPUPrimitiveDataType::UNSIGNED_INT, m_ebo.GetSize(), m_layoutScope);
    }

    void Cube::AddTransform(const Matrix4f& transform)
    {
        m_modelMatrix = transform;
    }
}