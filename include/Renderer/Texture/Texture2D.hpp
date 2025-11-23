/*
 * Project: TestProject
 * File: Texture2D.hpp
 * Author: olegfresi
 * Created: 03/02/25 19:09
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
#include <string>
#include "Texture.hpp"
#include "../Common/GPU.hpp"

namespace lux
{
    struct TextureSpecification
    {
        int width;
        int height;
        int channels;
        bool mipMap;

        TextureType type = TextureType::Texture2D;
        GPUTexturePixelFormat format = GPUTexturePixelFormat::RGBA;

        TextureFilter minFilter = TextureFilter::LinearMipmapLinear;
        TextureFilter magFilter = TextureFilter::Linear;

        TextureFilter mipmapMinFilter = TextureFilter::Nearest;
        TextureFilter mipmapMagFilter = TextureFilter::Nearest;

        TextureWrap wrapR;
        TextureWrap wrapS;
        TextureWrap wrapT;

        std::string samplerName;

        std::vector<std::filesystem::path> filePath;
    };



    class Texture2D : public Texture
    {
    public:
        Texture2D() = default;

        explicit Texture2D(const TextureSpecification& specs);
        ~Texture2D() override;

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;

        Texture2D(Texture2D&& other) noexcept;
        Texture2D& operator=(Texture2D&& other) noexcept;

        void Bind() const noexcept override;
        void Bind(uint32_t slot) const noexcept;
        void Unbind() const noexcept override;

        void Draw(uint32_t slot) const noexcept;
        void GenerateMipmaps() const;

        int GetWidth() const noexcept override { return m_specs.width; }
        int GetHeight() const noexcept override { return m_specs.height; }

        TextureSpecification GetSpecs() const noexcept { return m_specs; }

        uint32_t GetId() const noexcept { return m_texId; }
        uint32_t GetTextureUnit() const noexcept;

    private:
        uint32_t m_texId = 0u;
        uint32_t m_unit = 0u;
        TextureSpecification m_specs;
    };
}