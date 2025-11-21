/*
 * Project: TestProject
 * File: OpenGLRendererAPI.hpp
 * Author: olegfresi
 * Created: 13/02/25 21:22
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
#include "../Renderer/Common/RendererApi.hpp"

namespace lux
{
    class OpenGLRendererAPI : public RendererAPI
    {
    public:

        void Init() override;
        void ShutDown() override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void SetClearColor(const Color &color) override;

        void BeginFrame() override;
        void EndFrame() override;

        void Clear(const Color& color) override;
        void ClearColor(const Color& color) override;
        void ClearDepth() override;
        void ClearStencil() override;

        void EnableBlending(bool mode, GPUBlendFactor source = GPUBlendFactor::ONE, GPUBlendFactor dest = GPUBlendFactor::ZERO) override;
        void EnableDepthTest(bool mode, DepthTestFunction function = DepthTestFunction::LESS) override;
        void EnableDepthWrite(bool enabled) override;
        void EnableStencilTest(bool mode) override;
        void EnableCullFacing(bool mode, GPUCullFaceType type = GPUCullFaceType::BACK, GPUCullFaceOrder order = GPUCullFaceOrder::COUNTER_CLOCKWISE) override;
        void EnableMultiSampling(bool mode) override;

        void SetWireframeMode(bool mode) override;
    };
}