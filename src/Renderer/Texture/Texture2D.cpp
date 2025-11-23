#include "../../include/Utils/ImageLoad.hpp"
#include "../../include/Renderer/Texture/Texture2D.hpp"
#include "../../include/OpenGL/OpenglError.hpp"

namespace lux
{
    Texture2D::Texture2D(const TextureSpecification& specs) : m_specs{specs}
    {
        m_texId = GPUTexture::CreateTexture();
        m_unit = GPUTexture::AcquireUnit(m_texId).value_or(0);

        for(unsigned int i = 0; i < m_specs.filePath.size(); i++)
        {
            // TODO: change this raw pointer usage
            unsigned char* data = LoadImageFromPath(m_specs.filePath[i], &m_specs.width, &m_specs.height, &m_specs.channels, 0);

            GPUTexture::Load(m_texId, i, m_specs.width, m_specs.height, m_specs.channels, m_specs.format, m_specs.type, data);
            FreeImage(data);
        }

        if (m_specs.mipMap)
        {
            GenerateMipmaps();
            GPUTexture::SetTextureParameterI( m_texId, m_specs.type, TextureParameter::TextureMinFilter,
                                              m_specs.mipmapMinFilter );
            GPUTexture::SetTextureParameterI( m_texId, m_specs.type, TextureParameter::TextureMagFilter,
                                              m_specs.mipmapMagFilter );
        }
        else
        {
            GPUTexture::SetTextureParameterI( m_texId, m_specs.type, TextureParameter::TextureMinFilter,
                                              m_specs.minFilter );
            GPUTexture::SetTextureParameterI( m_texId, m_specs.type, TextureParameter::TextureMagFilter,
                                              m_specs.magFilter );
        }

        GPUTexture::SetTextureParameterI( m_texId, m_specs.type, TextureParameter::TextureWrapR, m_specs.wrapR );
        GPUTexture::SetTextureParameterI( m_texId, m_specs.type, TextureParameter::TextureWrapS, m_specs.wrapS );
        GPUTexture::SetTextureParameterI( m_texId, m_specs.type, TextureParameter::TextureWrapT, m_specs.wrapT );
    }

    Texture2D::~Texture2D()
    {
        if(m_texId != 0)
            GPUTexture::DeleteTexture(&m_texId);
        GPUTexture::ReleaseUnit(m_texId);
    }

    Texture2D::Texture2D(Texture2D&& other) noexcept
    {
        m_texId = other.m_texId;
        m_unit = other.m_unit;
        m_specs = std::move(other.m_specs);
        GPUTexture::ReleaseUnit(other.m_texId);
        other.m_texId = 0;
    }

    Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
    {
        if (this != &other)
        {
            m_texId = other.m_texId;
            m_unit = other.m_unit;
            m_specs = std::move(other.m_specs);
            other.m_texId = 0;
        }

        return *this;
    }

    void Texture2D::Bind() const noexcept
    {
        GPUTexture::Bind(m_texId, m_specs.type);
    }

    void Texture2D::Bind(uint32_t slot) const noexcept
    {
        GPUTexture::Bind(m_texId, slot, m_specs.type);
    }

    void Texture2D::Unbind() const noexcept
    {
        GPUTexture::Unbind(m_specs.type);
    }

    void Texture2D::Draw(uint32_t slot) const noexcept
    {
        Bind(slot);
    }

    void Texture2D::GenerateMipmaps() const
    {
        GPUTexture::GenerateMipmaps(m_texId, m_specs.type);
    }

    uint32_t Texture2D::GetTextureUnit() const noexcept
    {
        return m_unit;
    }
}