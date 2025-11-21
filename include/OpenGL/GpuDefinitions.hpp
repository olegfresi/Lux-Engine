/*
 * Project: TestProject
 * File: GpuDefinitions.hpp
 * Author: olegfresi
 * Created: 07/04/25 11:48
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
#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"
#include <cstdint>

namespace lux
{
    inline static constexpr int SLOTS = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;

    enum class GPUFrameBufferAttachmentType : uint32_t
    {
        NONE = GL_NONE,
        FRONT_LEFT = GL_FRONT_LEFT,
        FRONT_RIGHT = GL_FRONT_RIGHT,
        BACK_LEFT = GL_BACK_LEFT,
        BACK_RIGHT = GL_BACK_RIGHT,
        COLOR0 = GL_COLOR_ATTACHMENT0,
        COLOR1 = GL_COLOR_ATTACHMENT1,
        COLOR2 = GL_COLOR_ATTACHMENT2,
        COLOR3 = GL_COLOR_ATTACHMENT3,
        COLOR4 = GL_COLOR_ATTACHMENT4,
        COLOR5 = GL_COLOR_ATTACHMENT5,
        COLOR6 = GL_COLOR_ATTACHMENT6,
        COLOR7 = GL_COLOR_ATTACHMENT7,
        COLOR8 = GL_COLOR_ATTACHMENT8,
        COLOR9 = GL_COLOR_ATTACHMENT9,
        COLOR10 = GL_COLOR_ATTACHMENT10,
        COLOR11 = GL_COLOR_ATTACHMENT11,
        COLOR12 = GL_COLOR_ATTACHMENT12,
        COLOR13 = GL_COLOR_ATTACHMENT13,
        COLOR14 = GL_COLOR_ATTACHMENT14,
        COLOR15 = GL_COLOR_ATTACHMENT15,
        DEPTH = GL_DEPTH_ATTACHMENT,
        STENCIL = GL_STENCIL_ATTACHMENT,
        DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT
    };

    enum class GPUBufferType : uint32_t
    {
        VERTEX = GL_ARRAY_BUFFER,
        INDEX = GL_ELEMENT_ARRAY_BUFFER,
        UNIFORM = GL_UNIFORM_BUFFER,
        STORAGE = GL_SHADER_STORAGE_BUFFER
    };

    enum class GPUDrawPrimitive : uint32_t
    {
        LINES = GL_LINES,
        TRIANGLES = GL_TRIANGLES
    };

    enum class GPUPrimitiveDataType : uint32_t
    {
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        BYTE = GL_BYTE,
        UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
        SHORT = GL_SHORT,
        UNSIGNED_INT = GL_UNSIGNED_INT,
        UNSIGNED_INT_24_8 = GL_UNSIGNED_INT_24_8,
        INT = GL_INT,
        HALF_FLOAT = GL_HALF_FLOAT,
        FLOAT = GL_FLOAT,
        BOOL = GL_BOOL
    };

    enum class GPUFramebufferOperationType : uint32_t
    {
        READ = GL_READ_FRAMEBUFFER,
        DRAW = GL_DRAW_FRAMEBUFFER,
        READ_AND_DRAW = GL_FRAMEBUFFER
    };

    enum class GPUTextureFormat : uint32_t
    {
        DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
        DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
        DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
        DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,
        DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
        STENCIL_INDEX8 = GL_STENCIL_INDEX8,
        DEPTH_STENCIL = GL_DEPTH_STENCIL,
        RGBA = GL_RGBA,
        R8 = GL_R8,
        R8_SNORM = GL_R8_SNORM,
        R16 = GL_R16,
        R16_SNORM = GL_R16_SNORM,
        RG8 = GL_RG8,
        RG8_SNORM = GL_RG8_SNORM,
        RG16 = GL_RG16,
        RG16_SNORM = GL_RG16_SNORM,
        RGB4 = GL_RGB4,
        RGB8 = GL_RGB8,
        RGB8_SNORM = GL_RGB8_SNORM,
        RGB16_SNORM = GL_RGB16_SNORM,
        RGBA8 = GL_RGBA8,
        RGBA8_SNORM = GL_RGBA8_SNORM,
        RGBA16 = GL_RGBA16,
        SRGB8 = GL_SRGB8,
        R16F = GL_R16F,
        RG16F = GL_RG16F,
        RGB16F = GL_RGB16F,
        RGBA16F = GL_RGBA16F,
        R32F = GL_R32F,
        RG32F = GL_RG32F,
        RGB32F = GL_RGB32F,
        RGBA32F = GL_RGBA32F,
        R8I = GL_R8I,
        R8UI = GL_R8UI,
        R16I = GL_R16I,
        R16UI = GL_R16UI,
        R32I = GL_R32I,
        R32UI = GL_R32UI,
        RG8I = GL_RG8I,
        RG8UI = GL_RG8UI,
        RG16I = GL_RG16I,
        RG16UI = GL_RG16UI,
        RG32I = GL_RG32I,
        RG32UI = GL_RG32UI,
        RGB8I = GL_RGB8I,
        RGB8UI = GL_RGB8UI,
        RGB16I = GL_RGB16I,
        RGB16UI = GL_RGB16UI,
        RGB32I = GL_RGB32I,
        RGB32UI = GL_RGB32UI,
        RGBA8I = GL_RGBA8I,
        RGBA8UI = GL_RGBA8UI,
        RGBA16I = GL_RGBA16I,
        RGBA16UI = GL_RGBA16UI,
        RGBA32I = GL_RGBA32I,
        RGBA32UI = GL_RGBA32UI
    };

    enum class GPUTexturePixelFormat : uint32_t
    {
        RED = GL_RED,
        RED_INTEGER = GL_RED_INTEGER,
        GREEN = GL_GREEN,
        GREEN_INTEGER = GL_GREEN_INTEGER,
        BLUE = GL_BLUE,
        BLUE_INTEGER = GL_BLUE_INTEGER,
        ALPHA = GL_ALPHA,
        ALPHA_INTEGER = GL_ALPHA_INTEGER,
        RG = GL_RG,
        RG_INTEGER = GL_RG_INTEGER,
        RGB = GL_RGB,
        RGB_INTEGER = GL_RGB_INTEGER,
        RGBA = GL_RGBA,
        RGBA8 = GL_RGBA8,
        RGBA_INTEGER = GL_RGBA_INTEGER,
        BGRA = GL_BGR,
        BGRA_INTEGER = GL_BGRA_INTEGER,
        DEPTH_STENCIL = GL_DEPTH_STENCIL,
        STENCIL_INDEX = GL_STENCIL_INDEX
    };

    enum class GPUPipelineStage : uint32_t
    {
        NONE,
        VERTEX,
        FRAGMENT,
        GEOMETRY,
        TESS_CONTROL,
        TESS_EVALUATION,
        COMPUTE
    };

    enum class GPUStencilFunction : uint32_t
    {
        NEVER = GL_NEVER,
        ALWAYS = GL_ALWAYS,
        LESS = GL_LESS,
        LEQUAL = GL_LEQUAL,
        GREATER = GL_GREATER,
        GEQUAL = GL_GEQUAL,
        EQUAL = GL_EQUAL,
        NOTEQUAL = GL_NOTEQUAL
    };

    enum class GPUStencilOp : uint32_t
    {
        KEEP = GL_KEEP,
        INVERT = GL_INVERT,
        ZERO = GL_ZERO,
        REPLACE = GL_REPLACE,
        INCR = GL_INCR,
        INCR_WRAP = GL_INCR_WRAP,
        DECR = GL_DECR,
        DECR_WRAP = GL_DECR_WRAP
    };

    enum class GPUBlendFactor : uint32_t
    {
        ZERO = GL_ZERO,
        ONE = GL_ONE,
        SRC_COLOR = GL_SRC_COLOR,
        ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
        DST_COLOR = GL_DST_COLOR,
        ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
        SRC_ALPHA = GL_SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
        DST_ALPHA = GL_DST_ALPHA,
        ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
        CONSTANT_COLOR = GL_CONSTANT_COLOR,
        ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
        CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
        ONE_MINUS_CONSTANT_ALPH = GL_ONE_MINUS_CONSTANT_ALPHA
    };

    enum class GPUDepthFunc : uint32_t
    {
        NEVER = GL_NEVER,
        LESS = GL_LESS,
        EQUAL = GL_EQUAL,
        LEQUAL = GL_LEQUAL,
        GREATER = GL_GREATER,
        NOTEQUAL = GL_NOTEQUAL,
        GEQUAL = GL_GEQUAL,
        ALWAYS = GL_ALWAYS
    };

    enum class GPUFlags : uint32_t
    {
        MULTISAMPLE = GL_MULTISAMPLE,
        DEPTH_TEST = GL_DEPTH_TEST,
        BLEND = GL_BLEND,
        CULL_FACE = GL_CULL_FACE
    };

    enum class GPUCullFaceType : uint32_t
    {
        FRONT = GL_FRONT,
        BACK = GL_BACK,
        FRONT_AND_BACK = GL_FRONT_AND_BACK
    };

    enum class GPUCullFaceOrder : uint32_t
    {
        CLOCKWISE = GL_CW,
        COUNTER_CLOCKWISE = GL_CCW
    };

    enum class GPUDrawMode : uint32_t
    {
        LINE = GL_LINE,
        POINT = GL_POINT,
        FILL = GL_FILL
    };

    enum class BufferUsage : uint32_t
    {
        StreamDraw = GL_STREAM_DRAW,
        StreamRead = GL_STREAM_READ,
        StreamCopy = GL_STREAM_COPY,
        StaticDraw = GL_STATIC_DRAW,
        StaticRead = GL_STATIC_READ,
        StaticCopy = GL_STATIC_COPY,
        DynamicDraw = GL_DYNAMIC_DRAW,
        DynamicRead = GL_DYNAMIC_READ,
        DynamicCopy = GL_DYNAMIC_COPY
    };

    enum class BufferType : uint32_t
    {
        VertexBuffer = GL_ARRAY_BUFFER,
        IndexBuffer = GL_ELEMENT_ARRAY_BUFFER,
        UniformBuffer = GL_UNIFORM_BUFFER,
        ShaderStorageBuffer = GL_SHADER_STORAGE_BUFFER,
        TextureBuffer = GL_TEXTURE_BUFFER
    };

    enum class TextureFilter : uint32_t
    {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
        NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
        LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
        NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
        LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
    };

    enum class TextureWrap : uint32_t
    {
        Repeat = GL_REPEAT,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        ClampToEdge = GL_CLAMP_TO_EDGE,
        ClampToBorder = GL_CLAMP_TO_BORDER,
    };

    enum class TextureParameter : uint32_t
    {
        TextureMinFilter = GL_TEXTURE_MIN_FILTER,
        TextureMagFilter = GL_TEXTURE_MAG_FILTER,
        TextureWrapR = GL_TEXTURE_WRAP_R,
        TextureWrapS = GL_TEXTURE_WRAP_S,
        TextureWrapT = GL_TEXTURE_WRAP_T,
    };

    enum class TextureType : uint32_t
    {
        Texture1D = GL_TEXTURE_1D,
        Texture2D = GL_TEXTURE_2D,
        Texture3D = GL_TEXTURE_3D,
        Texture1DArray = GL_TEXTURE_1D_ARRAY,
        Texture2DArray = GL_TEXTURE_2D_ARRAY,
        TextureRectangle = GL_TEXTURE_RECTANGLE,
        TextureCubeMap = GL_TEXTURE_CUBE_MAP,
        TextureCubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
        Texture2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
        Texture2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
        PositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        NegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        PositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        NegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        PositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        NegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    enum class DepthTestFunction : uint32_t
    {
        NEVER = GL_NEVER,
        LESS = GL_LESS,
        EQUAL = GL_EQUAL,
        LESS_OR_EQUAL = GL_LEQUAL,
        GREATER = GL_GREATER,
        NOT_EQUAL = GL_NOTEQUAL,
        GREATER_OR_EQUAL = GL_GEQUAL,
        ALWAYS = GL_ALWAYS
    };

    enum class FrameBufferMode : uint32_t
    {
        NONE = GL_NONE,
        FRONT_LEFT = GL_FRONT_LEFT,
        FRONT_RIGHT = GL_FRONT_RIGHT,
        BACK_LEFT = GL_BACK_LEFT,
        BACK_RIGHT = GL_BACK_RIGHT,
        FRONT = GL_FRONT,
        BACK = GL_BACK,
        LEFT = GL_LEFT,
        RIGHT = GL_RIGHT,
        FRONT_AND_BACK = GL_FRONT_AND_BACK,
    };


    static constexpr int TEXTURE_UNITS = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
    static constexpr int TEXTURE_BASE_LEVEL =  GL_TEXTURE_BASE_LEVEL;
    static constexpr int TEXTURE_MAX_LEVEL =  GL_TEXTURE_MAX_LEVEL;
}