#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "../../include/Utils/ImageLoad.hpp"
#include "../../include/OpenGL/OpenglError.hpp"

namespace lux
{
    unsigned char* LoadImageFromPath(const std::filesystem::path& path, int* width, int* height, int* channels, int reqComp)
    {
        auto data = stbi_load( path.c_str(), width, height, channels, reqComp);
        if (!data) { CORE_ERROR("Failed to load texture: {}", path.string()); }

        return data;
    }

    void FreeImage(unsigned char* data) { stbi_image_free(data); }
}