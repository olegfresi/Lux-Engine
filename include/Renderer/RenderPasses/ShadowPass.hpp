/*
 * Project: TestProject
 * File: ShadowPass.hpp
 * Author: olegfresi
 * Created: 07/04/25 19:51
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
#include "RenderPass.hpp"
#include "../../include/Scene/Scene.hpp"

namespace lux
{
    class ShadowPass : public RenderPass
    {

    public:

        ShadowPass(Shader& depthShader, const FrameBufferSpecification& spec);
        ~ShadowPass() = default;

        void Begin(const Scene& scene, const Matrix4f& lightSpaceMatrix, const Matrix4f& model) const;
        void End(NonOwnPtr<Window> window) const;
        static void Execute() {}
        void BindDepthTexture() const noexcept;

        FrameBuffer& GetFrameBuffer() & noexcept { return m_frameBuffer; }

        uint32_t GetShadowTextureUnit() const noexcept { return m_shadowTextureUnit; }
        uint32_t GetShadowSize() const noexcept { return m_shadowSize; }

    private:
        FrameBuffer m_frameBuffer;
        uint32_t m_shadowSize;
        Shader& m_depthShader;
        uint32_t m_shadowTextureUnit;
    };
}