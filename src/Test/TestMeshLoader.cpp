#include <gtest/gtest.h>
/*
#include <Renderer/Mesh/MeshLoader.hpp>
#include <Renderer/Mesh/MeshParsers/ObjParser.hpp>
#include <filesystem>

class MeshLoaderTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Assicurati che la directory dei test esista
        std::filesystem::create_directories("test/meshes");
    }

    void TearDown() override {
        // Pulisci i file di test se necessario
    }
};

// Test per la struttura MeshData
TEST_F(MeshLoaderTest, MeshDataStructure) {
    MeshData data;
    EXPECT_TRUE(data.indices.empty());
    EXPECT_TRUE(data.vertices.empty());
}

// Test per il parser OBJ
TEST_F(MeshLoaderTest, ObjParserCreation) {
    OBJParser parser;
    EXPECT_NO_THROW(parser.ParseMesh("test/meshes/test.obj"));
}

TEST_F(MeshLoaderTest, ObjParserVertexCount) {
    OBJParser parser;
    auto meshData = parser.ParseMesh("test/meshes/test.obj");
    EXPECT_EQ(meshData.vertices.size(), 8 * 11); // 8 vertici * 11 componenti per vertice
}

TEST_F(MeshLoaderTest, ObjParserIndexCount) {
    OBJParser parser;
    auto meshData = parser.ParseMesh("test/meshes/test.obj");
    EXPECT_EQ(meshData.indices.size(), 6 * 4); // 6 facce * 4 vertici per faccia
}

// Test per la funzione ReadVec
TEST_F(MeshLoaderTest, ReadVec) {
    std::vector<std::string> words = {"v", "1.0", "2.0", "3.0"};
    auto vec = ReadVec<Vector3f>(words);
    EXPECT_FLOAT_EQ(vec[0], 1.0f);
    EXPECT_FLOAT_EQ(vec[1], 2.0f);
    EXPECT_FLOAT_EQ(vec[2], 3.0f);
}

// Test per la struttura NewVertex
TEST_F(MeshLoaderTest, NewVertexStructure) {
    NewVertex vertex;
    vertex.position = Vector3f(1.0f, 2.0f, 3.0f);
    vertex.color = Vector3f(0.5f, 0.5f, 0.5f);
    vertex.texCoord = Vector2f(0.0f, 1.0f);
    vertex.normal = Vector3f(0.0f, 1.0f, 0.0f);

    EXPECT_FLOAT_EQ(vertex.position.GetX(), 1.0f);
    EXPECT_FLOAT_EQ(vertex.color.GetX(), 0.5f);
    EXPECT_FLOAT_EQ(vertex.texCoord.GetX(), 0.0f);
    EXPECT_FLOAT_EQ(vertex.normal.GetX(), 0.0f);
}

// Test per la conversione VertexVectorToFloatVector
TEST_F(MeshLoaderTest, VertexVectorToFloatVector) {
    std::vector<NewVertex> vertices;
    NewVertex vertex;
    vertex.position = Vector3f(1.0f, 2.0f, 3.0f);
    vertex.color = Vector3f(0.5f, 0.5f, 0.5f);
    vertex.texCoord = Vector2f(0.0f, 1.0f);
    vertex.normal = Vector3f(0.0f, 1.0f, 0.0f);
    vertices.push_back(vertex);

    auto floatVector = VertexVectorToFloatVector(vertices);
    EXPECT_EQ(floatVector.size(), 11); // 11 componenti per vertice
    EXPECT_FLOAT_EQ(floatVector[0], 1.0f);
    EXPECT_FLOAT_EQ(floatVector[3], 0.5f);
    EXPECT_FLOAT_EQ(floatVector[6], 0.0f);
    EXPECT_FLOAT_EQ(floatVector[8], 0.0f);
}

// Test per la struttura Material
TEST_F(MeshLoaderTest, MaterialStructure) {
    Material material;
    material.ambientColor = Vector3f(0.2f, 0.2f, 0.2f);
    material.diffuseColor = Vector3f(0.8f, 0.8f, 0.8f);
    material.specularColor = Vector3f(1.0f, 1.0f, 1.0f);
    material.shininess = 32.0f;
    material.diffuseTexture = "texture.png";

    EXPECT_FLOAT_EQ(material.ambientColor.GetX(), 0.2f);
    EXPECT_FLOAT_EQ(material.diffuseColor.GetX(), 0.8f);
    EXPECT_FLOAT_EQ(material.specularColor.GetX(), 1.0f);
    EXPECT_FLOAT_EQ(material.shininess, 32.0f);
    EXPECT_EQ(material.diffuseTexture, "texture.png");
}

// Test per il caricamento dei materiali
TEST_F(MeshLoaderTest, LoadMtl) {
    // Crea un file MTL di test
    std::ofstream mtlFile("test/meshes/test.mtl");
    mtlFile << "newmtl material1\n"
            << "Ka 0.2 0.2 0.2\n"
            << "Kd 0.8 0.8 0.8\n"
            << "Ks 1.0 1.0 1.0\n"
            << "Ns 32.0\n"
            << "map_Kd texture.png\n";
    mtlFile.close();

    auto materials = LoadMtl("test/meshes/test.mtl");
    EXPECT_EQ(materials.size(), 1);
    EXPECT_FLOAT_EQ(materials["material1"].ambientColor.GetX(), 0.2f);
    EXPECT_FLOAT_EQ(materials["material1"].diffuseColor.GetX(), 0.8f);
    EXPECT_FLOAT_EQ(materials["material1"].specularColor.GetX(), 1.0f);
    EXPECT_FLOAT_EQ(materials["material1"].shininess, 32.0f);
    EXPECT_EQ(materials["material1"].diffuseTexture, "texture.png");

    // Pulisci il file di test
    std::filesystem::remove("test/meshes/test.mtl");
}

// Test per il caricamento di file MTL non esistenti
TEST_F(MeshLoaderTest, LoadMtlNonExistent) {
    EXPECT_THROW(LoadMtl("nonexistent.mtl"), std::runtime_error);
}

// Test per il parsing di file OBJ con materiali
TEST_F(MeshLoaderTest, ObjParserWithMaterials) {
    // Crea un file OBJ con riferimento a materiali
    std::ofstream objFile("test/meshes/test_with_mtl.obj");
    objFile << "mtllib test.mtl\n"
            << "usemtl material1\n"
            << "v -1.0 -1.0 -1.0\n"
            << "v  1.0 -1.0 -1.0\n"
            << "v  1.0  1.0 -1.0\n"
            << "v -1.0  1.0 -1.0\n"
            << "f 1 2 3 4\n";
    objFile.close();

    // Crea il file MTL corrispondente
    std::ofstream mtlFile("test/meshes/test.mtl");
    mtlFile << "newmtl material1\n"
            << "Ka 0.2 0.2 0.2\n"
            << "Kd 0.8 0.8 0.8\n"
            << "Ks 1.0 1.0 1.0\n"
            << "Ns 32.0\n"
            << "map_Kd texture.png\n";
    mtlFile.close();

    OBJParser parser;
    auto meshData = parser.ParseMesh("test/meshes/test_with_mtl.obj");
    EXPECT_GT(meshData.vertices.size(), 0);
    EXPECT_GT(meshData.indices.size(), 0);

    // Pulisci i file di test
    std::filesystem::remove("test/meshes/test_with_mtl.obj");
    std::filesystem::remove("test/meshes/test.mtl");
}
 */