/*
 * Project: TestProject
 * File: Camera.hpp
 * Author: olegfresi
 * Created: 09/02/25 16:59
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
#include "../../Math/Matrix.hpp"
#include "../../Math/Vector.hpp"

namespace lux
{
    using namespace math;

    class Camera
    {
    public:
        Camera();
        Camera(Vector3f pos, Vector3f front, Vector3f up, float aspectRatio, float fov, float yaw, float pitch, float near = 1.0f, float far = 1000.0f) noexcept;

        [[nodiscard]] const auto& GetViewToScreen() const noexcept { return m_viewToScreen; }
        [[nodiscard]] const auto& GetView() const noexcept { return m_view; }
        [[nodiscard]] const auto& GetProjection() const noexcept { return m_projection; }
        [[nodiscard]] const auto& GetTransform() const noexcept { return m_transform; }
        [[nodiscard]] const auto& GetPosition() const noexcept { return m_pos; }
        [[nodiscard]] const auto& GetFront() const noexcept { return m_front; }
        [[nodiscard]] const auto& GetUp() const noexcept { return m_up; }
        [[nodiscard]] const auto& GetRight() const noexcept { return m_right; }
        [[nodiscard]] const auto& GetNear() const noexcept { return m_near; }
        [[nodiscard]] const auto& GetFar() const noexcept { return m_far; }
        [[nodiscard]] const auto& GetFov() const noexcept { return m_fov; }
        [[nodiscard]] const auto& GetAspectRatio() const noexcept { return m_aspectRatio; }
        [[nodiscard]] const auto& GetPitch() const noexcept { return m_pitch; }
        [[nodiscard]] const auto& GetYaw() const noexcept { return m_yaw; }


        void SetPosition(const Vector3f& position) noexcept;
        void SetFront(const Vector3f& front) noexcept;
        void SetUp(const Vector3f& up) noexcept;
        void SetRight(const Vector3f& right) noexcept;
        void SetFov(float fov) noexcept;
        void SetTransform(const Matrix4f& transform) noexcept;
        void SetYaw(float yaw) noexcept;
        void SetPitch(float pitch) noexcept;
        void SetAspectRatio(float aspectRatio) noexcept;
        void SetNear(float near) noexcept;
        void SetFar(float far) noexcept;

        void UpdateView() noexcept;
        void UpdateProjection() noexcept;

        void ZoomIn(float amount) noexcept;
        void ZoomOut(float amount) noexcept;

    private:
        Vector3f m_pos;
        Vector3f m_front;
        Vector3f m_up;
        Vector3f m_right;

        Matrix4f m_projection;
        Matrix4f m_view;
        Matrix4f m_viewToScreen;
        Matrix4f m_transform;

        float m_pitch;
        float m_yaw;

        float m_fov {};
        float m_near{1.0f};
        float m_far {100.0f};
        float m_aspectRatio{};
    };
}