/*
 * Project: TestProject
 * File: RenderCommand.hpp
 * Author: olegfresi
 * Created: 18/02/25 16:09
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
#include "RendererApi.hpp"

namespace lux
{
    class VertexArray;

    class RenderCommand
    {
    public:
        static void Init()
        {
            INFO("RenderCommand initialized");
            s_RendererAPI->Init();
        }

        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            s_RendererAPI->SetViewport(x, y, width, height);
        }

        static void SetClearColor(const Color& color)
        {
            s_RendererAPI->SetClearColor(color);
        }

        static void Clear(Color color = Color(0.1f, 0.1f, 0.1f, 1.0f))
        {
            s_RendererAPI->ClearColor(color);
            s_RendererAPI->Clear(color);
        }

        static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
        {
            //s_RendererAPI->DrawIndexed(vertexArray, indexCount);
        }

        static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
        {
            //s_RendererAPI->DrawLines(vertexArray, vertexCount);
        }

        static void SetLineWidth(float width)
        {
            //s_RendererAPI->SetLineWidth(width);
        }

        static void EnableDepthTest(bool mode, DepthTestFunction func = DepthTestFunction::LESS)
        {
            s_RendererAPI->EnableDepthTest(mode, func);
        }

        static void EnableDepthWrite(bool enabled)
        {
            s_RendererAPI->EnableDepthWrite(enabled);
        }

        static void EnableBlending(bool mode, GPUBlendFactor source, GPUBlendFactor dest)
        {
            s_RendererAPI->EnableBlending(mode, source, dest);
        }

        static void EnableStencilTest(bool mode)
        {
            s_RendererAPI->EnableStencilTest(mode);
        }

        static void EnableFaceCulling(bool mode, GPUCullFaceType type = GPUCullFaceType::FRONT, GPUCullFaceOrder order = GPUCullFaceOrder::CLOCKWISE)
        {
            s_RendererAPI->EnableCullFacing(mode, type, order);
        }

        static void EnableWireframe(bool mode)
        {
            s_RendererAPI->SetWireframeMode(mode);
        }

        static void EnableMultiSampling(bool mode)
        {
            s_RendererAPI->EnableMultiSampling(mode);
        }

        static void ClearColor(const Color& color)
        {
            s_RendererAPI->ClearColor(color);
        }

        static void ClearDepth()
        {
            s_RendererAPI->ClearDepth();
        }
        static void ClearStencil()
        {
            s_RendererAPI->ClearStencil();
        }

    private:
        static inline Scope<RendererAPI> s_RendererAPI = RendererAPI::Create();
    };
}