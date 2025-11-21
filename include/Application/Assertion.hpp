/*
 * Project: TestProject
 * File: Assetion.hpp
 * Author: olegfresi
 * Created: 03/03/25 18:47
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
#include <iostream>
#include <csignal>
#include <filesystem>
#include "Logger.hpp"

namespace lux
{
    #define EXPAND_MACRO(x) x
    #define STRINGIFY_MACRO(x) #x
    #define BIT(x) (1 << x)

    #define DEBUGBREAK() raise(SIGTRAP)

    #define CORE_TRACE(...)    Log::GetCoreLogger()->trace(__VA_ARGS__)
    #define CORE_INFO(...)     Log::GetCoreLogger()->info(__VA_ARGS__)
    #define CORE_WARN(...)     Log::GetCoreLogger()->warn(__VA_ARGS__)
    #define CORE_ERROR(...)    Log::GetCoreLogger()->error(__VA_ARGS__)
    #define CORE_CRITICAL(...) Log::GetCoreLogger()->critical(__VA_ARGS__)

    #define TRACE(...)         Log::GetClientLogger()->trace(__VA_ARGS__)
    #define INFO(...)          Log::GetClientLogger()->info(__VA_ARGS__)
    #define WARN(...)          Log::GetClientLogger()->warn(__VA_ARGS__)
    #define ERROR(...)         Log::GetClientLogger()->error(__VA_ARGS__)
    #define CRITICAL(...)      Log::GetClientLogger()->critical(__VA_ARGS__)

        inline void AssertMessage(const char *file, int line)
        {
            ERROR("Assertion Failed at file: {0} Line: {1}", file, line);
        }

        inline void _AssertMessage([[maybe_unused]] const char *file,
                                   [[maybe_unused]] int line, const char *msg)
        {
            ERROR("Assertion Failed: {0}", msg);
        }

        inline void _CoreAssertMessage(const char *file, int line)
        {
            CORE_ERROR("Assertion Failed at file: {0} Line: {1}", file, line);
        }

        inline void _CoreAssertMessage([[maybe_unused]] const char *file,
                                       [[maybe_unused]] int line, const char *msg)
        {
            CORE_ERROR("Assertion Failed: {0}", msg);
        }

    #define INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { HZ##type##ERROR(msg, __VA_ARGS__); DEBUGBREAK(); } }
    #define INTERNAL_ASSERT_WITH_MSG(type, check, ...) INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
    #define INTERNAL_ASSERT_NO_MSG(type, check) INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

    #define INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
    #define INTERNAL_ASSERT_GET_MACRO(...) EXPAND_MACRO( INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, INTERNAL_ASSERT_WITH_MSG, INTERNAL_ASSERT_NO_MSG) )

    //#define ASSERT(x, ...) { if(!(x)) { _AssertMessage(__FILE__, __LINE__, __VA_ARGS__); DEBUGBREAK(); } }
    #define CORE_ASSERT(x, ...) { if(!(x)) {_CoreAssertMessage(__FILE__, __LINE__, __VA_ARGS__); DEBUGBREAK(); static_assert("Core Assertion Failed");} }
}
