#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <../../include/Renderer/Shader/Shader.hpp>

/*
class ShaderTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        std::ofstream vertexFile("test_vertex.glsl");
        vertexFile << R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aNormal;
            layout (location = 2) in vec2 aTexCoords;
            
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;
            
            out vec3 Normal;
            out vec2 TexCoords;
            
            void main() {
                gl_Position = projection * view * model * vec4(aPos, 1.0);
                Normal = mat3(transpose(inverse(model))) * aNormal;
                TexCoords = aTexCoords;
            }
        )";
        vertexFile.close();

        std::ofstream fragmentFile("test_fragment.glsl");
        fragmentFile << R"(
            #version 330 core
            in vec3 Normal;
            in vec2 TexCoords;
            
            out vec4 FragColor;
            
            uniform sampler2D texture1;
            uniform vec3 lightPos;
            uniform vec3 viewPos;
            
            void main() {
                vec3 norm = normalize(Normal);
                vec3 lightDir = normalize(lightPos - FragPos);
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = diff * vec3(1.0);
                vec4 texColor = texture(texture1, TexCoords);
                FragColor = vec4(diffuse, 1.0) * texColor;
            }
        )";
        fragmentFile.close();
    }

    void TearDown() override
    {
        std::filesystem::remove("test_vertex.glsl");
        std::filesystem::remove("test_fragment.glsl");
    }
};

TEST_F(ShaderTest, Creation)
{
    Shader shader("test_vertex.glsl", "test_fragment.glsl");
    EXPECT_NE(shader.GetId(), 0);
}

TEST_F(ShaderTest, Binding)
{
    Shader shader("test_vertex.glsl", "test_fragment.glsl");
    
    shader.Bind();
    EXPECT_TRUE(shader.IsBound());
    
    shader.Unbind();
    EXPECT_FALSE(shader.IsBound());
}

TEST_F(ShaderTest, Uniforms)
{
    Shader shader("test_vertex.glsl", "test_fragment.glsl");
    
    shader.SetUniform1f("lightPos.x", 1.0f);
    shader.SetUniform1f("lightPos.y", 2.0f);
    shader.SetUniform1f("lightPos.z", 3.0f);
    
    shader.SetUniform1i("texture1", 0);
    
    float lightPos[3] = {1.0f, 2.0f, 3.0f};
    shader.SetUniform1fv("lightPos", lightPos, 3);
    
    shader.SetUniform4f("color", 1.0f, 0.5f, 0.2f, 1.0f);
    
    math::Matrix4f model = math::Identity4f;
    shader.SetMat3f("model", model);
    shader.SetUniformMat4f("model", model, false);
    
    EXPECT_GE(shader.GetUniformLocation("model"), 0);
    EXPECT_GE(shader.GetUniformLocation("view"), 0);
    EXPECT_GE(shader.GetUniformLocation("projection"), 0);
}

TEST_F(ShaderTest, InvalidShader)
{
    EXPECT_THROW(Shader shader("nonexistent.vert", "nonexistent.frag"), std::runtime_error);
}

TEST_F(ShaderTest, Preprocessing)
{
    Shader shader("test_vertex.glsl", "test_fragment.glsl");
    EXPECT_NO_THROW(shader.PreprocessShader());
}

TEST_F(ShaderTest, MultipleUniforms)
{
    Shader shader("test_vertex.glsl", "test_fragment.glsl");
    
    shader.SetUniform1f("lightPos.x", 1.0f);
    shader.SetUniform1f("lightPos.y", 2.0f);
    shader.SetUniform1f("lightPos.z", 3.0f);
    
    math::Matrix4f view = math::Identity4f;
    math::Matrix4f projection = math::Identity4f;
    
    shader.SetUniformMat4f("view", view, false);
    shader.SetUniformMat4f("projection", projection, false);
    
    EXPECT_GE(shader.GetUniformLocation("lightPos"), 0);
    EXPECT_GE(shader.GetUniformLocation("view"), 0);
    EXPECT_GE(shader.GetUniformLocation("projection"), 0);
}

TEST_F(ShaderTest, TextureUniforms)
{
    Shader shader("test_vertex.glsl", "test_fragment.glsl");
    
    shader.SetUniform1i("texture1", 0);
    
    EXPECT_GE(shader.GetUniformLocation("texture1"), 0);
}
 */