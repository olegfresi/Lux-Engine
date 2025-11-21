/*
 * Project: TestProject
 * File: EngineSettings.hpp
 * Author: olegfresi
 * Created: 13/02/25 19:11
 * 
 * Copyright © 2025 olegfresi
 * 
 * Licensed under the MIT License. You may obtain a copy of the License at:
 * 
 *     https://opensource.org/licenses/MIT
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once
#include <cstdint>
#include <iostream>
namespace lux
{
    enum class GraphicsAPI : uint8_t
    {
        OPENGL,
        VULKAN,
        DIRECTX,
        METAL
    };

    enum class Os : uint8_t
    {
        WINDOWS,
        LINUX,
        MACOS
    };

    enum class DrawMode : uint8_t
    {
        _2D,
        _3D
    };

    static const int WindowSamples = 4;
    static const int TextureUnits = 32;

    static constexpr bool IsBigEndian = []
    {
#ifdef IS_BIG_ENDIAN
        return true;
#else
        return false;
#endif
    }();

    static constexpr GraphicsAPI API = []
    {
#ifdef GRAPHICS_API_OPENGL
        return GraphicsAPI::OPENGL;
#elif defined(GRAPHICS_API_VULKAN)
        return GraphicsAPI::VULKAN;
#elif defined(GRAPHICS_API_DIRECTX)
        return GraphicsAPI::DIRECTX;
#elif defined(GRAPHICS_API_METAL)
        return GraphicsAPI::METAL;
#else
        CORE_ASSERT(false, "GRAPHICS_API not defined!");
#endif
    }();


    static constexpr Os OperatingSystem = []
    {
#ifdef OS_TYPE_WINDOWS
        return Os::WINDOWS;
#elif defined(OS_TYPE_LINUX)
        return Os::LINUX;
#elif defined(OS_TYPE_MACOS)
        return Os::MACOS;
#else
        CORE_ASSERT(false, "OS_TYPE not defined!");
#endif
    }();


    inline void PrintConfig()
    {
        std::cout << "Os: ";
        switch (OperatingSystem)
        {
            case Os::WINDOWS: std::cout << "Windows"; break;
            case Os::LINUX: std::cout << "Linux"; break;
            case Os::MACOS: std::cout << "macOS"; break;
            default: std::cout << "Unknown Os"; break;
        }
        std::cout << std::endl;

        std::cout << "Graphics API: ";
        switch (API)
        {
            case GraphicsAPI::OPENGL: std::cout << "OpenGL"; break;
            case GraphicsAPI::VULKAN: std::cout << "Vulkan"; break;
            case GraphicsAPI::DIRECTX: std::cout << "DirectX"; break;
            case GraphicsAPI::METAL: std::cout << "Metal"; break;
            default: std::cout << "Unknown API"; break;
        }
        std::cout << std::endl;
    }
}