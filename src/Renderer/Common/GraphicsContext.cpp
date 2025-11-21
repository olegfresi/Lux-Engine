#include "../../../include/Renderer/Common/GraphicsContext.hpp"
#include "../../../include/OpenGL/OpenGLGraphicsContext.hpp"

namespace lux
{
    std::unique_ptr<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch(OperatingSystem)
        {
            case Os::MACOS:
                return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window));

            case Os::LINUX:
                break;

            case Os::WINDOWS:
                break;
        }
    }
}