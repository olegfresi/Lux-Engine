#pragma once
#include <string>
#include <vector>
#include "glslang/glslang/Public/ShaderLang.h"
#include "glslang/glslang/Include/intermediate.h"
#include "glslang/glslang/Include/ResourceLimits.h"
#include <iostream>
extern TBuiltInResource DefaultTBuiltInResource;

namespace lux
{
    class ShaderCompiler
    {
    public:
        static bool Initialize();
        static void Finalize();

        static bool ParseShaderAndPrintAST(const std::string& source, EShLanguage shaderType);
        static void PrintAST(glslang::TShader& shader);

        static std::vector<uint32_t> CompileToSPIRV(
            const std::string& source,
            EShLanguage stage,
            const std::string& entryPoint = "main",
            std::string* errorLog = nullptr
        );

    private:
        static bool InitResources(TBuiltInResource& resources);
        static EShLanguage GetShaderStage(const std::string& extension) {return EShLangVertex;}
    };

    inline bool compileGLSL(const std::string& source, EShLanguage stage, glslang::TProgram& program)
    {
        const char* shaderStrings[1];
        shaderStrings[0] = source.c_str();

        glslang::TShader shader(stage);
        shader.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientOpenGL, 100);
        shader.setEnvClient(glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450);
        shader.setEnvTarget(glslang::EShTargetNone, glslang::EShTargetSpv_1_0);
        shader.setStrings(shaderStrings, 1);
        shader.setAutoMapLocations(false);

        if (!shader.parse(&DefaultTBuiltInResource, 410, false, EShMsgDefault))
        {
            std::cerr << "GLSL Parsing Failed:\n" << shader.getInfoLog() << std::endl;
            return false;
        }

        program.addShader(&shader);
        if (!program.link(EShMsgDefault))
        {
            std::cerr << "Program Linking Failed:\n" << program.getInfoLog() << std::endl;
            return false;
        }

        return true;
    }

    struct ShaderVariable
    {
        std::string name;
        std::string type;
        int location = -1;
        int binding = -1;
        int set = -1;
        int size = 0;
        bool isArray = false;
        int arraySize = 0;
        int offset = 0;
        std::vector<ShaderVariable> members;
    };

    struct ShaderInput
    {
        std::string name;
        std::string type;
        int location;
        int size;
        int offset;
    };

    struct ShaderOutput
    {
        std::string name;
        std::string type;
        int location;
        int size;
        int offset;
    };

    struct ShaderUniform
    {
        std::string name;
        std::string type;
        int binding;
        int set;
        int size;
        bool isArray;
        int arraySize;
        int offset;
        std::vector<ShaderVariable> members;
    };

    struct ShaderUniformBlock
    {
        std::string name;
        int binding;
        int set;
        int size;
        std::vector<ShaderVariable> members;
    };

    struct ShaderPushConstant
    {
        std::string name;
        int size;
        std::vector<ShaderVariable> members;
    };

    struct ShaderReflectionData
    {
        std::vector<ShaderInput> inputs;
        std::vector<ShaderOutput> outputs;
        std::vector<ShaderUniform> uniforms;
        std::vector<ShaderUniformBlock> uniformBlocks;
        std::vector<ShaderPushConstant> pushConstants;
        std::unordered_map<std::string, ShaderVariable> variables;
    };

    class ReflectionVisitor : public glslang::TIntermTraverser
    {
    public:
        ShaderReflectionData data;

        ReflectionVisitor() : TIntermTraverser(true, true, true) {}

        void visitSymbol(glslang::TIntermSymbol* symbol) override
        {
            const glslang::TType& type = symbol->getType();
            const glslang::TQualifier& qualifier = type.getQualifier();

            ShaderVariable var;
            var.name = symbol->getName().c_str();
            var.type = GetTypeString(type);
            var.size = type.computeNumComponents();
            var.isArray = type.isArray();
            if (var.isArray)
                var.arraySize = type.getArraySizes()->getDimSize(0);

            if (data.variables.contains(var.name))
                return;

            if (qualifier.isUniformOrBuffer()) {
                if (type.getBasicType() == glslang::EbtBlock)
                {
                    std::string uniqueName = var.name;
                    if (uniqueName.find("anon@") != std::string::npos)
                        uniqueName = "anon_" + std::to_string(data.uniformBlocks.size());

                    ShaderUniformBlock block;
                    block.name = uniqueName;
                    block.binding = qualifier.layoutBinding;
                    block.set = qualifier.layoutSet;
                    block.size = type.computeNumComponents();
                    data.uniformBlocks.push_back(block);
                }
                else
                {
                    ShaderUniform uniform;
                    uniform.name = var.name;
                    uniform.type = var.type;
                    uniform.binding = qualifier.layoutBinding;
                    uniform.set = qualifier.layoutSet;
                    uniform.size = var.size;
                    uniform.isArray = var.isArray;
                    uniform.arraySize = var.arraySize;
                    uniform.offset = GetUniformOffset(var.name);
                    data.uniforms.push_back(uniform);
                }
            }
            else if (qualifier.isPipeInput())
            {
                ShaderInput input;
                input.name = var.name;
                input.type = var.type;
                input.location = qualifier.layoutLocation;
                input.size = var.size;
                input.offset = 0;
                data.inputs.push_back(input);
            }
            else if (qualifier.isPipeOutput())
            {
                ShaderOutput output;
                output.name = var.name;
                output.type = var.type;
                output.location = qualifier.layoutLocation;
                output.size = var.size;
                output.offset = 0;
                data.outputs.push_back(output);
            }
            else if (qualifier.isPushConstant())
            {
                ShaderPushConstant push;
                push.name = var.name;
                push.size = type.computeNumComponents();
                data.pushConstants.push_back(push);
            }

            data.variables[var.name] = var;
        }

    private:
        static std::string GetTypeString(const glslang::TType& type)
        {
            switch (type.getBasicType())
            {
                case glslang::EbtVoid: return "void";
                case glslang::EbtFloat: return "float";
                case glslang::EbtDouble: return "double";
                case glslang::EbtInt: return "int";
                case glslang::EbtUint: return "uint";
                case glslang::EbtBool: return "bool";
                case glslang::EbtSampler: return "sampler";
                case glslang::EbtStruct: return "struct";
                case glslang::EbtBlock: return "block";
                default: return "unknown";
            }
        }

        static int GetUniformOffset(const std::string& uniformName)
        {
            static std::unordered_map<std::string, int> offsets;
            if (offsets.contains(uniformName))
                offsets[uniformName] = offsets.size() * 16;

            return offsets[uniformName];
        }
    };

    inline void PrintShaderReflection(const ShaderReflectionData& reflection)
    {
        std::cout << "\nShader Reflection Data:\n";
        std::cout << "=====================\n\n";

        if (!reflection.inputs.empty())
        {
            std::cout << "Inputs:\n";
            for (const auto& input : reflection.inputs)
                std::cout << "  " << input.name << " (location: " << input.location
                          << ", type: " << input.type << ", size: " << input.size
                          << ", offset: " << input.offset << ")\n";

            std::cout << "\n";
        }

        if (!reflection.outputs.empty())
        {
            std::cout << "Outputs:\n";
            for (const auto& output : reflection.outputs)
                std::cout << "  " << output.name << " (location: " << output.location
                          << ", type: " << output.type << ", size: " << output.size
                          << ", offset: " << output.offset << ")\n";

            std::cout << "\n";
        }

        if (!reflection.uniforms.empty())
        {
            std::cout << "Uniforms:\n";
            for (const auto& uniform : reflection.uniforms)
            {
                std::cout << "  " << uniform.name << " (binding: " << uniform.binding
                          << ", set: " << uniform.set << ", type: " << uniform.type
                          << ", size: " << uniform.size << ", offset: " << uniform.offset;
                if (uniform.isArray)
                    std::cout << ", array size: " << uniform.arraySize;

                std::cout << ")\n";
            }
            std::cout << "\n";
        }

        if (!reflection.uniformBlocks.empty())
        {
            std::cout << "Uniform Blocks:\n";
            for (const auto& block : reflection.uniformBlocks)
                std::cout << "  " << block.name << " (binding: " << block.binding
                          << ", set: " << block.set << ", size: " << block.size << ")\n";

            std::cout << "\n";
        }

        if (!reflection.pushConstants.empty())
        {
            std::cout << "Push Constants:\n";
            for (const auto& push : reflection.pushConstants)
                std::cout << "  " << push.name << " (size: " << push.size << ")\n";

            std::cout << "\n";
        }
    }

    inline bool CompileGLSL(const std::string& source, EShLanguage stage, glslang::TProgram& program)
    {
        glslang::TShader shader(stage);
        shader.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientOpenGL, 100);
        shader.setEnvClient(glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450);
        shader.setEnvTarget(glslang::EShTargetNone, glslang::EShTargetSpv_1_0);
        shader.setPreamble("#define LUX_FORCE_EXPLICIT_LAYOUT 1\n");
        const char* shaderStrings[1] = { source.c_str() };
        shader.setStrings(shaderStrings, 1);
        shader.setStrings(shaderStrings, 1);

        TBuiltInResource resources = {};
        resources.maxLights = 32;
        resources.maxClipPlanes = 6;
        resources.maxTextureUnits = 32;
        resources.maxTextureCoords = 32;
        resources.maxVertexAttribs = 64;
        resources.maxVertexUniformComponents = 4096;
        resources.maxVaryingFloats = 64;
        resources.maxVertexTextureImageUnits = 32;
        resources.maxCombinedTextureImageUnits = 80;
        resources.maxTextureImageUnits = 32;
        resources.maxFragmentUniformComponents = 4096;
        resources.maxDrawBuffers = 32;
        resources.maxVertexUniformVectors = 128;
        resources.maxVaryingVectors = 8;
        resources.maxFragmentUniformVectors = 16;
        resources.maxVertexOutputVectors = 16;
        resources.maxFragmentInputVectors = 15;
        resources.minProgramTexelOffset = -8;
        resources.maxProgramTexelOffset = 7;

        int defaultVersion = 410;
        EShMessages messages = EShMsgDefault;

        if (!shader.parse(&resources, defaultVersion, false, messages))
        {
            std::cerr << "Shader parsing failed:\n" << shader.getInfoLog() << std::endl;
            return false;
        }

        program.addShader(&shader);

        if (!program.link(messages))
        {
            std::cerr << "Shader linking failed:\n" << program.getInfoLog() << std::endl;
            return false;
        }

        return true;
    }
}