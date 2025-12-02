/*
 * Project: TestProject
 * File: MeshRenderer.hpp
 * Author: olegfresi
 * Created: 27/02/25 17:16
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
#include "OpenglError.hpp"
#include "GLFW/glfw3.h"
#include "../Renderer/Common/GPU.hpp"
#include "OpenGLBuffer.hpp"
#include <iostream>

namespace lux
{
    struct MeshRenderer
    {
        static void Draw(GPUDrawPrimitive primitive, GPUPrimitiveDataType type, uint32_t size, const Scope<IVertexLayout>& l)
        {
            switch(API)
            {
                case GraphicsAPI::OPENGL:
                    l->Bind();
                    GLCheck(glDrawElements(static_cast<GLenum >(primitive), size, static_cast<GLenum>(type), nullptr));
                    l->Unbind();
                    break;
            }
        }

        static void DrawInstanced(GPUDrawPrimitive primitive, GPUPrimitiveDataType type, uint32_t size, const Scope<IVertexLayout>& l, uint32_t instances)
        {
            switch(API)
            {
                case GraphicsAPI::OPENGL:
                    assert(instances > 0);
                    l->Bind();
                    GLCheck(glDrawElementsInstanced(static_cast<GLenum >(primitive), size, static_cast<GLenum>(type), nullptr, instances));
                    l->Unbind();
                    break;
            }
        }
    };
}