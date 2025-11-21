/*
 * Project: TestProject
 * File: OpenGLFrameBuffer.hpp
 * Author: olegfresi
 * Created: 01/07/25 15:53
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
#include "../Renderer/Buffer/FrameBuffer.hpp"

namespace lux
{
    class OpenGLFrameBuffer : public IFrameBuffer
    {
    public:

        OpenGLFrameBuffer(FrameBufferSpecification specs) : m_specs{specs} { INFO("OpenGLFrameBuffer created"); }
        void GenBuffer(uint32_t *id) const override;
        void BindFrameBuffer(uint32_t id) const override;
        void BindFrameBuffer(uint32_t id, uint32_t width, uint32_t height) const override;
        void UnbindFrameBuffer() const override;
        void DeleteFrameBuffer(uint32_t id) const override;

        void CreateFrameBufferColorTexture(uint32_t id, int index, bool multisample) const noexcept override;
        void CreateFrameBufferDepthTexture(uint32_t id) const noexcept override;
        void RestoreDefaultFramebuffer(Window* window, int& width, int& height) const noexcept override;

        bool CheckFrameBufferStatus() const override;
        Color ReadFrameBuffer(uint32_t x, uint32_t y, GPUTexturePixelFormat pixelFormat, GPUPrimitiveDataType dataType) const noexcept override;

        void ReadFrameBuffer(FrameBufferMode mode) const noexcept override;
        void WriteFrameBuffer(FrameBufferMode mode) const noexcept override;
        void DrawBuffers(size_t size) const noexcept override;

    private:

        FrameBufferSpecification m_specs;
    };
}