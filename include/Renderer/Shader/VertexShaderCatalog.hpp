/*
 * Project: TestProject
 * File: VertexProgramCatalog.hpp
 * Author: olegfresi
 * Created: 04/04/25 19:09
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
#include "../../Application/Pointers.hpp"
#include "VertexShader.hpp"

namespace lux
{
    class VertexShaderCatalog
    {
    public:
        VertexShaderCatalog(const std::string& name) : m_name{name} {}
        ~VertexShaderCatalog() = default;

        void SetInformation(const std::string& rendererType, char commentChar) noexcept;
        void Insert(NonOwnPtr<VertexShader> shader);
        void Remove(NonOwnPtr<VertexShader> shader);
        void PrintContents(std::filesystem::path path) const noexcept;

        NonOwnPtr<VertexShader> Find(const std::string& shaderName) const noexcept;

        const std::string& GetName () const noexcept { return m_name; }
        static void SetActive(NonOwnPtr<VertexShaderCatalog> catalog) noexcept { ms_active = catalog; }
        static NonOwnPtr<VertexShaderCatalog> GetActive() noexcept { return ms_active; }

    private:
        std::string m_name;
        std::map<std::string, NonOwnPtr<VertexShader>> m_entry;
        std::string m_rendererType;
        char m_commentChar;

        static const std::string ms_nullString;
        static NonOwnPtr<VertexShaderCatalog> ms_active;
    };
}