#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <../../include/Renderer/Shader/ShaderCompiler.hpp>
/*
class ShaderCompilerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        EXPECT_TRUE(ShaderCompiler::Initialize());
    }

    void TearDown() override
    {
        ShaderCompiler::Finalize();
    }
};

TEST_F(ShaderCompilerTest, Initialization)
{
    EXPECT_TRUE(ShaderCompiler::Initialize());
    ShaderCompiler::Finalize();
}

TEST_F(ShaderCompilerTest, ParseVertexShader)
{
    std::string vertexShader = R"(
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
    
    EXPECT_TRUE(ShaderCompiler::ParseShaderAndPrintAST(vertexShader, EShLangVertex));
}

TEST_F(ShaderCompilerTest, ParseFragmentShader)
{
    std::string fragmentShader = R"(
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
    
    EXPECT_TRUE(ShaderCompiler::ParseShaderAndPrintAST(fragmentShader, EShLangFragment));
}

TEST_F(ShaderCompilerTest, CompileToSPIRV)
{
    std::string vertexShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
    )";
    
    std::vector<uint32_t> spirv = ShaderCompiler::CompileToSPIRV(
        vertexShader,
        EShLangVertex,
        "main"
    );
    
    EXPECT_FALSE(spirv.empty());
}

TEST_F(ShaderCompilerTest, ErrorHandling)
{
    std::string invalidShader = R"(
        #version 330 core
        invalid syntax here
    )";
    
    std::string errorLog;
    std::vector<uint32_t> spirv = ShaderCompiler::CompileToSPIRV(
        invalidShader,
        EShLangVertex,
        "main",
        &errorLog
    );
    
    EXPECT_TRUE(spirv.empty());
    EXPECT_FALSE(errorLog.empty());
}


TEST_F(ShaderCompilerTest, ComplexShaderCompilation)
{
    std::string complexVertexShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec2 aTexCoords;
        layout (location = 3) in vec3 aTangent;
        layout (location = 4) in vec3 aBitangent;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        uniform mat4 normalMatrix;
        
        out vec3 FragPos;
        out vec2 TexCoords;
        out vec3 Normal;
        out vec3 Tangent;
        out vec3 Bitangent;
        
        void main() {
            FragPos = vec3(model * vec4(aPos, 1.0));
            TexCoords = aTexCoords;
            Normal = mat3(normalMatrix) * aNormal;
            Tangent = mat3(normalMatrix) * aTangent;
            Bitangent = mat3(normalMatrix) * aBitangent;
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";
    
    std::vector<uint32_t> spirv = ShaderCompiler::CompileToSPIRV(
        complexVertexShader,
        EShLangVertex,
        "main"
    );
    
    EXPECT_FALSE(spirv.empty());
}

TEST_F(ShaderCompilerTest, ShaderWithIncludes)
{
    std::string vertexShader = R"(
        #version 330 core
        #include "common.glsl"
        
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";
    
    std::vector<uint32_t> spirv = ShaderCompiler::CompileToSPIRV(
        vertexShader,
        EShLangVertex,
        "main"
    );
    
    EXPECT_FALSE(spirv.empty());
}
*/