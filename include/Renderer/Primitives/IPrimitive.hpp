/*
 * Project: TestProject
 * File: IPrimitive.hpp
 * Author: olegfresi
 * Created: 08/05/25 14:51
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
#include "../Shader/Shader.hpp"
#include "../Mesh/Mesh.hpp"

namespace lux
{
    class IPrimitive
    {
    public:
        virtual ~IPrimitive() = default;

        virtual void Draw(const Matrix4f& view, const Matrix4f& projection) = 0;
        virtual void AddTransform(const Matrix4f& transform) = 0;
        virtual const Shader& GetShader() const = 0;
        virtual void SetShader(NonOwnPtr<Shader> shader) = 0;
        virtual void SetTexture(NonOwnPtr<Texture2D> texture) = 0;
    };
}