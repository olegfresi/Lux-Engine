/*
 * Project: TestProject
 * File: RenderConstant.hpp
 * Author: olegfresi
 * Created: 04/04/25 18:15
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
#include <cstdint>
#include <array>
#include <map>
#include "../../../Application/Pointers.hpp"

namespace  lux
{
    class RenderConstant
    {
    public:

        enum class Type : uint8_t
        {
            W_MATRIX,                          // 4x4 model-to-world matrix
            V_MATRIX,                          // 4x4 world-to-view matrix
            P_MATRIX,                          // 4x4 view-to-clip matrix
            WV_MATRIX,                         // 4x4 model-to-view matrix
            VP_MATRIX,                         // 4x4 world-to-clip matrix
            WVP_MATRIX,                        // 4x4 model-to-clip matrix
            W_MATRIX_TRANSPOSE,                // 4x4 trans model-to-world
            V_MATRIX_TRANSPOSE,                // 4x4 trans world-to-view
            P_MATRIX_TRANSPOSE,                // 4x4 trans view-to-clip
            WV_MATRIX_TRANSPOSE,               // 4x4 trans model-to-view
            VP_MATRIX_TRANSPOSE,               // 4x4 trans world-to-clip
            WVP_MATRIX_TRANSPOSE,              // 4x4 trans model-to-clip
            W_MATRIX_INVERSE,                  // 4x4 inv model-to-world
            V_MATRIX_INVERSE,                  // 4x4 inv world-to-view
            P_MATRIX_INVERSE,                  // 4x4 inv view-to-clip
            WV_MATRIX_INVERSE,                 // 4x4 inv model-to-view
            VP_MATRIX_INVERSE,                 // 4x4 inv world-to-clip
            WVP_MATRIX_INVERSE,                // 4x4 inv model-to-clip
            W_MATRIX_INVERSE_TRANSPOSE,        // 4x4 inv-trans model-to-world
            V_MATRIX_INVERSE_TRANSPOSE,        // 4x4 inv-trans world-to-view
            P_MATRIX_INVERSE_TRANSPOSE,        // 4x4 inv-trans view-to-clip
            WV_MATRIX_INVERSE_TRANSPOSE,       // 4x4 inv-trans model-to-view
            VP_MATRIX_INVERSE_TRANSPOSE,       // 4x4 inv-trans world-to-clip
            WVP_MATRIX_INVERSE_TRANSPOSE,      // 4x4 inv-trans model-to-clip

            MATERIAL_EMISSIVE,                 // (r,g,b)
            MATERIAL_AMBIENT,                  // (r,g,b)
            MATERIAL_DIFFUSE,                  // (r,g,b;alpha)
            MATERIAL_SPECULAR,                 // (r,g,b;shininess)

            CAMERA_MODEL_POSITION,             // (x,y,z,1)
            CAMERA_MODEL_DIRECTION,            // (x,y,z,0)
            CAMERA_MODEL_UP,                   // (x,y,z,0)
            CAMERA_MODEL_RIGHT,                // (x,y,z,0)

            CAMERA_WORLD_POSITION,             // (x,y,z,1)
            CAMERA_WORLD_DIRECTION,            // (x,y,z,0)
            CAMERA_WORLD_UP,                   // (x,y,z,0)
            CAMERA_WORLD_RIGHT,                // (x,y,z,0)

            PROJECTOR_MODEL_POSITION,          // (x,y,z,1)
            PROJECTOR_MODEL_DIRECTION,         // (x,y,z,0)
            PROJECTOR_MODEL_UP,                // (x,y,z,0)
            PROJECTOR_MODEL_RIGHT,             // (x,y,z,0)

            PROJECTOR_WORLD_POSITION,          // (x,y,z,1)
            PROJECTOR_WORLD_DIRECTION,         // (x,y,z,0)
            PROJECTOR_WORLD_UP,                // (x,y,z,0)
            PROJECTOR_WORLD_RIGHT,             // (x,y,z,0)

            PROJECTOR_MATRIX,                  // 4x4 world-to-clip matrix

            LIGHT0_MODEL_POSITION,             // (x,y,z,1)
            LIGHT0_MODEL_DIRECTION,            // (x,y,z,0)
            LIGHT0_WORLD_POSITION,             // (x,y,z,1)
            LIGHT0_WORLD_DIRECTION,            // (x,y,z,0)
            LIGHT0_AMBIENT,                    // (r,g,b,a)
            LIGHT0_DIFFUSE,                    // (r,g,b,a)
            LIGHT0_SPECULAR,                   // (r,g,b,a)
            LIGHT0_SPOTCUTOFF,                 // (angle,cos,sin,exponent)
            LIGHT0_ATTENUATION,                // (const,lin,quad,intensity)

            LIGHT1_MODEL_POSITION,             // (x,y,z,1)
            LIGHT1_MODEL_DIRECTION,            // (x,y,z,0)
            LIGHT1_WORLD_POSITION,             // (x,y,z,1)
            LIGHT1_WORLD_DIRECTION,            // (x,y,z,0)
            LIGHT1_AMBIENT,                    // (r,g,b,a)
            LIGHT1_DIFFUSE,                    // (r,g,b,a)
            LIGHT1_SPECULAR,                   // (r,g,b,a)
            LIGHT1_SPOTCUTOFF,                 // (angle,cos,sin,exponent)
            LIGHT1_ATTENUATION,                // (const,lin,quad,intensity)

            LIGHT2_MODEL_POSITION,             // (x,y,z,1)
            LIGHT2_MODEL_DIRECTION,            // (x,y,z,0)
            LIGHT2_WORLD_POSITION,             // (x,y,z,1)
            LIGHT2_WORLD_DIRECTION,            // (x,y,z,0)
            LIGHT2_AMBIENT,                    // (r,g,b,a)
            LIGHT2_DIFFUSE,                    // (r,g,b,a)
            LIGHT2_SPECULAR,                   // (r,g,b,a)
            LIGHT2_SPOTCUTOFF,                 // (angle,cos,sin,exponent)
            LIGHT2_ATTENUATION,                // (const,lin,quad,intensity)

            LIGHT3_MODEL_POSITION,             // (x,y,z,1)
            LIGHT3_MODEL_DIRECTION,            // (x,y,z,0)
            LIGHT3_WORLD_POSITION,             // (x,y,z,1)
            LIGHT3_WORLD_DIRECTION,            // (x,y,z,0)
            LIGHT3_AMBIENT,                    // (r,g,b,a)
            LIGHT3_DIFFUSE,                    // (r,g,b,a)
            LIGHT3_SPECULAR,                   // (r,g,b,a)
            LIGHT3_SPOTCUTOFF,                 // (angle,cos,sin,exponent)
            LIGHT3_ATTENUATION,                // (const,lin,quad,intensity)

            LIGHT4_MODEL_POSITION,             // (x,y,z,1)
            LIGHT4_MODEL_DIRECTION,            // (x,y,z,0)
            LIGHT4_WORLD_POSITION,             // (x,y,z,1)
            LIGHT4_WORLD_DIRECTION,            // (x,y,z,0)
            LIGHT4_AMBIENT,                    // (r,g,b,a)
            LIGHT4_DIFFUSE,                    // (r,g,b,a)
            LIGHT4_SPECULAR,                   // (r,g,b,a)
            LIGHT4_SPOTCUTOFF,                 // (angle,cos,sin,exponent)
            LIGHT4_ATTENUATION,                // (const,lin,quad,intensity)

            LIGHT5_MODEL_POSITION,             // (x,y,z,1)
            LIGHT5_MODEL_DIRECTION,            // (x,y,z,0)
            LIGHT5_WORLD_POSITION,             // (x,y,z,1)
            LIGHT5_WORLD_DIRECTION,            // (x,y,z,0)
            LIGHT5_AMBIENT,                    // (r,g,b,a)
            LIGHT5_DIFFUSE,                    // (r,g,b,a)
            LIGHT5_SPECULAR,                   // (r,g,b,a)
            LIGHT5_SPOTCUTOFF,                 // (angle,cos,sin,exponent)
            LIGHT5_ATTENUATION,                // (const,lin,quad,intensity)

            LIGHT6_MODEL_POSITION,             // (x,y,z,1)
            LIGHT6_MODEL_DIRECTION,            // (x,y,z,0)
            LIGHT6_WORLD_POSITION,             // (x,y,z,1)
            LIGHT6_WORLD_DIRECTION,            // (x,y,z,0)
            LIGHT6_AMBIENT,                    // (r,g,b,a)
            LIGHT6_DIFFUSE,                    // (r,g,b,a)
            LIGHT6_SPECULAR,                   // (r,g,b,a)
            LIGHT6_SPOTCUTOFF,                 // (angle,cos,sin,exponent)
            LIGHT6_ATTENUATION,                // (const,lin,quad,intensity)

            LIGHT7_MODEL_POSITION,             // (x,y,z,1)
            LIGHT7_MODEL_DIRECTION,            // (x,y,z,0)
            LIGHT7_WORLD_POSITION,             // (x,y,z,1)
            LIGHT7_WORLD_DIRECTION,            // (x,y,z,0)
            LIGHT7_AMBIENT,                    // (r,g,b,a)
            LIGHT7_DIFFUSE,                    // (r,g,b,a)
            LIGHT7_SPECULAR,                   // (r,g,b,a)
            LIGHT7_SPOTCUTOFF,                 // (angle,cos,sin,exponent)
            LIGHT7_ATTENUATION,                // (const,lin,quad,intensity)

            MAX_TYPES
        };

        RenderConstant (Type type, int baseRegister, int registerQuantity);
        ~RenderConstant ();

        Type GetType () const;
        int GetBaseRegister () const;
        int GetRegisterQuantity () const;
        std::array<float, 16> GetData () const;

        static const std::string& GetName (Type type);
        static Type GetType (const std::string& name);

    private:
        Type m_eType;
        int m_BaseRegister;
        int m_RegisterQuantity;
        std::array<float, 16> m_data;

        static std::string ms_stringMap[(int)Type::MAX_TYPES + 1];
        static NonOwnPtr<std::map<std::string, Type>> ms_typeMap;
    };
}