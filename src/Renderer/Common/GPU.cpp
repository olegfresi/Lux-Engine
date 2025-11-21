#include "../../../include/Renderer/Common/GPU.hpp"
#include "../../../include/OpenGL/OpenglError.hpp"
#include "../../../include/Application/EngineSettings.hpp"

namespace lux
{
    uint32_t GPUTexture::CreateTexture()
    {
        uint32_t id;
        GLCheck(glGenTextures(1, &id));

        return id;
    }

    void GPUTexture::CreateTexture(uint32_t *id)
    {
        GLCheck(glGenTextures(1, id));
    }

    void GPUTexture::DeleteTexture(uint32_t *textureId)
    {
        ReleaseUnit(*textureId);
        GLCheck(glDeleteTextures(1, textureId));
    }

    void GPUTexture::DeleteTextures(std::vector<uint32_t>& textures)
    {
        for (auto textureId : textures)
        {
            ReleaseUnit(textureId);
            GLCheck(glDeleteTextures(1, &textureId));
        }
    }

    void GPUTexture::Bind(uint32_t textureId, TextureType type)
    {
        if(!AcquireUnit(textureId))
        {
            CORE_ERROR("Failed to acquire texture unit for texture ID {}", textureId);
            return;
        }

        assert(textureId != 0 && "Texture not valid!");
        GLCheck(glActiveTexture(GL_TEXTURE0 + m_textureToUnit[textureId]));
        GLCheck(glBindTexture( static_cast<GLenum>(type), textureId ));
    }


    void GPUTexture::Bind(uint32_t textureId, uint32_t unit, TextureType type)
    {
        assert(textureId != 0 && "Texture not valid!");
        GLCheck(glActiveTexture(GL_TEXTURE0 + unit));
        GLCheck(glBindTexture(static_cast<GLenum>(type), textureId));
    }


    void GPUTexture::Unbind(TextureType type)
    {
        GLCheck(glBindTexture(static_cast<GLenum>(type), 0));
    }

    void GPUTexture::Load(uint32_t id, uint32_t index, uint32_t width, uint32_t height, uint32_t channels, GPUTexturePixelFormat format, TextureType type, const unsigned char* data)
    {
        GLenum glFormat = (channels == 4) ? GL_RGBA : GL_RGB;
        if(!AcquireUnit(id))
        {
            CORE_ERROR("Failed to acquire texture unit for texture ID {}", id);
            return;
        }

        if (type == TextureType::TextureCubeMap)
        {

            if(width != height)
                CORE_ERROR("For cubemap with must be equal to height");

            GLCheck(glBindTexture(GL_TEXTURE_CUBE_MAP, id));
            GLCheck(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, static_cast<GLenum >(format),
                                 width, height, 0,glFormat, GL_UNSIGNED_BYTE, data));
        }
        else
        {
            GLCheck(glBindTexture(GL_TEXTURE_2D, id));
            GLCheck(glTexImage2D(static_cast<GLenum>(type), 0,static_cast<GLint>(format),
                                 width, height, 0,glFormat, GL_UNSIGNED_BYTE, data));
        }
    }

    void GPUTexture::GenTextureImage(TextureType target, uint32_t samples, GPUTextureFormat internalFormat, uint32_t width,
                                     uint32_t height, GPUPrimitiveDataType type, GPUTextureFormat format)
    {
        if(target == TextureType::Texture2DMultisample)
        {
            GLCheck(glTexImage2DMultisample(static_cast<GLenum>(target), samples,static_cast<GLenum>(internalFormat),
                                            width, height, GL_TRUE));
        }
        else
        {
            GLCheck(glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(internalFormat),
                                 width, height, 0, static_cast<GLenum>(format), static_cast<GLenum>(type), nullptr));
        }
    }

    void GPUTexture::GenerateMipmaps(uint32_t textureId, TextureType type)
    {
        GLCheck(glBindTexture(static_cast<GLenum>(type), textureId));
        GLCheck(glGenerateMipmap(static_cast<GLenum>(type)));
    }

    void GPUTexture::SetWrap(TextureFilter wrapS, TextureFilter wrapT)
    {
        GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(wrapS)));
        GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLenum>(wrapT)));
    }

    void GPUTexture::SetTextureData(uint32_t textureId, uint32_t width, uint32_t height, GPUTexturePixelFormat format,
                        GPUPrimitiveDataType type, bool createMipMap,  const std::byte* data)
    {

    }

    void GPUTexture::SetTextureParameterI(uint32_t textureId, TextureType type, TextureParameter parameter, TextureWrap wrapType)
    {
        Bind(textureId, type);
        GLCheck(glTexParameteri(static_cast<GLenum>(type), static_cast<GLenum>(parameter), static_cast<GLenum>(wrapType)));
    }

    void GPUTexture::SetTextureParameterI(uint32_t textureId, TextureType type, TextureParameter parameter, TextureFilter filterType)
    {
        Bind(textureId, type);
        GLCheck(glTexParameteri(static_cast<GLenum>(type), static_cast<GLenum>(parameter), static_cast<GLenum>(filterType)));
    }

    bool GPUTexture::IsTexture(uint32_t textureId)
    {
        GLCheck(return glIsTexture(textureId));
    }

    std::optional<uint32_t> GPUTexture::AcquireUnit(uint32_t textureId)
    {
        if (m_textureToUnit.contains(textureId))
        {
            return m_textureToUnit[textureId];
        }

        for (uint32_t i = 0; i < MaxTextureUnits; ++i)
        {
            if (!m_usedUnits.test(i))
            {
                m_usedUnits.set(i);
                m_textureToUnit[textureId] = i;

                GLCheck(glActiveTexture(GL_TEXTURE0 + i));
                return i;
            }
        }

        CORE_ERROR("No available texture units for texture ID {}", textureId);
        return std::nullopt;
    }

    std::optional<uint32_t> GPUTexture::TryAcquireUnit(uint32_t requestedUnit, uint32_t textureId)
    {
        if (requestedUnit >= MaxTextureUnits)
            return std::nullopt;

        if (!m_usedUnits.test(requestedUnit))
        {
            m_usedUnits.set(requestedUnit);
            m_textureToUnit[textureId] = requestedUnit;

            glActiveTexture(GL_TEXTURE0 + requestedUnit);
            return requestedUnit;
        }

        return std::nullopt;
    }

    std::optional<uint32_t> GPUTexture::GetUnitForTexture(uint32_t textureId)
    {
        auto it = m_textureToUnit.find(textureId);
        if (it != m_textureToUnit.end())
            return it->second;

        return std::nullopt;
    }

    void GPUTexture::ReleaseUnit(uint32_t textureId)
    {
        auto it = m_textureToUnit.find(textureId);
        if (it != m_textureToUnit.end())
        {
            uint32_t unit = it->second;
            m_usedUnits.reset(unit);
            m_textureToUnit.erase(it);
        }
    }

    void GPUTexture::Reset()
    {
        m_usedUnits.reset();
        m_textureToUnit.clear();
    }
}