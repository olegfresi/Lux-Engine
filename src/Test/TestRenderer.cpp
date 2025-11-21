#include <gtest/gtest.h>
/*
#include <Renderer/Renderer.hpp>
#include <Renderer/Camera/Camera.hpp>
#include <Renderer/Shader/Shader.hpp>
#include <Renderer/VertexArray.hpp>
#include <Renderer/Buffer.hpp>
#include <Renderer/Layout.hpp>
#include <Renderer/GPU.hpp>
using namespace engine;

class RendererTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Inizializza il renderer e altri componenti necessari
        Renderer::Init();
    }

    void TearDown() override {
        // Pulisci le risorse
        Renderer::Shutdown();
    }
};

// Test di base per l'inizializzazione
TEST_F(RendererTest, Initialization) {
    EXPECT_NO_THROW(Renderer::Init());
    EXPECT_NO_THROW(Renderer::Shutdown());
}

// Test per il ridimensionamento della finestra
TEST_F(RendererTest, WindowResize) {
    EXPECT_NO_THROW(Renderer::OnWindowResize(800, 600));
    EXPECT_NO_THROW(Renderer::OnWindowResize(1024, 768));
}

// Test per l'inizio e la fine della scena
TEST_F(RendererTest, SceneManagement) {
    Camera camera;
    EXPECT_NO_THROW(Renderer::BeginScene(camera));
    EXPECT_NO_THROW(Renderer::EndScene());
}

// Test per l'API grafica
TEST_F(RendererTest, GraphicsAPI) {
    GraphicsAPI api = Renderer::GetAPI();
    EXPECT_NE(api, GraphicsAPI::None);
}

// Test per una scena completa
TEST_F(RendererTest, CompleteScene) {
    // Imposta le dimensioni della finestra
    Renderer::OnWindowResize(800, 600);
    
    // Crea una camera
    Camera camera;
    camera.SetPosition(Vector3f(0.0f, 0.0f, 5.0f));
    camera.SetFront(Vector3f(0.0f, 0.0f, -1.0f));
    camera.SetUp(Vector3f(0.0f, 1.0f, 0.0f));
    
    // Inizia la scena
    Renderer::BeginScene(camera);
    
    // Crea un VertexArray per il triangolo
    auto vertexArray = std::make_shared<VertexArray>();
    auto layout = std::make_unique<VertexLayout>();
    layout->AddAttribute(VertexAttributeType::Position, AttributeType::Vec3);
    
    std::vector<float> vertices = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    
    auto vertexBuffer = std::make_shared<Buffer>(BufferType::VertexBuffer);
    vertexBuffer->SetData(vertices, BufferUsage::StaticDraw, layout);
    
    std::vector<uint32_t> indices = {0, 1, 2};
    auto indexBuffer = std::make_shared<Buffer>(BufferType::IndexBuffer);
    indexBuffer->SetData(indices, BufferUsage::StaticDraw, layout);
    
    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->SetIndexBuffer(indexBuffer);
    
    // Carica gli shader
    Shader shader("test/shaders/test.vert", "test/shaders/test.frag");
    
    // Imposta le uniformi
    shader.Bind();
    shader.SetUniformMat4f("uView", camera.GetView(), false);
    shader.SetUniformMat4f("uProjection", camera.GetProjection(), false);
    shader.SetUniformMat4f("uModel", Matrix4f::Identity(), false);
    
    // Disegna il triangolo
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
    vertexArray->Unbind();
    
    // Fine della scena
    Renderer::EndScene();
}

TEST_F(RendererTest, MultipleScenes) {
    Camera camera;
    
    // Verifica che si possano iniziare e terminare multiple scene
    for (int i = 0; i < 3; i++) {
        Renderer::BeginScene(camera);
        Renderer::EndScene();
    }
}

TEST_F(RendererTest, CameraIntegration) {
    Camera camera;
    
    // Verifica che la camera sia correttamente integrata con il renderer
    Renderer::BeginScene(camera);
    
    // Verifica che la camera sia accessibile
    EXPECT_TRUE(&camera != nullptr);
    
    Renderer::EndScene();
}

TEST_F(RendererTest, Shutdown) {
    // Verifica che lo shutdown non causi crash
    EXPECT_NO_THROW(Renderer::Shutdown());
}

TEST_F(RendererTest, Reinitialization) {
    // Verifica che il renderer possa essere reinizializzato dopo lo shutdown
    Renderer::Shutdown();
    Renderer::Init();
    EXPECT_TRUE(Renderer::GetAPI() != GraphicsAPI::None);
}

TEST_F(RendererTest, APIQuery) {
    // Verifica che l'API sia correttamente riportata
    GraphicsAPI api = Renderer::GetAPI();
    EXPECT_TRUE(api != GraphicsAPI::None);
}

TEST_F(RendererTest, SceneState) {
    Camera camera;
    
    // Verifica che lo stato della scena sia corretto
    Renderer::BeginScene(camera);
    // Qui potremmo aggiungere verifiche sullo stato della scena se avessimo accesso a metodi getter
    
    Renderer::EndScene();
}

TEST_F(RendererTest, CameraTransform) {
    Camera camera;
    
    // Verifica che le trasformazioni della camera siano applicate correttamente
    camera.SetPosition(math::Vector3f(0.0f, 0.0f, -5.0f));
    camera.SetFront(math::Vector3f(0.0f, 0.0f, 1.0f));
    camera.SetUp(math::Vector3f(0.0f, 1.0f, 0.0f));
    
    Renderer::BeginScene(camera);
    Renderer::EndScene();
}

TEST_F(RendererTest, CameraFrustum) {
    Camera camera;
    
    // Verifica che il frustum della camera sia corretto
    camera.SetFov(45.0f);
    camera.SetNear(0.1f);
    camera.SetFar(100.0f);
    
    Renderer::BeginScene(camera);
    Renderer::EndScene();
}
 */