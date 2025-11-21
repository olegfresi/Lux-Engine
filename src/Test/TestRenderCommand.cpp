#include <gtest/gtest.h>
/*
#include <Renderer/RenderCommand.hpp>
#include <Renderer/VertexArray.hpp>
#include <Renderer/Buffer.hpp>
#include <Renderer/Layout.hpp>
#include <Renderer/GPU.hpp>

class RenderCommandTest : public ::testing::Test {
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
TEST_F(RenderCommandTest, Initialization) {
    EXPECT_NO_THROW(RenderCommand::Init());
}

// Test per il viewport
TEST_F(RenderCommandTest, Viewport) {
    EXPECT_NO_THROW(RenderCommand::SetViewport(0, 0, 800, 600));
}

// Test per il colore di pulizia
TEST_F(RenderCommandTest, ClearColor) {
    Color color(0.2f, 0.3f, 0.4f, 1.0f);
    EXPECT_NO_THROW(RenderCommand::SetClearColor(color));
}

// Test per la pulizia del buffer
TEST_F(RenderCommandTest, Clear) {
    EXPECT_NO_THROW(RenderCommand::Clear());
}

// Test per il disegno indicizzato
TEST_F(RenderCommandTest, DrawIndexed) {
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
    
    EXPECT_NO_THROW(RenderCommand::DrawIndexed(vertexArray));
    EXPECT_NO_THROW(RenderCommand::DrawIndexed(vertexArray, 3));
}

// Test per il disegno di linee
TEST_F(RenderCommandTest, DrawLines) {
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
    
    vertexArray->AddVertexBuffer(vertexBuffer);
    
    EXPECT_NO_THROW(RenderCommand::DrawLines(vertexArray, 3));
}

// Test per la larghezza delle linee
TEST_F(RenderCommandTest, LineWidth) {
    EXPECT_NO_THROW(RenderCommand::SetLineWidth(2.0f));
    EXPECT_NO_THROW(RenderCommand::SetLineWidth(1.0f));
}

// Test per il disegno di una scena completa
TEST_F(RenderCommandTest, CompleteScene) {
    // Imposta il viewport
    RenderCommand::SetViewport(0, 0, 800, 600);
    
    // Imposta il colore di pulizia
    Color clearColor(0.2f, 0.3f, 0.4f, 1.0f);
    RenderCommand::SetClearColor(clearColor);
    
    // Pulisci il buffer
    RenderCommand::Clear();
    
    // Crea e disegna un triangolo
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
    
    // Imposta la larghezza delle linee e disegna
    RenderCommand::SetLineWidth(2.0f);
    EXPECT_NO_THROW(RenderCommand::DrawIndexed(vertexArray));
}
 */