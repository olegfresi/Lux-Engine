#include "../../../include/Renderer/Texture/CubeMap.hpp"
#include "../../../include/Renderer/Common/GPU.hpp"
#include "../../../include/OpenGL/OpenglError.hpp"

namespace lux
{
    CubeMap::CubeMap(const std::vector<std::filesystem::path>& paths, std::string samplerName) : m_filePaths(paths)
    {
        Texture2D texture { TextureSpecification
        {
            .width = 0,
            .height = 0,
            .channels = 0,
            .mipMap = false,
            .type = TextureType::TextureCubeMap,
            .format = GPUTexturePixelFormat::RGB,
            .minFilter = TextureFilter::Linear,
            .magFilter = TextureFilter::Linear,
            .mipmapMinFilter = TextureFilter::LinearMipmapLinear,
            .mipmapMagFilter = TextureFilter::Linear,
            .wrapR = TextureWrap::ClampToEdge,
            .wrapS = TextureWrap::ClampToEdge,
            .wrapT = TextureWrap::ClampToEdge,
            .samplerName = samplerName,
            .filePath = paths
        }};

        m_texture = std::move(texture);
    }


    void CubeMap::DrawCubeMap() const noexcept
    {
        m_texture.Draw(m_texture.GetTextureUnit());
    }
}

