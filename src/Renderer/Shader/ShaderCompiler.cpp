#include "../../include/Renderer/Shader/ShaderCompiler.hpp"
#include "glslang/SPIRV/GlslangToSpv.h"
#include <iostream>

namespace lux
{
    bool ShaderCompiler::Initialize()
    {
        if (!glslang::InitializeProcess())
            return false;

        return true;
    }

    void ShaderCompiler::Finalize()
    {
        glslang::FinalizeProcess();
    }

    std::vector<uint32_t> ShaderCompiler::CompileToSPIRV(
        const std::string& source,
        EShLanguage stage,
        const std::string& entryPoint,
        std::string* errorLog)
    {
        glslang::TShader shader(stage);
        const char* shaderStrings[1] = { source.c_str() };
        shader.setStrings(shaderStrings, 1);

        TBuiltInResource resources;
        InitResources(resources);

        int defaultVersion = 450;
        EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

        if (!shader.parse(&resources, defaultVersion, false, messages)) {
            if (errorLog) {
                *errorLog = shader.getInfoLog();
            }
            return std::vector<uint32_t>();
        }

        glslang::TProgram program;
        program.addShader(&shader);

        if (!program.link(messages)) {
            if (errorLog) {
                *errorLog = program.getInfoLog();
            }
            return std::vector<uint32_t>();
        }

        std::vector<uint32_t> spirv;
        glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);

        return spirv;
    }


    bool ShaderCompiler::ParseShaderAndPrintAST(const std::string& source, EShLanguage shaderType)
    {
        glslang::InitializeProcess();

        const char* shaderSource = source.c_str();
        glslang::TShader shader(shaderType);
        shader.setStrings(&shaderSource, 1);

        TBuiltInResource Resources = {};
        Resources.maxLights = 32;
        Resources.maxClipPlanes = 6;
        Resources.maxTextureUnits = 32;
        Resources.maxTextureCoords = 32;
        Resources.maxVertexAttribs = 64;
        Resources.maxVertexUniformComponents = 4096;
        Resources.maxVaryingFloats = 64;
        Resources.maxVertexTextureImageUnits = 32;
        Resources.maxCombinedTextureImageUnits = 80;
        Resources.maxTextureImageUnits = 32;
        Resources.maxFragmentUniformComponents = 4096;
        Resources.maxDrawBuffers = 32;
        Resources.maxVertexUniformVectors = 128;
        Resources.maxVaryingVectors = 8;
        Resources.maxFragmentUniformVectors = 16;
        Resources.maxVertexOutputVectors = 16;
        Resources.maxFragmentInputVectors = 15;
        Resources.minProgramTexelOffset = -8;
        Resources.maxProgramTexelOffset = 7;

        int defaultVersion = 410;
        EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules | EShMsgAST);

        if (!shader.parse(&Resources, defaultVersion, false, messages))
        {
            std::cerr << "Error during parsing the shader:\n" << shader.getInfoLog() << std::endl;
            glslang::FinalizeProcess();
            return false;
        }

        PrintAST(shader);

        glslang::TProgram program;
        program.addShader(&shader);

        if (!program.link(EShMsgDefault))
        {
            std::cerr << "Error in linking the program GLSL:\n" << program.getInfoLog() << std::endl;
            glslang::FinalizeProcess();
            return false;
        }

        std::cout << "Info of program:" << std::endl;
        if (const char* infoLog = program.getInfoLog())
            std::cout << infoLog << std::endl;

        if (const char* debugLog = program.getInfoDebugLog())
            std::cout << debugLog << std::endl;

        glslang::FinalizeProcess();
        return true;
    }

    void ShaderCompiler::PrintAST(glslang::TShader& shader)
    {
        if (shader.getIntermediate())
        {
            std::cout << "AST of the shader:" << std::endl;

            const char* infoLog = shader.getInfoLog();
            const char* infoDebugLog = shader.getInfoDebugLog();

            if (infoLog && strlen(infoLog) > 0)
                std::cout << "Info Log:\n" << infoLog << std::endl;

            if (infoDebugLog && strlen(infoDebugLog) > 0)
                std::cout << "Debug Info:\n" << infoDebugLog << std::endl;
        } else
            std::cerr << "AST not available!" << std::endl;
    }

    bool ShaderCompiler::InitResources(TBuiltInResource& resources)
    {
        resources.maxLights = 32;
        resources.maxClipPlanes = 6;
        resources.maxTextureUnits = 32;

        return true;
    }
}