/*
 * Project: TestProject
 * File: CameraController.hpp
 * Author: olegfresi
 * Created: 17/03/25 20:52
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
#include "Camera.hpp"
#include "../../Input/ActionMapper.hpp"
#include "../../Event/Events.hpp"
#include "../../Event/EventSystem.hpp"

namespace lux
{

    using namespace event;

    enum class MoveDirection : uint8_t
    {
        Forward = 0,
        Backward,
        Right,
        Left = 3
    };

    class CameraController
    {
    public:

        CameraController(Camera& camera, ActionMapper& mapper, NonOwnPtr<EventDispatcher> dispatcher);

        void SetCameraTranslationSpeed(float speed) { m_cameraTranslationSpeed = speed; }
        void SetCameraRotationSpeed(float speed) { m_cameraRotationSpeed = speed; }
        void SetCameraZoomSpeed(float speed) { m_cameraZoomSpeed = speed; }
        void SetCameraPanSpeed(float speed) { m_cameraPanSpeed = speed; }

        void Move(MoveDirection direction, double deltaTime) noexcept;
        void MoveAround(const ActionEvent& action, bool first) noexcept;
        void MoveUpDown(const ActionEvent& action, bool first) const noexcept;
        void Zoom(const ActionEvent& action) noexcept;
        void Pan(float deltaX, float deltaY) noexcept;

        void GamepadLook(float rightX, float rightY, double deltaTime) const noexcept;
        void GamepadMove(float leftX,  float leftY,  double deltaTime) noexcept;
        void GamepadZoom(float triggerValue, double deltaTime) noexcept;

        Camera& GetCamera() { return m_camera; }
        const Camera& GetCamera() const { return m_camera; }

        void UpdateCamera(double deltaTime) noexcept;

        void OnAction(const ActionEvent& action) noexcept;
        void OnMouseDrag(const MouseDragEvent& e) const noexcept;
        void OnMouseMotion(const MouseMotionEvent& e) noexcept;
        void OnMouseScroll(const MouseScrollEvent& e) noexcept;

    private:

        Vector3f m_targetPosition = {0.0f, 0.0f, 0.0f};
        float m_orbitRadius = 5.0f;

        double m_lastMouseX = 0.0;
        double m_lastMouseY = 0.0;

        ActionMapper& m_actionMapper;
        Camera& m_camera;
        float m_cameraTranslationSpeed = 0.05f;
        float m_cameraRotationSpeed = 180.0f;
        float m_cameraZoomSpeed = 0.25f;
        float m_cameraPanSpeed = 0.5f;
        float m_gamepadSpeedMultiplier = 20.0f;
        uint32_t m_listenerId;
        std::bitset<static_cast<size_t>(InputAction::ActionLast)> m_activeActions;
        NonOwnPtr<EventDispatcher> m_dispatcher;
    };
}