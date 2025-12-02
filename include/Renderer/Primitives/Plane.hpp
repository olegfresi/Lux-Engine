/*
 * Project: TestProject
 * File: Plane.hpp
 * Author: olegfresi
 * Created: 05/05/25 20:59
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
#include "IPrimitive.hpp"
#include "../Shader/Shader.hpp"
#include "../../Math/Color.hpp"
#include "../Buffer/Layout.hpp"
#include "../Buffer/Buffer.hpp"
#include <vector>

namespace lux
{

    using namespace math;

    class Plane : public IPrimitive
    {
    public:
        explicit Plane(NonOwnPtr<Shader> shader, const Vector3f& position = Vector3f{},
                       const Vector2f& size = Vector2f(1.0f, 1.0f), const Color& color = Color::Red());
        ~Plane() override = default;

        void Draw(const Matrix4f& view, const Matrix4f& projection) override;

        const Shader& GetShader() const noexcept override { return *m_shader; }
        void SetShader(NonOwnPtr<Shader> shader) override { m_shader = shader; }
        void AddTransform(const Matrix4f& transform) override
        {
            m_modelMatrix = transform;
        }

        void SetTexture(NonOwnPtr<Texture2D> texture) override
        {
            m_texture = texture;
        }

    private:

        std::vector<float> m_vertices;
        std::vector<uint32_t > m_indices;
        Layout m_layout; // = Layout::Create<Vector3f, Vector2f, Vector3f>();
        Scope<IVertexLayout> m_layoutScope;

        Vector3f m_position;
        Vector2f m_size;
        Color m_color;
        NonOwnPtr<Texture2D> m_texture;
        Buffer m_vbo{BufferType::VertexBuffer};
        Buffer m_ebo{BufferType::IndexBuffer};

        NonOwnPtr<Shader> m_shader;
        Matrix4f m_modelMatrix;
    };
}