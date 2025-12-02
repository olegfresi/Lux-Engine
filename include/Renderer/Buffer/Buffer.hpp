/*
 * Project: TestProject
 * File: Bufefr.hpp
 * Author: olegfresi
 * Created: 20/02/25 21:21
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
#include <vector>
#include <memory>
#include "../../OpenGL/OpenglError.hpp"
#include "../Common/GPU.hpp"
#include "Layout.hpp"

namespace lux
{
    class Buffer;

    class IVertexLayout
    {
    public:
        virtual ~IVertexLayout() = default;
        virtual void SetupLayout(const std::vector<std::pair<const Layout&, const Buffer&>>& vbos) const noexcept = 0;
        virtual void Bind() const noexcept = 0;
        virtual void Unbind() const noexcept = 0;
        virtual std::unique_ptr<IVertexLayout> Clone() const = 0;
    };

    static Scope<IVertexLayout> CreateVertexLayout();

    class IBuffer
    {
    public:
        virtual ~IBuffer() = default;
        virtual void CreateBuffer(BufferType type) = 0;
        virtual void BindBuffer() = 0;
        virtual void UnbindBuffer() = 0;
        virtual void SetData(const void* data, BufferUsage usage) = 0;
        virtual void Update(const void* data) = 0;
        virtual void SetDataSize(uint32_t size) = 0;
        virtual uint32_t GetId() const noexcept = 0;
        virtual size_t GetSize() const noexcept = 0;
        virtual std::unique_ptr<IBuffer> Clone() const = 0;
    };

    class Buffer
    {
    public:
        explicit Buffer(BufferType type) : m_type{type}, m_buffer{CreateBufferFactory(type)} { CreateBuffer(); }
        Buffer(const Buffer& other) : m_type{other.m_type}, m_buffer{other.m_buffer->Clone()} { if(other.m_buffer) CreateBuffer(); }
        Buffer& operator=(const Buffer& other)
        {
            if (this != &other)
            {
                m_type = other.m_type;
                m_buffer = other.m_buffer->Clone();
                CreateBuffer();
            }

            return *this;
        }

        virtual ~Buffer() = default;

        template <typename T>
        void SetData(const std::vector<T>& data, BufferUsage usage, const Scope<IVertexLayout>& l) noexcept
        {
            CORE_ASSERT(l != nullptr, "Layout is null");
            l->Bind();
            m_buffer->SetDataSize(data.size() * sizeof(T));
            m_buffer->SetData(data.data(), usage);
            l->Unbind();
        }

        void BindBuffer() const noexcept;
        void UnbindBuffer() const noexcept;

        template <typename T>
        void Update(const std::vector<T>& data) noexcept { m_buffer->Update( data.data()); }

        [[nodiscard]] size_t GetSize() const noexcept { return m_buffer->GetSize(); }
        [[nodiscard]] uint32_t GetId() const noexcept { return m_buffer->GetId(); }

    private:
        BufferType m_type;
        Scope<IBuffer> m_buffer;

        static Scope<IBuffer> CreateBufferFactory(BufferType type);
        void CreateBuffer() const noexcept { m_buffer->CreateBuffer(m_type); }
    };
}