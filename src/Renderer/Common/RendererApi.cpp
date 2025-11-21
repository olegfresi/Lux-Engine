#include "../../../include/Renderer/Common/RendererApi.hpp"
#include "../../../include/OpenGL/OpenGLRendererAPI.hpp"

namespace lux
{
    Scope<RendererAPI> RendererAPI::Create()
    {
        switch(RendererAPI::GetAPI())
        {
            case GraphicsAPI::OPENGL:
                return CreateScope<OpenGLRendererAPI>();
            case GraphicsAPI::VULKAN:
                return nullptr;
            case GraphicsAPI::DIRECTX:
                return nullptr;
            case GraphicsAPI::METAL:
                return nullptr;
        }
    }
}