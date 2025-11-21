/*
 * Project: TestProject
 * File: OpenglError.hpp
 * Author: olegfresi
 * Created: 31/01/25 18:22
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
#include "glew/include/GL/glew.h"
#include "../Application/Assertion.hpp"
#include <iostream>
#include <cassert>

namespace lux
{
#define ASSERT(x) if (!(x)) assert(false)
#define GLCheck(x) GLClearError();\
x;\
ASSERT(GLCheckError())

    [[maybe_unused]] inline static void GLClearError()
    {
        while (glGetError() != GL_NO_ERROR);
    }

    [[maybe_unused]] inline static bool GLCheckError()
    {
        while (GLenum error = glGetError())
        {

            std::cout << "[OpenGL Error] ";
            switch(error) {
                case GL_INVALID_ENUM :
                    std::cout << "GL_INVALID_ENUM : An unacceptable value is specified for an enumerated argument.";
                    break;
                case GL_INVALID_VALUE :
                    std::cout << "GL_INVALID_OPERATION : A numeric argument is out of range.";
                    break;
                case GL_INVALID_OPERATION :
                    std::cout << "GL_INVALID_OPERATION : The specified operation is not allowed in the current state.";
                    break;
                case GL_INVALID_FRAMEBUFFER_OPERATION :
                    std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION : The framebuffer object is not complete.";
                    break;
                case GL_OUT_OF_MEMORY :
                    std::cout << "GL_OUT_OF_MEMORY : There is not enough memory left to execute the command.";
                    break;
                case GL_STACK_UNDERFLOW :
                    std::cout << "GL_STACK_UNDERFLOW : An attempt has been made to perform an operation that would cause an internal stack to underflow.";
                    break;
                case GL_STACK_OVERFLOW :
                    std::cout << "GL_STACK_OVERFLOW : An attempt has been made to perform an operation that would cause an internal stack to overflow.";
                    break;
                default :
                    std::cout << "Unrecognized error" << error;
            }
            std::cout << std::endl;
            return false;
        }
        return true;
    }
}