#include "../../../include/Renderer/Buffer/FrameBuffer.hpp"
#include "../../../include/OpenGL/OpenGLFrameBuffer.hpp"
#include "../../../include/Application/EngineSettings.hpp"
#include "../../../include/Renderer/Common/RenderCommand.hpp"

namespace lux
{
    Scope<IFrameBuffer> FrameBuffer::CreateFrameBuffer(const FrameBufferSpecification& specs) noexcept
    {
        switch (API)
        {
            case GraphicsAPI::OPENGL:
                return CreateScope<OpenGLFrameBuffer>(specs);
            case GraphicsAPI::VULKAN:
                break;
            case GraphicsAPI::DIRECTX:
                break;
            case GraphicsAPI::METAL:
                break;
        }

        return nullptr;
    }


    static TextureType TextureTarget(bool multisample)
    {
        return multisample ? TextureType::Texture2DMultisample : TextureType::Texture2D;
    }

    static bool IsDepthFormat(FrameBufferTextureFormat format)
    {
        switch (format)
        {
            case FrameBufferTextureFormat::DEPTH24_STENCIL8:
                return true;

            default:
                return false;
        }
    }

    static void CreateTextures(bool multisample, std::vector<uint32_t>& textures, size_t count, uint32_t width, uint32_t height)
    {
        for (size_t i = 0; i < count; i++)
        {
            GPUTexture::CreateTexture(&textures[i]);
            GPUTexture::Bind(textures[i], TextureTarget(multisample));
            GPUTexture::GenTextureImage(TextureTarget(multisample), 4, GPUTextureFormat::RGBA, width, height,
                                        GPUPrimitiveDataType::UNSIGNED_BYTE, GPUTextureFormat::RGBA);
        }
    }

    FrameBuffer::FrameBuffer(const FrameBufferSpecification& spec) noexcept : m_specs(spec), m_frameBuffer(CreateFrameBuffer(spec))
    {
        for(auto format : m_specs.m_attachmentsSpecifications)
        {
            if(!IsDepthFormat(format.format))
                m_colorAttachmentsFormats.emplace_back(format.format);
            else
                m_depthAttachmentFormat = format.format;
        }

        Invalidate();
    }

    FrameBuffer::~FrameBuffer() noexcept
    {
        m_frameBuffer->DeleteFrameBuffer(m_id);
        GPUTexture::DeleteTexture(&m_depthAttachment);
        GPUTexture::DeleteTextures(m_colorAttachments);
    }

    void FrameBuffer::Invalidate()
    {
        if (m_id)
        {
            m_frameBuffer->DeleteFrameBuffer(m_id);
            GPUTexture::DeleteTexture(&m_depthAttachment);
            GPUTexture::DeleteTextures(m_colorAttachments);

            m_colorAttachments.clear();
            m_depthAttachment = 0;
        }

        m_frameBuffer->GenBuffer(&m_id);
        m_frameBuffer->BindFrameBuffer(m_id);
        bool multisample = m_specs.samples > 1;

        if (!m_specs.m_attachmentsSpecifications.empty())
        {
            m_colorAttachmentsFormats.clear();
            for (auto &spec : m_specs.m_attachmentsSpecifications)
            {
                if (!IsDepthFormat(spec.format))
                    m_colorAttachmentsFormats.push_back(spec.format);
                else
                    m_depthAttachmentFormat = spec.format;
            }

            if (!m_colorAttachmentsFormats.empty())
            {
                m_colorAttachments.resize(m_colorAttachmentsFormats.size());
                CreateTextures(multisample, m_colorAttachments, m_colorAttachments.size(), m_specs.width, m_specs.height);

                for (size_t i = 0; i < m_colorAttachmentsFormats.size(); ++i)
                {
                    GPUTexture::Bind(m_colorAttachments[i], TextureTarget(multisample));

                    switch (m_colorAttachmentsFormats[i])
                    {
                        case FrameBufferTextureFormat::RGBA8:
                            AttachColorTexture(&m_colorAttachments[i], m_specs.samples, GPUTextureFormat::RGBA8, m_specs.width, m_specs.height, i);
                            break;
                        default:
                            break;
                    }
                }

                m_frameBuffer->DrawBuffers(m_colorAttachmentsFormats.size());
            }
            else
            {
                m_frameBuffer->ReadFrameBuffer(FrameBufferMode::NONE);
                m_frameBuffer->WriteFrameBuffer(FrameBufferMode::NONE);
            }
        }

        if (m_depthAttachmentFormat != FrameBufferTextureFormat::NONE)
        {
            GPUTexture::CreateTexture(&m_depthAttachment);
            GPUTexture::Bind(m_depthAttachment, TextureTarget(multisample));
            GPUTexture::GenTextureImage(TextureTarget(multisample), 4, GPUTextureFormat::DEPTH24_STENCIL8, m_specs.width, m_specs.height,
                                        GPUPrimitiveDataType::UNSIGNED_INT_24_8, GPUTextureFormat::DEPTH_STENCIL);

            GPUTexture::SetTextureParameterI(m_depthAttachment, TextureType::Texture2D, TextureParameter::TextureMinFilter, TextureFilter::Linear);
            GPUTexture::SetTextureParameterI(m_depthAttachment, TextureType::Texture2D, TextureParameter::TextureMagFilter, TextureFilter::Linear);
            GPUTexture::SetTextureParameterI(m_depthAttachment, TextureType::Texture2D, TextureParameter::TextureWrapS, TextureWrap::ClampToEdge);
            GPUTexture::SetTextureParameterI(m_depthAttachment, TextureType::Texture2D, TextureParameter::TextureWrapR, TextureWrap::ClampToEdge);
            GPUTexture::SetTextureParameterI(m_depthAttachment, TextureType::Texture2D, TextureParameter::TextureWrapT, TextureWrap::ClampToEdge);
            m_frameBuffer->CreateFrameBufferDepthTexture(m_depthAttachment);
            GPUTexture::Unbind(TextureType::Texture2D);
        }

        m_frameBuffer->UnbindFrameBuffer();
        if (!CheckFrameBufferStatus())
            CORE_ERROR("Frame buffer is not complete");
    }

    void FrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        m_specs.width = width;
        m_specs.height = height;
        Invalidate();
    }

    bool FrameBuffer::CheckFrameBufferStatus() const
    {
        return m_frameBuffer->CheckFrameBufferStatus();
    }

    void FrameBuffer::AttachColorTexture(const NonOwnPtr<uint32_t> id, int samples, GPUTextureFormat format, uint32_t width, uint32_t height, int index) const
    {
        bool multisample = samples > 1;

        GPUTexture::Bind(*id, TextureTarget(multisample));

        if(multisample)
        {
            GPUTexture::GenTextureImage(TextureTarget(multisample), samples, GPUTextureFormat::RGBA8, width, height, GPUPrimitiveDataType::UNSIGNED_BYTE, GPUTextureFormat::RGBA8);
            m_frameBuffer->CreateFrameBufferColorTexture(*id, index, multisample);
        }
        else
        {
            GPUTexture::GenTextureImage(TextureTarget(multisample), 1, GPUTextureFormat::RGBA8, width, height, GPUPrimitiveDataType::UNSIGNED_BYTE, GPUTextureFormat::RGBA);
            GPUTexture::SetTextureParameterI(*id, TextureType::Texture2D, TextureParameter::TextureMinFilter, TextureFilter::Linear);
            GPUTexture::SetTextureParameterI(*id, TextureType::Texture2D, TextureParameter::TextureMagFilter, TextureFilter::Linear);
            GPUTexture::SetTextureParameterI(*id, TextureType::Texture2D, TextureParameter::TextureWrapS, TextureWrap::ClampToEdge);
            GPUTexture::SetTextureParameterI(*id, TextureType::Texture2D, TextureParameter::TextureWrapT, TextureWrap::ClampToEdge);
            GPUTexture::SetTextureParameterI(*id, TextureType::Texture2D, TextureParameter::TextureWrapR, TextureWrap::ClampToEdge);

            m_frameBuffer->CreateFrameBufferColorTexture(*id, index, multisample);
        }
    }

    void FrameBuffer::AttachDepthTexture(const NonOwnPtr<uint32_t> id, GPUTextureFormat format, GPUFrameBufferAttachmentType type, uint32_t width, uint32_t height)
    {
        m_frameBuffer->BindFrameBuffer(m_id);
        GPUTexture::CreateTexture(&m_depthAttachment);
        GPUTexture::Bind(m_depthAttachment, TextureType::Texture2D);
        GPUTexture::GenTextureImage(TextureType::Texture2D, 0, format, width, height,
                                     GPUPrimitiveDataType::UNSIGNED_INT_24_8, GPUTextureFormat::DEPTH_STENCIL);
        GPUTexture::SetTextureParameterI(*id, TextureType::Texture2D, TextureParameter::TextureMinFilter, TextureFilter::Nearest);
        GPUTexture::SetTextureParameterI(*id, TextureType::Texture2D, TextureParameter::TextureMagFilter, TextureFilter::Nearest);
        GPUTexture::SetTextureParameterI(*id, TextureType::Texture2D, TextureParameter::TextureWrapS, TextureWrap::ClampToEdge);
        GPUTexture::SetTextureParameterI(*id, TextureType::Texture2D, TextureParameter::TextureWrapT, TextureWrap::ClampToEdge);
        GPUTexture::SetTextureParameterI(*id,TextureType::Texture2D, TextureParameter::TextureWrapR, TextureWrap::ClampToEdge);
        m_frameBuffer->CreateFrameBufferDepthTexture(m_depthAttachment);

        if (!CheckFrameBufferStatus())
            CORE_ERROR("Frame buffer is not complete");
    }

    void FrameBuffer::CreateColorAttachment()
    {
        m_colorAttachments.resize(m_specs.m_attachmentsSpecifications.size());
        CreateTextures(m_specs.samples > 1, m_colorAttachments, m_colorAttachments.size(), m_specs.width, m_specs.height);

        for (size_t i = 0; i < m_specs.m_attachmentsSpecifications.size(); i++)
        {
            switch (m_specs.m_attachmentsSpecifications[i].format)
            {
                case FrameBufferTextureFormat::RGBA8:
                    AttachColorTexture(&m_colorAttachments[i], m_specs.samples, GPUTextureFormat::RGBA8, m_specs.width, m_specs.height, i);
                    break;
                default:
                    break;
            }
        }
    }

    void FrameBuffer::CreateDepthStencilAttachment()
    {
        for (int i = 0; i < m_specs.m_attachmentsSpecifications.size(); i++)
            switch (m_specs.m_attachmentsSpecifications[i].format)
            {
            case FrameBufferTextureFormat::DEPTH24_STENCIL8:
                    AttachDepthTexture(&m_depthAttachment, GPUTextureFormat::DEPTH24_STENCIL8,
                                       GPUFrameBufferAttachmentType::DEPTH_STENCIL, m_specs.width, m_specs.height);
                    break;

            default:
                    break;
            }
    }

    void FrameBuffer::Bind() const
    {
        m_frameBuffer->BindFrameBuffer(m_id, m_specs.width, m_specs.height);
    }

    void FrameBuffer::Unbind() const
    {
        m_frameBuffer->UnbindFrameBuffer();
    }

    void FrameBuffer::RestoreDefaultFramebuffer(NonOwnPtr<Window> window, int& width, int& height) const
    {
        m_frameBuffer->RestoreDefaultFramebuffer(window, width, height);
        RenderCommand::SetViewport(0, 0, width, height);
    }
}