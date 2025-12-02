/*
 * Project: TestProject
 * File: FrameBuffer.hpp
 * Author: olegfresi
 * Created: 08/04/25 18:27
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
#include "Buffer.hpp"
#include "../../Math/Color.hpp"
#include "../../Application/Window.hpp"
#include <vector>

namespace lux
{
    class RenderBuffer
    {
    public:
        enum class RenderBufferType
        {
            COLOR,
            DEPTH,
            STENCIL,
            DEPTH_STENCIL
        };

        RenderBuffer(RenderBufferType type) : m_type(type) {}

    private:
        RenderBufferType m_type;
    };


    enum class FrameBufferTextureFormat : uint32_t
    {
        NONE = 0,
        RGBA8,
        DEPTH24_STENCIL8,
        DEPTH = DEPTH24_STENCIL8,
        COLOR = RGBA8
    };

    struct FrameBufferTextureSpecification
    {
        FrameBufferTextureSpecification() = default;
        FrameBufferTextureSpecification(FrameBufferTextureFormat format) : format{format} {}

        FrameBufferTextureFormat format = FrameBufferTextureFormat::NONE;

        // TODO: filtering/wrap
    };

    struct FrameBufferSpecification
    {
        uint32_t width;
        uint32_t height;
        uint32_t samples = 1;
        std::vector<FrameBufferTextureSpecification> m_attachmentsSpecifications;
    };

    class IFrameBuffer
    {
    public:
        virtual ~IFrameBuffer() = default;
        virtual void GenBuffer(uint32_t *id) const = 0;
        virtual void BindFrameBuffer(uint32_t id) const = 0;
        virtual void BindFrameBuffer(uint32_t id, uint32_t width, uint32_t height) const = 0;
        virtual void UnbindFrameBuffer() const = 0;
        virtual void DeleteFrameBuffer(uint32_t id) const = 0;

        virtual void CreateFrameBufferColorTexture(uint32_t id, int index, bool multisample) const noexcept = 0;
        virtual void CreateFrameBufferDepthTexture(uint32_t id) const noexcept = 0;
        virtual void RestoreDefaultFramebuffer(Window* window, int& width, int& height) const noexcept = 0;

        virtual bool CheckFrameBufferStatus() const = 0;
        virtual Color ReadFrameBuffer(uint32_t x, uint32_t y, GPUTexturePixelFormat pixelFormat, GPUPrimitiveDataType dataType) const noexcept = 0;

        virtual void ReadFrameBuffer(FrameBufferMode mode) const noexcept = 0;
        virtual void WriteFrameBuffer(FrameBufferMode mode) const noexcept = 0;
        virtual void DrawBuffers(size_t size) const noexcept = 0;
    };

    class FrameBuffer
    {
    public:

        explicit FrameBuffer(const FrameBufferSpecification& specs) noexcept;
        ~FrameBuffer() noexcept;

        void Invalidate();
        void Resize(uint32_t width, uint32_t height);
        void Bind() const;
        void Unbind() const;
        bool CheckFrameBufferStatus() const;
        void RestoreDefaultFramebuffer(NonOwnPtr<Window> window, int& width, int& height) const;

        void CreateColorAttachment();
        void CreateDepthStencilAttachment();
        void AttachColorTexture(const NonOwnPtr<uint32_t>, int samples, GPUTextureFormat format, uint32_t width, uint32_t height, int index) const;
        void AttachDepthTexture(const NonOwnPtr<uint32_t> id, GPUTextureFormat format, GPUFrameBufferAttachmentType type, uint32_t width, uint32_t height);
        void BindDepthTexture(uint32_t unit) const { GPUTexture::Bind(m_depthAttachment, unit, TextureType::Texture2D); }

        uint32_t GetId() const noexcept     { return m_id; }
        uint32_t GetWidth() const noexcept  { return m_specs.width; }
        uint32_t GetHeight() const noexcept { return m_specs.height; }
        uint32_t GetDepthTexture() const noexcept { return m_depthAttachment; }

        float Ratio() const noexcept { return static_cast<float>(m_specs.width) / static_cast<float>(m_specs.height); }

        FrameBufferSpecification GetSpecifications() const noexcept { return m_specs; }

    private:
        uint32_t m_id = 0;
        uint32_t m_depthAttachment;
        FrameBufferSpecification m_specs;

        std::vector<FrameBufferTextureFormat> m_colorAttachmentsFormats;
        FrameBufferTextureFormat m_depthAttachmentFormat = FrameBufferTextureFormat::NONE;

        std::vector<uint32_t> m_colorAttachments;

        Scope<IFrameBuffer> m_frameBuffer;
        static Scope<IFrameBuffer> CreateFrameBuffer(const FrameBufferSpecification& specs) noexcept;
    };
}