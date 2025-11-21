/*
 * Project: TestProject
 * File: GPU.hpp
 * Author: olegfresi
 * Created: 23/02/25 21:08
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
#include "../../Math/Vector.hpp"
#include "../../Math/Matrix.hpp"
#include "../../OpenGL/GpuDefinitions.hpp"
#include <string>
using namespace lux::math;

namespace lux
{
#if (API == API_OPENGL)
#include "../../OpenGL/GpuDefinitions.hpp"
#else
#error "Unsupported API"
#endif

    class GPUDataType
    {
    public:

        GPUDataType(const std::string& name, uint32_t sizeInBytes, lux::GPUPrimitiveDataType primitiveDataType) :
            name(name), typeSizeInBytes(sizeInBytes), mPrimitiveDataType(primitiveDataType) {}

        uint32_t GetPrimitiveTypeSizeInBytes() const noexcept;
        uint32_t GetTypeSizeInBytes() const noexcept { return typeSizeInBytes; }
        uint32_t GetSizePrimitiveType() const noexcept
        {
            uint32_t primitiveTypeSizeInBytes = GetPrimitiveTypeSizeInBytes();
            assert(primitiveTypeSizeInBytes != 0);
            uint32_t sizeInPrimitiveTypes = typeSizeInBytes / primitiveTypeSizeInBytes;
            return sizeInPrimitiveTypes;
        }

        std::string GetGPUDataTypeName() const noexcept { return name; }

        GPUPrimitiveDataType GetPrimitiveDataType() const noexcept { return mPrimitiveDataType; }

        template <typename T>
        static GPUPrimitiveDataType GetPrimitiveDataTypeFromType();

        template <typename T>
        static int GetSizeFromType();

    private:
        std::string name;
        uint32_t typeSizeInBytes = 0;
        GPUPrimitiveDataType mPrimitiveDataType = GPUPrimitiveDataType::FLOAT;
    };

    struct PrimitiveTypes
    {
        inline static const GPUDataType Bool { "bool", sizeof( bool ), GPUPrimitiveDataType::BOOL };
        inline static const GPUDataType Int { "int", sizeof( int ), GPUPrimitiveDataType::INT };
        inline static const GPUDataType UnsignedInt { "uint", sizeof( uint32_t ), GPUPrimitiveDataType::INT };
        inline static const GPUDataType Float { "float", sizeof( float ), GPUPrimitiveDataType::FLOAT };
        inline static const GPUDataType Vector2i { "vec2i", sizeof( int ) * 2, GPUPrimitiveDataType::INT };
        inline static const GPUDataType Vector2f { "vec2f", sizeof( float ) * 2, GPUPrimitiveDataType::FLOAT };
        inline static const GPUDataType Vector3i { "vec3i", sizeof( int ) * 3, GPUPrimitiveDataType::INT };
        inline static const GPUDataType Vector3f { "vec3f", sizeof( float ) * 3, GPUPrimitiveDataType::FLOAT };
        inline static const GPUDataType Vector4i { "vec4i", sizeof( int ) * 4, GPUPrimitiveDataType::INT };
        inline static const GPUDataType Vector4f { "vec4f", sizeof( float ) * 4, GPUPrimitiveDataType::FLOAT };
        inline static const GPUDataType Matrix2i { "mat2i", sizeof( int ) * 2 * 2, GPUPrimitiveDataType::INT };
        inline static const GPUDataType Matrix2f { "mat2f", sizeof( float ) * 2 * 2, GPUPrimitiveDataType::FLOAT };
        inline static const GPUDataType Matrix3i { "mat3i", sizeof( int ) * 3 * 3, GPUPrimitiveDataType::INT };
        inline static const GPUDataType Matrix3f { "mat3f", sizeof( float ) * 3 * 3, GPUPrimitiveDataType::FLOAT };
        inline static const GPUDataType Matrix4i { "mat4i", sizeof( int ) * 4 * 4, GPUPrimitiveDataType::INT };
        inline static const GPUDataType Matrix4f { "mat4f", sizeof( float ) * 4 * 4, GPUPrimitiveDataType::FLOAT };
        inline static const GPUDataType Sampler2D { "sampler2D", 0, GPUPrimitiveDataType::INT };
    };

    inline uint32_t GPUDataType::GetPrimitiveTypeSizeInBytes() const noexcept
    {
        uint32_t primitiveTypeSizeInBytes = 0;
        switch (mPrimitiveDataType)
        {
            case GPUPrimitiveDataType::FLOAT:
                primitiveTypeSizeInBytes = sizeof( float );
                break;

            case GPUPrimitiveDataType::INT:
                primitiveTypeSizeInBytes = sizeof( int );
                break;

            case GPUPrimitiveDataType::BOOL:
                primitiveTypeSizeInBytes = sizeof( bool );
                break;

            case GPUPrimitiveDataType::UNSIGNED_BYTE:
                primitiveTypeSizeInBytes = sizeof( std::byte );
                break;

            case GPUPrimitiveDataType::BYTE:
                primitiveTypeSizeInBytes = sizeof( std::byte );
                break;

            case GPUPrimitiveDataType::UNSIGNED_SHORT:
                primitiveTypeSizeInBytes = sizeof( ushort );
                break;

            case GPUPrimitiveDataType::SHORT:
                primitiveTypeSizeInBytes = sizeof( short );
                break;

            case GPUPrimitiveDataType::UNSIGNED_INT:
                primitiveTypeSizeInBytes = sizeof( uint32_t );
                break;

            case GPUPrimitiveDataType::HALF_FLOAT:
                primitiveTypeSizeInBytes = sizeof( float ) / 2;
                break;

            default:
                assert(false);
        }

        return primitiveTypeSizeInBytes;
    }

    template <typename T>
    GPUPrimitiveDataType GPUDataType::GetPrimitiveDataTypeFromType()
    {
        if constexpr (std::is_same_v<T, bool>)
            return PrimitiveTypes::Bool.GetPrimitiveDataType();

        else if constexpr (std::is_same_v<T, int>)
            return PrimitiveTypes::Int.GetPrimitiveDataType();

        else if constexpr (std::is_same_v<T, uint32_t>)
            return PrimitiveTypes::UnsignedInt.GetPrimitiveDataType();

        else if constexpr (std::is_same_v<T, float>)
            return PrimitiveTypes::Float.GetPrimitiveDataType();

        else if constexpr (std::is_same_v<T, Vector2f>)
            return PrimitiveTypes::Vector2f.GetPrimitiveDataType();

        else if constexpr (std::is_same_v<T, Vector2i>)
            return PrimitiveTypes::Vector2i.GetPrimitiveDataType();

        else if constexpr (std::is_same_v<T, Vector3f>)
            return PrimitiveTypes::Vector3f.GetPrimitiveDataType();

        else if constexpr (std::is_same_v<T, Vector3i>)
            return PrimitiveTypes::Vector3i.GetPrimitiveDataType();

        else if constexpr (std::is_same_v<T, Vector4f>)
            return PrimitiveTypes::Vector4f.GetPrimitiveDataType();

        else if constexpr (std::is_same_v<T, Vector4i>)
            return PrimitiveTypes::Vector4i.GetPrimitiveDataType();

        else if constexpr (std::is_same_v<T, Matrix3f>)
            return PrimitiveTypes::Matrix3f.GetPrimitiveDataType();

        else if constexpr (std::is_same_v<T, Matrix4f>)
            return PrimitiveTypes::Matrix4f.GetPrimitiveDataType();

        return PrimitiveTypes::Sampler2D.GetPrimitiveDataType();
    }

    template <typename T>
    int GPUDataType::GetSizeFromType()
    {
        constexpr size_t sizeF = sizeof (float);
        constexpr size_t sizeI = sizeof (int);

        if constexpr (std::is_same_v<T, bool>)
            return PrimitiveTypes::Bool.GetTypeSizeInBytes() ;

        else if constexpr (std::is_same_v<T, int>)
            return PrimitiveTypes::Int.GetTypeSizeInBytes() ;

        else if constexpr (std::is_same_v<T, uint32_t>)
            return PrimitiveTypes::UnsignedInt.GetTypeSizeInBytes() ;

        else if constexpr (std::is_same_v<T, float>)
            return PrimitiveTypes::Float.GetTypeSizeInBytes() ;

        else if constexpr (std::is_same_v<T, Vector2f>)
            return PrimitiveTypes::Vector2f.GetTypeSizeInBytes()  / sizeF;

        else if constexpr (std::is_same_v<T, Vector2i>)
            return PrimitiveTypes::Vector2i.GetTypeSizeInBytes()  / sizeI;

        else if constexpr (std::is_same_v<T, Vector3f>)
            return PrimitiveTypes::Vector3f.GetTypeSizeInBytes()  / sizeF;

        else if constexpr (std::is_same_v<T, Vector3i>)
            return PrimitiveTypes::Vector3i.GetTypeSizeInBytes() / sizeI;

        else if constexpr (std::is_same_v<T, Vector4f>)
            return PrimitiveTypes::Vector4f.GetTypeSizeInBytes()  / sizeF;

        else if constexpr (std::is_same_v<T, Vector4i>)
            return PrimitiveTypes::Vector4i.GetTypeSizeInBytes()  / sizeI;

        else if constexpr (std::is_same_v<T, Matrix3f>)
            return PrimitiveTypes::Matrix3f.GetTypeSizeInBytes()  / 3 * sizeF;

        else if constexpr (std::is_same_v<T, Matrix4f>)
            return PrimitiveTypes::Matrix4f.GetTypeSizeInBytes()  / 4 * sizeF;

        return PrimitiveTypes::Sampler2D.GetTypeSizeInBytes();
    }

    struct GPUTexture
    {
        static uint32_t CreateTexture();
        static void  CreateTexture(uint32_t *id);
        static void DeleteTexture(uint32_t *textureId);
        static void DeleteTextures(std::vector<uint32_t>& textures);
        static void Bind(uint32_t textureId, TextureType type);
        static void Bind(uint32_t textureId, uint32_t unit, TextureType type);
        static void Unbind(TextureType type);
        static void Load(uint32_t id, uint32_t index, uint32_t width, uint32_t height, uint32_t channels,
                         GPUTexturePixelFormat format, TextureType type, const unsigned char* data);

        static void GenTextureImage(TextureType target, uint32_t samples, GPUTextureFormat internalFormat, uint32_t width,
                                    uint32_t height, GPUPrimitiveDataType type, GPUTextureFormat format = GPUTextureFormat::RGB8);

        static void GenerateMipmaps(uint32_t textureId, TextureType type);
        static void SetWrap(TextureFilter wrapS, TextureFilter wrapT);
        static void SetTextureData(uint32_t textureId, uint32_t width, uint32_t height, GPUTexturePixelFormat format,
                            GPUPrimitiveDataType type, bool createMipMap,  const std::byte* data);

        static bool IsTexture(uint32_t textureId);

        static void SetTextureParameterI(uint32_t textureId, TextureType type, TextureParameter parameter, TextureWrap wrapType);
        static void SetTextureParameterI(uint32_t textureId, TextureType type, TextureParameter parameter, TextureFilter filterType);

        static std::optional<uint32_t> AcquireUnit(uint32_t textureId);
        static std::optional<uint32_t> TryAcquireUnit(uint32_t requestedUnit, uint32_t textureId);
        static std::optional<uint32_t> GetUnitForTexture(uint32_t textureId);
        static void ReleaseUnit(uint32_t textureId);
        static void Reset();


        static constexpr uint32_t MaxTextureUnits = 32;
        static inline std::bitset<MaxTextureUnits> m_usedUnits{};
        static inline std::unordered_map<uint32_t, uint32_t> m_textureToUnit{};
    };
}