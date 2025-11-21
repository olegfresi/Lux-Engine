#include <gtest/gtest.h>
/*
#include <Renderer/Mesh/Mesh.hpp>
#include <Renderer/Mesh/MeshLoader.hpp>
#include <Renderer/Shader/Shader.hpp>
#include <Renderer/Texture/Texture.hpp>

class MeshTest : public ::testing::Test {
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

// Test di base per la creazione e l'inizializzazione
TEST_F(MeshTest, Creation) {
    auto mesh = Mesh::Create();
    ASSERT_NE(mesh, nullptr);
    EXPECT_EQ(mesh->GetVertexCount(), 0);
    EXPECT_EQ(mesh->GetIndexCount(), 0);
}

TEST_F(MeshTest, LoadFromFile) {
    auto mesh = Mesh::Create();
    ASSERT_NE(mesh, nullptr);
    
    bool success = mesh->LoadMeshFromFile("test/meshes/test.obj");
    EXPECT_TRUE(success);
    EXPECT_GT(mesh->GetVertexCount(), 0);
    EXPECT_GT(mesh->GetIndexCount(), 0);
}

TEST_F(MeshTest, VertexData) {
    auto mesh = Mesh::Create();
    ASSERT_NE(mesh, nullptr);
    
    bool success = mesh->LoadMeshFromFile("test/meshes/test.obj");
    ASSERT_TRUE(success);
    
    const auto& vertices = mesh->GetVertices();
    EXPECT_GT(vertices.size(), 0);
    
    // Verifica che i dati dei vertici siano validi
    for (const auto& vertex : vertices) {
        EXPECT_FALSE(std::isnan(vertex.Position.x));
        EXPECT_FALSE(std::isnan(vertex.Position.y));
        EXPECT_FALSE(std::isnan(vertex.Position.z));
        EXPECT_FALSE(std::isnan(vertex.Normal.x));
        EXPECT_FALSE(std::isnan(vertex.Normal.y));
        EXPECT_FALSE(std::isnan(vertex.Normal.z));
        EXPECT_FALSE(std::isnan(vertex.TexCoords.x));
        EXPECT_FALSE(std::isnan(vertex.TexCoords.y));
    }
}

TEST_F(MeshTest, IndexData) {
    auto mesh = Mesh::Create();
    ASSERT_NE(mesh, nullptr);
    
    bool success = mesh->LoadMeshFromFile("test/meshes/test.obj");
    ASSERT_TRUE(success);
    
    const auto& indices = mesh->GetIndices();
    EXPECT_GT(indices.size(), 0);
    
    // Verifica che gli indici siano validi
    for (const auto& index : indices) {
        EXPECT_LT(index, mesh->GetVertexCount());
    }
}

TEST_F(MeshTest, MaterialHandling) {
    auto mesh = Mesh::Create();
    ASSERT_NE(mesh, nullptr);
    
    bool success = mesh->LoadMeshFromFile("test/meshes/test.obj");
    ASSERT_TRUE(success);
    
    const auto& materials = mesh->GetMaterials();
    EXPECT_GT(materials.size(), 0);
    
    // Verifica che i materiali siano validi
    for (const auto& material : materials) {
        EXPECT_FALSE(material.Name.empty());
        EXPECT_GE(material.DiffuseMap, 0);
        EXPECT_GE(material.SpecularMap, 0);
        EXPECT_GE(material.NormalMap, 0);
    }
}

TEST_F(MeshTest, Drawing) {
    auto mesh = Mesh::Create();
    ASSERT_NE(mesh, nullptr);
    
    bool success = mesh->LoadMeshFromFile("test/meshes/test.obj");
    ASSERT_TRUE(success);
    
    // Verifica che il mesh possa essere disegnato
    EXPECT_NO_THROW(mesh->Draw());
}

TEST_F(MeshTest, ShaderBinding) {
    auto mesh = Mesh::Create();
    ASSERT_NE(mesh, nullptr);
    
    bool success = mesh->LoadMeshFromFile("test/meshes/test.obj");
    ASSERT_TRUE(success);
    
    auto shader = Shader::Create("test/shaders/test.vert", "test/shaders/test.frag");
    ASSERT_NE(shader, nullptr);
    
    // Verifica che il mesh possa essere disegnato con uno shader
    EXPECT_NO_THROW(mesh->Draw(shader));
}

TEST_F(MeshTest, TextureBinding) {
    auto mesh = Mesh::Create();
    ASSERT_NE(mesh, nullptr);
    
    bool success = mesh->LoadMeshFromFile("test/meshes/test.obj");
    ASSERT_TRUE(success);
    
    auto texture = Texture2D::Create(1, 1);
    ASSERT_NE(texture, nullptr);
    
    // Verifica che il mesh possa essere disegnato con una texture
    EXPECT_NO_THROW(mesh->Draw(nullptr, texture));
}

TEST_F(MeshTest, InvalidFile) {
    auto mesh = Mesh::Create();
    ASSERT_NE(mesh, nullptr);
    
    bool success = mesh->LoadMeshFromFile("nonexistent.obj");
    EXPECT_FALSE(success);
    EXPECT_EQ(mesh->GetVertexCount(), 0);
    EXPECT_EQ(mesh->GetIndexCount(), 0);
}

TEST_F(MeshTest, Reload) {
    auto mesh = Mesh::Create();
    ASSERT_NE(mesh, nullptr);
    
    bool success = mesh->LoadMeshFromFile("test/meshes/test.obj");
    ASSERT_TRUE(success);
    
    uint32_t initialVertexCount = mesh->GetVertexCount();
    uint32_t initialIndexCount = mesh->GetIndexCount();
    
    success = mesh->LoadMeshFromFile("test/meshes/test.obj");
    EXPECT_TRUE(success);
    EXPECT_EQ(mesh->GetVertexCount(), initialVertexCount);
    EXPECT_EQ(mesh->GetIndexCount(), initialIndexCount);
}

TEST_F(MeshTest, Cleanup) {
    auto mesh = Mesh::Create();
    ASSERT_NE(mesh, nullptr);
    
    bool success = mesh->LoadMeshFromFile("test/meshes/test.obj");
    ASSERT_TRUE(success);
    
    // Verifica che la pulizia delle risorse funzioni
    EXPECT_NO_THROW(mesh->Cleanup());
    EXPECT_EQ(mesh->GetVertexCount(), 0);
    EXPECT_EQ(mesh->GetIndexCount(), 0);
}
 */