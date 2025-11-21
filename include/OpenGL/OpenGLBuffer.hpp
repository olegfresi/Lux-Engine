/*
 * Project: TestProject
 * File: OpenGLBuffer.hpp
 * Author: olegfresi
 * Created: 26/02/25 17:09
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
#include "../Renderer/Buffer/Buffer.hpp"
#include "../Renderer/Buffer/FrameBuffer.hpp"
#include "../Application/EngineSettings.hpp"

namespace lux
{
    class OpenGLBuffer : public IBuffer
    {
    public:
        OpenGLBuffer() { INFO("OpenGLBuffer created"); }
        ~OpenGLBuffer() override
        {
            GLCheck(glDeleteBuffers(1, &m_id));
        }

        void CreateBuffer(BufferType type) override
        {
            m_type = type;
            GLCheck(glGenBuffers(1, &m_id));
            GLCheck(glGenBuffers(1, &m_vao));
        }

        void BindBuffer() override
        {
            GLCheck(glBindBuffer(static_cast<GLenum>(m_type), m_id));
        }

        void UnbindBuffer() override
        {
            GLCheck(glBindBuffer(static_cast<GLenum>(m_type), 0));
        }

        void SetData(const void* data, BufferUsage usage) override
        {
            BindBuffer();
            GLCheck(glBufferData(static_cast<GLenum>(m_type), m_size, data, static_cast<GLenum >(usage)));
        }

        void Update(const void* data) override
        {
            BindBuffer();
            GLCheck(glBufferSubData(static_cast<GLenum>(m_type), 0, sizeof(data), data));
            UnbindBuffer();
        }

        [[nodiscard]] uint32_t GetId() const noexcept override { return m_id; }
        [[nodiscard]] size_t GetSize() const noexcept override { return m_size; }

        void SetDataSize(uint32_t size) override
        {
            m_size = size;
        }

        Scope<IBuffer> Clone() const override
        {
            return CreateScope<OpenGLBuffer>(*this);
        }

    private:
        uint32_t m_id;
        uint32_t m_vao;
        BufferType m_type;
        uint32_t m_size;
    };

    class OpenGLVertexLayout : public IVertexLayout
    {
    public:
        OpenGLVertexLayout()
        {
            INFO("OpenGLVertexLayout created");
            GLCheck(glGenVertexArrays(1, &m_vao));
        }

        void SetupLayout(const Layout& l) const noexcept override
        {
            Bind();
            for (const auto &attr : l.attributes)
            {
                GLCheck(glEnableVertexAttribArray(attr.index));
                GLCheck(glVertexAttribPointer(attr.index, attr.size, static_cast<GLenum >(attr.type), attr.normalized, attr.stride, attr.pointer));
            }
            Unbind();
        }

        void Bind() const noexcept override
        {
            GLCheck(glBindVertexArray(m_vao));
        }

        void Unbind() const noexcept override
        {
            GLCheck(glBindVertexArray(0));
        }

        uint32_t GetVao() const noexcept { return m_vao; }

        std::unique_ptr<IVertexLayout> Clone() const override
        {
            return std::make_unique<OpenGLVertexLayout>(*this);
        }

    private:
        uint32_t m_vao;
    };

    Scope<IVertexLayout> CreateVertexLayout()
    {
        switch (API)
        {
            case GraphicsAPI::OPENGL:
                return std::make_unique<OpenGLVertexLayout>();
            case GraphicsAPI::VULKAN:
                break;
            case GraphicsAPI::DIRECTX:
                break;
            case GraphicsAPI::METAL:
                break;
        }

        return nullptr;
    }
}