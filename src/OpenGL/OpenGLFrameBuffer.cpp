#include "../../include/OpenGL/OpenGLFrameBuffer.hpp"

namespace lux
{
    void OpenGLFrameBuffer::CreateFrameBufferColorTexture(uint32_t id, int index, bool multisample) const noexcept
    {
        if(multisample)
        {
            GLCheck(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, id, 0));
        }
        else
        {
            GLCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, id, 0));
        }
    }

    void OpenGLFrameBuffer::CreateFrameBufferDepthTexture(uint32_t id) const noexcept
    {
        GLCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, id, 0));
    }

    bool OpenGLFrameBuffer::CheckFrameBufferStatus() const
    {
        GLCheck(if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Framebuffer is incomplete!"));

        return true;
    }

    void OpenGLFrameBuffer::UnbindFrameBuffer() const
    {
        GLCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void OpenGLFrameBuffer::BindFrameBuffer(uint32_t id) const
    {
        GLCheck(glBindFramebuffer(GL_FRAMEBUFFER, id));
    }

    void OpenGLFrameBuffer::BindFrameBuffer(uint32_t id, uint32_t width, uint32_t height) const
    {
        GLCheck(glBindFramebuffer(GL_FRAMEBUFFER, id));
        GLCheck(glViewport(0, 0, width, height));
    }

    void OpenGLFrameBuffer::ReadFrameBuffer(FrameBufferMode mode) const noexcept
    {
        GLCheck(glReadBuffer(static_cast<GLenum>(mode)));
    }

    void OpenGLFrameBuffer::WriteFrameBuffer(FrameBufferMode mode) const noexcept
    {
        GLCheck(glDrawBuffer(static_cast<GLenum>(mode)));
    }

    void OpenGLFrameBuffer::DrawBuffers(size_t size) const noexcept
    {
        std::vector<GLenum> drawBuffers(size);
        for (size_t i = 0; i < drawBuffers.size(); ++i)
            drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;

        GLCheck(glDrawBuffers(static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data()));
    }

    void OpenGLFrameBuffer::GenBuffer(uint32_t *id) const
    {
        GLCheck(glGenFramebuffers(1, id));
    }

    void OpenGLFrameBuffer::DeleteFrameBuffer(uint32_t id) const
    {
        GLCheck(glDeleteFramebuffers(1, &id));
    }

    Color OpenGLFrameBuffer::ReadFrameBuffer(uint32_t x, uint32_t y, GPUTexturePixelFormat pixelFormat, GPUPrimitiveDataType dataType) const noexcept
    {
        return Color();
    }

    void OpenGLFrameBuffer::RestoreDefaultFramebuffer(Window* window, int& width, int& height) const noexcept
    {
        GLCheck(glfwGetFramebufferSize(static_cast<GLFWwindow*>(window->GetNativeWindowHandle()), &width, &height));
        GLCheck(glViewport(0, 0, width, height));
    }
}