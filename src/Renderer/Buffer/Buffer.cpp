#include "../../../include/Renderer/Buffer/Buffer.hpp"
#include "../../../include/OpenGL/OpenGLBuffer.hpp"
#include "../../../include/Application/EngineSettings.hpp"

namespace lux
{
    void Buffer::BindBuffer() const noexcept
    {
        m_buffer->BindBuffer();
    }

    void Buffer::UnbindBuffer() const noexcept
    {
        m_buffer->UnbindBuffer();
    }

    Scope<IBuffer> Buffer::CreateBufferFactory(BufferType)
    {
        switch (API)
        {
            case GraphicsAPI::OPENGL:
                return CreateScope<OpenGLBuffer>();
            case GraphicsAPI::VULKAN:
                break;
            case GraphicsAPI::DIRECTX:
                break;
            case GraphicsAPI::METAL:
                break;
        }

        return nullptr;
    }
}
