#define GL_SILENCE_DEPRECATION
#include "../../include/OpenGL/OpenGLRendererAPI.hpp"
#include "../../include/OpenGL/OpenglError.hpp"
#include "glfw/include/GLFW/glfw3.h"

namespace lux
{
    void OpenGLRendererAPI::Init()
    {
    }

    void OpenGLRendererAPI::ShutDown()
    {

    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        GLCheck(glViewport(x, y, width, height));
    }

    void OpenGLRendererAPI::SetClearColor(const Color &color)
    {
        GLCheck(glClearColor(color.GetR(), color.GetG(), color.GetB(), color.GetA()));
    }

    void OpenGLRendererAPI::Clear(const Color& color)
    {
        ClearColor(color);
        ClearDepth();
        ClearStencil();
    }

    void OpenGLRendererAPI::ClearColor(const Color& color)
    {
        SetClearColor(color);
        GLCheck(glClear(GL_COLOR_BUFFER_BIT));
    }

    void OpenGLRendererAPI::ClearDepth()
    {
        GLCheck(glClear(GL_DEPTH_BUFFER_BIT) );
    }

    void OpenGLRendererAPI::ClearStencil()
    {
        GLCheck(glClear(GL_STENCIL_BUFFER_BIT));
    }

    void OpenGLRendererAPI::BeginFrame()
    {

    }

    void OpenGLRendererAPI::EndFrame()
    {

    }

    void OpenGLRendererAPI::SetWireframeMode(bool mode)
    {
        if (mode)
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        }
        else
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
    }

    void OpenGLRendererAPI::EnableDepthTest(bool mode, DepthTestFunction func)
    {
        if(mode)
        {
            GLCheck( glEnable( GL_DEPTH_TEST ));
            GLCheck( glDepthFunc( static_cast<GLenum>(func)));
        }
        else
        {
            GLCheck( glDisable( GL_DEPTH_TEST ));
        }
    }

    void OpenGLRendererAPI::EnableStencilTest(bool mode)
    {
        if(mode)
        {
            GLCheck( glEnable( GL_STENCIL_TEST ));
        }
        else
        {
            GLCheck( glDisable( GL_STENCIL_TEST ));
        }
    }

    void OpenGLRendererAPI::EnableBlending(bool mode, GPUBlendFactor source, GPUBlendFactor dest)
    {
        if(mode)
        {
            GLCheck( glEnable( GL_BLEND ));
            GLCheck( glBlendFunc( static_cast<GLenum>(source), static_cast<GLenum>(dest)));
        }
        else
        {
            GLCheck( glDisable( GL_BLEND ));
        }
    }

    void OpenGLRendererAPI::EnableDepthWrite(bool enabled)
    {
        if(enabled)
        {
            GLCheck( glDepthMask( GL_TRUE ));
        }
        else
        {
            GLCheck( glDepthMask( GL_FALSE ));
        }
    }

    void OpenGLRendererAPI::EnableCullFacing(bool mode, GPUCullFaceType type, GPUCullFaceOrder order)
    {
        if(mode)
        {
            GLCheck( glEnable( GL_CULL_FACE));
            GLCheck( glCullFace( static_cast<GLenum>(type)));
            GLCheck( glFrontFace(static_cast<GLenum>(order)));
        }
        else
        {
            GLCheck( glDisable( GL_CULL_FACE ));
        }
    }

    void OpenGLRendererAPI::EnableMultiSampling(bool mode)
    {
        if(mode)
        {
            GLCheck(glEnable(GL_MULTISAMPLE));
        }
        else
        {
            GLCheck(glDisable(GL_MULTISAMPLE));
        }
    }
}