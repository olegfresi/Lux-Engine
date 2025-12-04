#include "../../include/Application/Application.hpp"
#include "../../include/Renderer/Shader/Shader.hpp"
#include "../../include/Renderer/Mesh/Mesh.hpp"
#include "../../include/Renderer/Common/GraphicsContext.hpp"
#include "../../include/Renderer/Common/RenderCommand.hpp"
#include "../../include/Renderer/Shader/ShaderCompiler.hpp"
#include "../../../thirdparty/glslang/glslang/Public/ShaderLang.h"
#include "../../../thirdparty/glslang/glslang/Include/intermediate.h"
#include "../../../thirdparty/glslang/glslang/MachineIndependent/ParseHelper.h"
#include "../../include/Scene/Scene.hpp"
#include "../../include/Application/FPSCounter.hpp"
#include "../../include/Renderer/Texture/CubeMap.hpp"
#include "../../include/FileSystem/FileSystem.hpp"
#include "../../include/Renderer/Primitives/Plane.hpp"
#include "../../include/Renderer/RenderPasses/ShadowPass.hpp"
#include "../../include/Input/Mouse.hpp"
#include "../../include/Event/Events.hpp"
#include "GLFW/glfw3.h"
#include <vector>
#include <string>
#define GLM_ENABLE_EXPERIMENTAL
/*
#include "../../../thirdparty/glslang/glslang/Public/ResourceLimits.h"
#include "../../../thirdparty/glslang/glslang/Include/BaseTypes.h"
#include "../../../thirdparty/glslang/glslang/Include/Types.h"
#include "../../../thirdparty/glslang/glslang/Include/InitializeGlobals.h"
#include "../../../thirdparty/glslang/glslang/Include/ShHandle.h"
#include "../../../thirdparty/glslang/glslang/Include/InfoSink.h"
#include "../../../thirdparty/glslang/glslang/Include/Common.h"
*/

namespace lux
{
    using namespace math;
    static ActionMapper mapper{};

    void Setup()
    {
        mapper.BindKeyCombo(InputAction::MoveForward, {InputType::Keyboard}, {static_cast<int>(KeyboardKeys::KeyW)});
        mapper.BindKeyCombo(InputAction::MoveBackward, {InputType::Keyboard}, {static_cast<int>(KeyboardKeys::KeyS)});
        mapper.BindKeyCombo(InputAction::MoveLeft, {InputType::Keyboard}, {static_cast<int>(KeyboardKeys::KeyA)});
        mapper.BindKeyCombo(InputAction::MoveRight, {InputType::Keyboard}, {static_cast<int>(KeyboardKeys::KeyD)});
        mapper.BindKeyCombo(InputAction::Pan, {InputType::Keyboard}, {static_cast<int>(KeyboardKeys::KeyLeftAlt)});

        mapper.BindKeyCombo(InputAction::MoveAround, {InputType::Mouse}, {static_cast<int>(MouseButtons::MouseButtonRight)});
        mapper.BindKeyCombo(InputAction::MoveUpDown, {InputType::Mouse}, {static_cast<int>(MouseButtons::MouseCursorUpDown)});

        mapper.BindKeyCombo(InputAction::MoveForward, {InputType::Gamepad}, {static_cast<int>(GamepadAxes::AxisLeftY)});
        mapper.BindKeyCombo(InputAction::MoveLeft, {InputType::Gamepad}, {static_cast<int>(GamepadAxes::AxisLeftX)});
        mapper.BindKeyCombo(InputAction::MoveAround, {InputType::Gamepad}, {static_cast<int>(GamepadAxes::AxisRightX)});
        mapper.BindKeyCombo(InputAction::MoveUpDown, {InputType::Gamepad}, {static_cast<int>(GamepadAxes::AxisRightY)});
        mapper.BindKeyCombo(InputAction::Zoom, {InputType::Gamepad}, {static_cast<int>(GamepadAxes::AxisRT)});

        mapper.BindKeyCombo(InputAction::MoveForward, {InputType::Gamepad}, {static_cast<int>(GamepadKeys::ButtonTriangle)});
        mapper.BindKeyCombo(InputAction::MoveBackward, {InputType::Gamepad}, {static_cast<int>(GamepadKeys::ButtonCross)});
        mapper.BindKeyCombo(InputAction::MoveLeft, {InputType::Gamepad}, {static_cast<int>(GamepadKeys::ButtonSquare)});
        mapper.BindKeyCombo(InputAction::MoveRight, {InputType::Gamepad}, {static_cast<int>(GamepadKeys::ButtonCircle)});
    }

    AppContext::~AppContext()
    {
        for (auto& layer : m_layers)
        {
            if(layer != nullptr)
                delete (layer);

            layer = nullptr;
        }
    }

    // TODO: Remove camera controller and camera, move away dispatcher and inputDeviceSystem and mapper
    Application::Application() : m_dispatcher{ &m_context->m_dispatcher }, m_mapper{mapper}, m_window{ CreateScope<Window>(settings, m_dispatcher) },
                    m_inputDeviceSystem{ CreateScope<InputDevice>( m_dispatcher, &m_mapper) },
                    m_camera{ Vector3f(1.0f, 0.0f, 3.0f), Vector3f(0.0f, 0.0f, -1.0f), Vector3f(0.0f, 1.0f, 0.0f), 800 / 600, 45.0f, -90.0f, 0.0f},
                    m_cameraController{ m_camera, m_mapper, m_dispatcher }
    {
        INFO("Application created");
        m_layerID = TypeID<Application>();
        m_window->SetWindowIcon("LuxLogo.png");
        InitSubSystems();
    }

    Application::~Application()
    {
        CORE_INFO("Application destroyed");
    }

    void Application::RunContext() const noexcept
    {
        for (const auto& layer : m_context->m_layers)
            layer->OnUpdate();
    }

    void Application::InputDeviceSetup()
    {
        m_mouse = CreateScope<Mouse>(m_dispatcher, &m_mapper);
        m_keyboard = CreateScope<Keyboard>(m_dispatcher, &m_mapper);
        m_gamepad = CreateScope<Gamepad>(m_dispatcher, &m_mapper);
    }

    void Application::InitSubSystems()
    {
        InputDeviceSetup();
        RenderCommand::Init();
        ShaderCompiler::Initialize();
    }

    void Render(const Scene& scene, const Matrix4f& lightSpaceMatrix, Shader& depthShader, const FrameBufferSpecification& fbSpecs)
    {
        // Already in shadowPass.Begin
        // RenderCommand::EnableDepthWrite(true);
        // RenderCommand::EnableDepthTest(true, DepthTestFunction::LESS);
        // RenderCommand::EnableFaceCulling(true, GPUCullFaceType::BACK, GPUCullFaceOrder::COUNTER_CLOCKWISE);

        ShadowPass shadowPass{depthShader, fbSpecs};
        shadowPass.Begin(scene, lightSpaceMatrix, Identity4f);

        for (const auto& mesh : scene.GetMeshes())
        {
            mesh->SetupMesh();
            mesh->GetShader()->Bind();
            auto m = Matrix4f::Scale(Vector3f(0.3f, 0.3f, 0.3f));
            mesh->GetShader()->SetUniform("model", m);
            mesh->GetShader()->SetUniform("view", scene.GetCamera().GetView());
            mesh->GetShader()->SetUniform("projection", scene.GetCamera().GetProjection());
            mesh->GetShader()->SetUniform("viewPos", scene.GetCamera().GetPosition());
            mesh->Draw(GPUDrawPrimitive::TRIANGLES, GPUPrimitiveDataType::UNSIGNED_INT);
        }

        for(const auto& primitive : scene.GetPrimitives())
        {
            primitive->GetShader().Bind();
            primitive->Draw(scene.GetCamera().GetView(), scene.GetCamera().GetProjection());
        }
    }

    void Application::Run()
    {
        std::vector<std::filesystem::path> faces = FileSystem::GetFilesInDirectory("assets/textures");

        TextureSpecification specs
        {
            .width = 0,
            .height = 0,
            .channels = 0,
            .mipMap = false,
            .type = TextureType::Texture2D,
            .format = GPUTexturePixelFormat::RGB,
            .minFilter = TextureFilter::Linear,
            .magFilter = TextureFilter::Linear,
            .mipmapMinFilter = TextureFilter::LinearMipmapLinear,
            .mipmapMagFilter = TextureFilter::Linear,
            .wrapR = TextureWrap::Repeat,
            .wrapS = TextureWrap::Repeat,
            .wrapT = TextureWrap::Repeat,
            .samplerName = "diffuseTexture",
            .filePath = {"assets/hexagons_medieval.png"}
        };

        TextureSpecification specs2
        {
            .width = 0,
            .height = 0,
            .channels = 0,
            .mipMap = false,
            .type = TextureType::Texture2D,
            .format = GPUTexturePixelFormat::RGB,
            .minFilter = TextureFilter::Linear,
            .magFilter = TextureFilter::Linear,
            .mipmapMinFilter = TextureFilter::LinearMipmapLinear,
            .mipmapMagFilter = TextureFilter::Linear,
            .wrapR = TextureWrap::Repeat,
            .wrapS = TextureWrap::Repeat,
            .wrapT = TextureWrap::Repeat,
            .samplerName = "diffuseTexture",
            .filePath = {"assets/wood.png"}
        };

        Scene scene{"Main scene", &m_camera};
        FPSCounter counter;

        /*
        Ref<Mesh> objMesh = CreateRef<Mesh>(MeshType::STATIC, objShader, specs);
        objMesh->LoadMeshFromFile("assets/castle.obj", "assets/castle.mtl");

        objShader.Bind();
        objShader.SetUniform("model", math::Identity4f);
        objShader.SetUniform("objectColor", math::Vector3f(1.0f, 0.5f, 0.31f));
        objShader.SetUniform("lightColor", math::Vector3f(1.0f, 1.0f, 1.0f));
        objShader.SetUniform("lightPos", math::Vector3f(1.2f, 1.0f, 2.0f));
        objShader.SetUniform("ambient", objMesh->m_material.ambient);
        objShader.SetUniform("diffuse", objMesh->m_material.diffuse);
        objShader.SetUniform("specular", objMesh->m_material.specular);
        objShader.SetUniform("shininess", objMesh->m_material.shininess);
        objShader.SetUniform("roughness", objMesh->m_material.roughness);
        objShader.SetUniform("transparency", objMesh->m_material.transparency);
        objShader.Unbind();

        Ref<Mesh> lightSphere = CreateRef<Mesh>(MeshType::STATIC, lightShader);
        lightSphere->LoadMeshFromFile("assets/sphere.obj");

        scene.AddMesh(objMesh);
        scene.AddMesh(lightSphere);
        */

        Shader shadowShader("assets/Shaders/shadow.vert", "assets/Shaders/shadow.frag");
        Shader depthShader("assets/Shaders/depth.vert", "assets/Shaders/depth.frag");
        Shader frustumShader("assets/Shaders/frustum.vert", "assets/Shaders/frustum.frag");
        Shader primitiveShader("assets/Shaders/primitive.vert", "assets/Shaders/primitive.frag");
        Shader objShader("assets/Shaders/vertex_shader2.vert", "assets/Shaders/fragment_shader2.frag");
        Shader lightShader("assets/Shaders/lightVShader.vert", "assets/Shaders/lightFShader.frag");
        Shader cubeShader("assets/Shaders/primitive.vert", "assets/Shaders/primitive.frag");
        Shader skyBoxShader("assets/Shaders/skybox.vert", "assets/Shaders/skybox.frag");
        SkyBox skybox(faces, &skyBoxShader, Vector3f(100.0f, 100.0f, 100.0f), "skybox");

        Texture2D diffuseTexture{specs};
        Texture2D woodTexture{specs2};

        Ref<Mesh> objMesh = CreateRef<Mesh>(MeshType::STATIC, &shadowShader, specs);
        objMesh->LoadMeshFromFile("assets/castle.obj", "assets/castle.mtl");
        scene.AddMesh(objMesh);

        Ref<Plane> plane = CreateRef<Plane>(&cubeShader, Vector3f(0.4f, -1.0f, 0.3f), Vector2f(20.0f, 20.0f));

        FrameBufferSpecification fbSpec;
        fbSpec.width = 2048;
        fbSpec.height = 2048;
        fbSpec.m_attachmentsSpecifications.push_back(FrameBufferTextureSpecification(FrameBufferTextureFormat::DEPTH));
        ShadowPass shadowPass(depthShader, fbSpec);

        float frustumSize = 10.0f;
        Vector3f lightPos = Vector3f(-3.5f, 4.0f, 2.5f);
        Vector3f lightDirection = (Vector3f(0, 0, 0) - lightPos);
        lightDirection.Normalize();
        Vector3f lightTarget;
        Matrix4f lightProjection = Matrix4f::Orthographic(-frustumSize, frustumSize, -frustumSize, frustumSize, 1.0f, 10.0f);
        //Matrix4f lightView = Matrix4f::LookAt(lightPos, lightTarget, Vector3f (0, 1, 0));
        Matrix4f lightSpaceMatrix{};

        std::string shaderSource = ReadShaderFile("assets/Shaders/vertex_shader2.vert");
        glslang::TProgram program;
        if (!CompileGLSL(shaderSource, EShLangVertex, program))
        {
            glslang::FinalizeProcess();
            std::cout << "Error" << std::endl;
            return;
        }

        ReflectionVisitor visitor;
        if (const glslang::TIntermediate* intermediate = program.getIntermediate(EShLangVertex))
            if (TIntermNode* root = intermediate->getTreeRoot())
                root->traverse(&visitor);

        PrintShaderReflection(visitor.data);

        //SetupFrustumBuffers();

        constexpr int n = 5;
        std::vector<SceneObject> objects;

        for (const auto& mesh : scene.GetMeshes())
            for (int i = 0; i < n; ++i)
                objects.push_back({
                    mesh.get(),
                    { Matrix4f::Translate(Vector3f{ i * 8.0f, 0.0f, 0.0f }), Identity4f, Identity4f}
                });

        auto grouped = scene.GroupMeshInstances(objects);

        for (auto& [meshPtr, transforms] : grouped)
        {
            if (transforms.size() > 1)
                meshPtr->SetupMeshInstanced(transforms);
            else
                meshPtr->SetupMesh();
        }


        while (!m_window->ShouldClose())
        {
            // float radius = 5.0f;
            // float orbitSpeed = 1.0f;
            // float fixedHeight = 1.0f;
            double time = glfwGetTime();
            double deltaTime = time - m_lastFrameTime;
            m_lastFrameTime = time;

            // float lightX = radius * cosf(orbitSpeed * time * 0.5);
            // float lightZ = radius * sinf(orbitSpeed * time * 0.5);

            lightTarget = lightPos + lightDirection;
            lightSpaceMatrix = lightProjection * Matrix4f::LookAt(lightPos, lightTarget, Vector3f(0, 1, 0));
            //Render(scene);

            shadowPass.Begin(scene, lightSpaceMatrix, Identity4f);
            shadowPass.End(m_window.get());

            skybox.DrawSkyBox(Identity4f, scene.GetCamera().GetView(), scene.GetCamera().GetProjection());

            // == RENDER PASS ==
            RenderCommand::EnableDepthWrite(true);
            RenderCommand::EnableDepthTest(true, DepthTestFunction::LESS);
            RenderCommand::EnableFaceCulling(true, GPUCullFaceType::BACK, GPUCullFaceOrder::COUNTER_CLOCKWISE);

            shadowShader.Bind();
            shadowShader.SetUniform("model", Identity4f);
            shadowShader.SetUniform("lightSpaceMatrix", lightSpaceMatrix);
            shadowShader.SetUniform("view", m_camera.GetView());
            shadowShader.SetUniform("projection", m_camera.GetProjection());
            shadowShader.SetUniform("lightPos", lightPos);
            shadowShader.SetUniform("viewPos", m_camera.GetPosition());
            shadowShader.SetUniform("shadowMap", shadowPass.GetShadowTextureUnit());
            shadowPass.BindDepthTexture();

            shadowShader.SetUniform("diffuseTexture", diffuseTexture.GetTextureUnit());
            diffuseTexture.Bind(diffuseTexture.GetTextureUnit());
            objMesh->SetShader(&shadowShader);

            for (auto& [meshPtr, transforms] : grouped)
            {
                uint32_t instanceCount = static_cast<uint32_t>(transforms.size());

                if (instanceCount > 1)
                    meshPtr->Draw(
                        GPUDrawPrimitive::TRIANGLES,
                        GPUPrimitiveDataType::UNSIGNED_INT,
                        instanceCount,
                        true
                    );
                else
                    meshPtr->Draw(
                        GPUDrawPrimitive::TRIANGLES,
                        GPUPrimitiveDataType::UNSIGNED_INT,
                        1,
                        false
                    );
            }

            shadowShader.SetUniform("diffuseTexture", woodTexture.GetTextureUnit());
            woodTexture.Bind(woodTexture.GetTextureUnit());
            plane->SetTexture(&woodTexture);
            plane->SetShader(&shadowShader);
            plane->Draw(m_camera.GetView(), m_camera.GetProjection());

            //UpdateFrustumCorners(lightProjection, lightView);
            //DrawFrustum(m_camera.GetView(), m_camera.GetProjection(), frustumShader);

            m_gamepad->Update(deltaTime);
            counter.Update();

            m_window->GetRenderContext()->SwapBuffers();
            m_cameraController.UpdateCamera(deltaTime);

            RunContext();

            m_context->m_dispatcher.PollEvents();
            m_window->ProcessEvents();
            m_window->VSync(false);
        }

        ShaderCompiler::Finalize();
        m_window->Close();
    }
}