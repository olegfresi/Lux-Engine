#include "../../../include/Renderer/RenderPasses/ShadowPass.hpp"
#include "../../../include/Renderer/Common/RenderCommand.hpp"

namespace lux
{
    ShadowPass::ShadowPass(Shader& depthShader, const FrameBufferSpecification& spec) : m_depthShader{depthShader},
        m_frameBuffer{spec}, m_shadowTextureUnit{ GPUTexture::AcquireUnit(m_frameBuffer.GetDepthTexture()).value_or(0) } {}

    void ShadowPass::Begin(const Scene& scene, const Matrix4f& lightSpaceMatrix, const Matrix4f& model) const
    {
        RenderCommand::Clear();
        RenderCommand::EnableDepthTest(true, DepthTestFunction::LESS_OR_EQUAL);
        RenderCommand::EnableFaceCulling(false);
        RenderCommand::EnableDepthWrite(true);

        m_depthShader.Bind();
        m_depthShader.SetUniform("lightSpaceMatrix", lightSpaceMatrix);
        m_depthShader.SetUniform("model", model);
        m_depthShader.SetUniform("useInstancing", false);
        RenderCommand::SetViewport(0, 0, m_shadowSize, m_shadowSize);
        m_frameBuffer.Bind();
        RenderCommand::ClearDepth();

        for(const auto& mesh : scene.GetMeshes())
            mesh->SetShader(&m_depthShader);

        for(const auto& primitive : scene.GetPrimitives())
            primitive->SetShader(&m_depthShader);

        for(const auto& mesh : scene.GetMeshes())
            mesh->Draw(GPUDrawPrimitive::TRIANGLES, GPUPrimitiveDataType::UNSIGNED_INT);

        for(const auto& primitive : scene.GetPrimitives())
            primitive->Draw(scene.GetCamera().GetView(), scene.GetCamera().GetProjection());

        m_frameBuffer.Unbind();
    }

    void ShadowPass::End(NonOwnPtr<Window> window) const
    {
        RenderCommand::Clear();
        RenderCommand::ClearDepth();
        int width = 0;
        int height = 0;
        m_frameBuffer.RestoreDefaultFramebuffer(window, width, height);
    }

    void ShadowPass::BindDepthTexture() const noexcept
    {
        m_frameBuffer.BindDepthTexture(m_shadowTextureUnit);
    }

}