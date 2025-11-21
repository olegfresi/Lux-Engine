#include "../../../include/Renderer/RenderPasses/ShadowPass.hpp"
#include "../../../include/Renderer/Common/RenderCommand.hpp"

namespace lux
{
    ShadowPass::ShadowPass(Shader& depthShader, const FrameBufferSpecification& spec) : m_depthShader{depthShader}, m_frameBuffer{spec},
        m_shadowSize{2048}, m_shadowTextureUnit{ GPUTexture::AcquireUnit(m_frameBuffer.GetDepthTexture()).value_or(0) }
    {

    }

    void ShadowPass::Begin(const Scene& scene, const Matrix4f& lightSpaceMatrix, const Matrix4f& model)
    {
        RenderCommand::Clear();
        RenderCommand::EnableDepthTest(true, DepthTestFunction::LESS_OR_EQUAL);
        RenderCommand::EnableFaceCulling(false);
        RenderCommand::EnableDepthWrite(true);

        m_depthShader.Bind();
        m_depthShader.SetUniform("lightSpaceMatrix", lightSpaceMatrix);
        m_depthShader.SetUniform("model", model);
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

    void ShadowPass::End(NonOwnPtr<Window> window)
    {
        RenderCommand::Clear();
        RenderCommand::ClearDepth();
        int width = 0;
        int height = 0;
        m_frameBuffer.RestoreDefaultFramebuffer(window, width, height);
        GLCheck(glfwGetFramebufferSize((GLFWwindow *)window->GetNativeWindowHandle(), &width, &height));
        GLCheck(glViewport(0, 0, width, height));
    }

    void ShadowPass::BindDepthTexture() noexcept
    {
        m_frameBuffer.BindDepthTexture(m_shadowTextureUnit);
    }

    void ShadowPass::Execute()
    {}
}