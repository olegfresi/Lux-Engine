/*
 * Project: TestProject
 * File: ShaderProgram.hpp
 * Author: olegfresi
 * Created: 03/04/25 16:53
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
#include "ShaderNodes/Attributes.hpp"
#include "ShaderNodes/RenderConstant.hpp"
#include "ShaderNodes/NumericalConstant.hpp"
#include "ShaderNodes/UserConstant.hpp"
#include "ShaderNodes/SamplerInfo.hpp"
#include "../../Application/Pointers.hpp"
#include <string>
#include <vector>

namespace lux
{
    class ShaderProgram
    {
    public:
        virtual ~ShaderProgram() = default;

        const std::string& GetProgramText() const;
        const Attributes& GetInputAttributes() const;
        const Attributes& GetOutputAttributes() const;

        int GetRcQuantity() const;
        NonOwnPtr<RenderConstant> GetRenderConstant(int index) const;
        NonOwnPtr<RenderConstant> GetRenderConstant(RenderConstant::Type type) const;

        int GetNcQuantity() const;
        NonOwnPtr<NumericalConstant> GetNcQuantity(int index) const;

        int GetUcQuantity() const;
        NonOwnPtr<UserConstant> GetUserConstant(int index) const;
        NonOwnPtr<UserConstant> GetUserConstant(std::string name) const;

        int GetSamplerQuantity() const;
        NonOwnPtr<SamplerInfo> GetSampler(int index) const;
        NonOwnPtr<SamplerInfo> GetSampler(std::string name) const;

    protected:
        static bool Load(const std::string& fileName, char commentChar, NonOwnPtr<ShaderProgram> program);
        ShaderProgram();

        std::string m_programText;

        Attributes m_inputAttributes;
        Attributes m_outputAttributes;

        std::vector<NonOwnPtr<RenderConstant>> m_renderConstants;
        std::vector<NonOwnPtr<NumericalConstant>> m_numericalConstants;
        std::vector<NonOwnPtr<UserConstant>> m_userConstants;
        std::vector<NonOwnPtr<SamplerInfo>> m_samplers;

        static const std::string ms_floatStr;
        static const std::string ms_float2Str;
        static const std::string ms_float3Str;
        static const std::string ms_float4Str;
        static const std::string ms_float4x4Str;
        static const std::string ms_sampler1DStr;
        static const std::string ms_sampler2DStr;
        static const std::string ms_sampler3DStr;
        static const std::string ms_samplerCubeStr;
        static const std::string ms_samplerProjStr;
        static const std::string ms_positionStr;
        static const std::string ms_normalStr;
        static const std::string ms_colorStr;
        static const std::string ms_color0Str;
        static const std::string ms_color1Str;
        static const std::string ms_texCoordStr;
        static const std::string ms_inStr;
        static const std::string ms_outStr;
        static const std::string ms_EOL;
    };
}