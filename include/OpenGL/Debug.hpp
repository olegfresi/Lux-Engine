/*
 * Project: TestProject
 * File: Debug.hpp
 * Author: olegfresi
 * Created: 10/09/25 16:32
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

GLuint frustumVAO, frustumVBO, frustumEBO;

Vector3f frustumCornersWorld[8];

unsigned int frustumIndices[] =
        {
                0,1, 1,2, 2,3, 3,0,    // near plane
                4,5, 5,6, 6,7, 7,4,    // far plane
                0,4, 1,5, 2,6, 3,7     // sides
        };

void SetupFrustumBuffers()
{
    glGenVertexArrays(1, &frustumVAO);
    glGenBuffers(1, &frustumVBO);
    glGenBuffers(1, &frustumEBO);

    glBindVertexArray(frustumVAO);

    glBindBuffer(GL_ARRAY_BUFFER, frustumVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(frustumCornersWorld), frustumCornersWorld, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, frustumEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(frustumIndices), frustumIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void*)0);

    glBindVertexArray(0);
}

void UpdateFrustumCorners(const Matrix4f& lightProjection, const Matrix4f& lightView)
{
    Matrix4f lightSpaceMatrix = lightProjection * lightView;
    Matrix4f invLightSpace = lightSpaceMatrix.Inverse();

    Vector4f frustumCornersNDC[8] = {
            // Near plane
            Vector4f(-1, -1, -1, 1),
            Vector4f(1, -1, -1, 1),
            Vector4f(1, 1, -1, 1),
            Vector4f(-1, 1, -1, 1),
            // Far plane
            Vector4f(-1, -1, 1, 1),
            Vector4f(1, -1, 1, 1),
            Vector4f(1, 1, 1, 1),
            Vector4f(-1, 1, 1, 1)
    };

    for (int i = 0; i < 8; ++i)
    {
        Vector4f world = invLightSpace * frustumCornersNDC[i];
        if (world.GetW() != 0.0f)
            world /= world.GetW();
        frustumCornersWorld[i] = Vector3f(world.GetX(), world.GetY(), world.GetZ());
    }

    glBindBuffer(GL_ARRAY_BUFFER, frustumVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(frustumCornersWorld), frustumCornersWorld);
}

void DrawFrustum(const Matrix4f& view, const Matrix4f & projection,  const lux::Shader& shader)
{
    shader.Bind();

    glUniformMatrix4fv(glGetUniformLocation(shader.GetId(), "uView"), 1, GL_FALSE, view.Data());
    glUniformMatrix4fv(glGetUniformLocation(shader.GetId(), "uProjection"), 1, GL_FALSE, projection.Data());

    glBindVertexArray(frustumVAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
