#include "../../include/Renderer/Camera/CameraController.hpp"
#include "../../include/Input/Keyboard.hpp"
#include "../../include/Input/Gamepad.hpp"

namespace lux
{
    static int lastX = 400;
    static int lastY = 300;
    static constexpr float velocity = 10.0f;

    CameraController::CameraController(Camera& camera, ActionMapper& mapper, NonOwnPtr<EventDispatcher> dispatcher) : m_actionMapper{mapper}, m_camera(camera), m_listenerId{0}, m_dispatcher{dispatcher}
    {
        m_dispatcher->RegisterListener<ActionEvent>(m_listenerId,
                                                    [this](const ActionEvent& e) { OnAction(e); }
        );

        m_dispatcher->RegisterListener<MouseDragEvent>(m_listenerId,
                                                       [this](const MouseDragEvent& e) { OnMouseDrag(e); }
        );

        m_dispatcher->RegisterListener<MouseMotionEvent>(m_listenerId,
                                                         [this](const MouseMotionEvent& e) { OnMouseMotion(e); }
        );

        m_dispatcher->RegisterListener<MouseScrollEvent>(m_listenerId,
                                                         [this](const MouseScrollEvent& e) { OnMouseScroll(e); }
        );

        m_dispatcher->RegisterListener<GamepadLookEvent>(m_listenerId,
                                                         [this](const GamepadLookEvent& e) {
                                                             GamepadLook(e.m_rightX, e.m_rightY, e.m_deltaTime);
                                                         }
        );

        m_dispatcher->RegisterListener<GamepadMoveEvent>(m_listenerId,
                                                         [this](const GamepadMoveEvent& e) {
                                                             GamepadMove(e.m_leftX, e.m_leftY, e.m_deltaTime);
                                                         }
        );

        m_dispatcher->RegisterListener<GamepadZoomEvent>(m_listenerId,
                                                         [this](const GamepadZoomEvent& e) {
                                                             GamepadZoom(e.m_triggerValue, e.m_deltaTime);
                                                         }
        );

        CORE_INFO("Created Camera Controller");
    }

    void CameraController::UpdateCamera(double deltaTime) noexcept
    {
        if (InputDevice::IsKeyDown(InputType::Keyboard, m_actionMapper.GetKeyCombo(InputAction::MoveForward, InputType::Keyboard).key))
        {
            if (InputDevice::IsKeyDown( InputType::Keyboard, static_cast<int>(KeyboardKeys::KeyLeftShift)))
                Move(MoveDirection::Forward, deltaTime * velocity);
            else
                Move(MoveDirection::Forward, deltaTime);
        }

        if (InputDevice::IsKeyDown(InputType::Keyboard, m_actionMapper.GetKeyCombo(InputAction::MoveBackward, InputType::Keyboard).key))
        {
            if (InputDevice::IsKeyDown( InputType::Keyboard, static_cast<int>(KeyboardKeys::KeyLeftShift)))
                Move(MoveDirection::Backward, deltaTime * velocity);
            else
                Move(MoveDirection::Backward, deltaTime);
        }

        if (InputDevice::IsKeyDown(InputType::Keyboard, m_actionMapper.GetKeyCombo(InputAction::MoveLeft, InputType::Keyboard).key))
        {
            if (InputDevice::IsKeyDown( InputType::Keyboard, static_cast<int>(KeyboardKeys::KeyLeftShift)))
                Move(MoveDirection::Left, deltaTime * velocity);
            else
                Move(MoveDirection::Left, deltaTime);
        }

        if (InputDevice::IsKeyDown(InputType::Keyboard, m_actionMapper.GetKeyCombo(InputAction::MoveRight, InputType::Keyboard).key))
        {
            if (InputDevice::IsKeyDown( InputType::Keyboard, static_cast<int>(KeyboardKeys::KeyLeftShift)))
                Move(MoveDirection::Right, deltaTime * velocity);
            else
                Move(MoveDirection::Right, deltaTime);
        }

        m_camera.UpdateView();
    }

    void CameraController::OnAction(const ActionEvent& action) noexcept
    {
        float deltaTime = 0.001f;

        switch (action.m_action)
        {
            case InputAction::MoveForward:
                Move(MoveDirection::Forward, deltaTime);
                break;

            case InputAction::MoveBackward:
                Move(MoveDirection::Backward, deltaTime);
                break;

            case InputAction::MoveLeft:
                Move(MoveDirection::Left, deltaTime);
                break;

            case InputAction::MoveRight:
                Move(MoveDirection::Right, deltaTime);
                break;

            case InputAction::MoveAround:
                MoveAround(action, true);
                break;

            case InputAction::MoveUpDown:
                MoveUpDown(action, true);
                break;

            case InputAction::Zoom:
                Zoom(action);
                break;

            case InputAction::ActionLast:
                break;

            case InputAction::Pan:
                break;

            default:
                assert(false);
        }

        m_camera.UpdateView();
    }

    void CameraController::Move(MoveDirection direction, double deltaTime) noexcept
    {
        auto temp = Cross(m_camera.GetFront(), m_camera.GetUp());
        temp.Normalize();

        float speed = m_cameraTranslationSpeed * deltaTime * 200.0f;

        Vector3f originalFront = m_camera.GetFront();
        Vector3f right = Cross(originalFront, m_camera.GetUp()).Normalize();

        switch (direction)
        {
            case MoveDirection::Forward:
                m_camera.SetPosition(m_camera.GetPosition() + originalFront * speed);
                break;

            case MoveDirection::Backward:
                m_camera.SetPosition(m_camera.GetPosition() - originalFront * speed);
                break;

            case MoveDirection::Left:
                m_camera.SetPosition(m_camera.GetPosition() - right * speed);
                break;

            case MoveDirection::Right:
                m_camera.SetPosition(m_camera.GetPosition() + right * speed);
                break;
        }

        Vector3f offset = m_camera.GetPosition() - m_targetPosition;
        m_orbitRadius = offset.Length();
    }

    void CameraController::Zoom(const ActionEvent &action) noexcept
    {
        float zoomAmount = action.m_yOffset;

        if (zoomAmount > 0)
            m_orbitRadius *= 0.9f;
        else
            m_orbitRadius *= 1.1f;

        if (m_orbitRadius < 1.0f)
            m_orbitRadius = 1.0f;

        if (m_orbitRadius > 20.0f)
            m_orbitRadius = 20.0f;

        Vector3f target = m_targetPosition;
        float radius = m_orbitRadius;

        float x = target.GetX() + radius * cos(DegToRad(m_camera.GetYaw())) * cos(DegToRad(m_camera.GetPitch()));
        float y = target.GetY() + radius * sin(DegToRad(m_camera.GetPitch()));
        float z = target.GetZ() + radius * sin(DegToRad(m_camera.GetYaw())) * cos(DegToRad(m_camera.GetPitch()));

        m_camera.SetPosition({ x, y, z });
        m_camera.SetFront((target - m_camera.GetPosition()).Normalize());
    }

    void CameraController::MoveAround(const ActionEvent& action, bool) noexcept
    {
        Vector3f offset = m_camera.GetPosition() - m_targetPosition;
        m_orbitRadius = offset.Length();

        float xpos = action.m_xOffset;
        float ypos = action.m_yOffset;

        float xOffset = xpos - lastX;
        float yOffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        float sensitivityX = 0.1f;
        float sensitivityY = 0.15f;
        xOffset *= sensitivityX;
        yOffset *= sensitivityY;

        m_camera.SetYaw(m_camera.GetYaw() + xOffset);
        m_camera.SetPitch(m_camera.GetPitch() + yOffset);

        if (m_camera.GetPitch() > 89.0f)
            m_camera.SetPitch(89.0f);
        if (m_camera.GetPitch() < -89.0f)
            m_camera.SetPitch(-89.0f);

        Vector3f target = m_targetPosition;
        float radius = m_orbitRadius;

        float x = target.GetX() + radius * cos(DegToRad(m_camera.GetYaw())) * cos(DegToRad(m_camera.GetPitch()));
        float y = target.GetY() + radius * sin(DegToRad(m_camera.GetPitch()));
        float z = target.GetZ() + radius * sin(DegToRad(m_camera.GetYaw())) * cos(DegToRad(m_camera.GetPitch()));

        m_camera.SetPosition({ x, y, z });
        m_camera.SetFront((target - m_camera.GetPosition()).Normalize());
    }

    void CameraController::MoveUpDown(const ActionEvent &action, bool) const noexcept
    {
        m_camera.SetFov(m_camera.GetFov() - action.m_yOffset);
        if (m_camera.GetFov() < 1.0f)
            m_camera.SetFov(1.0f);
        if (m_camera.GetFov() > 45.0f)
            m_camera.SetFov(45.0f);
    }

    void CameraController::OnMouseDrag(const MouseDragEvent& e) const noexcept
    {
        float deltaX = static_cast<float>(e.m_dx);
        float deltaY = static_cast<float>(e.m_dy);

        float sensitivityX = 0.5f;
        float sensitivityY = 0.75f;
        deltaX *= sensitivityX;
        deltaY *= sensitivityY;

        m_camera.SetYaw(m_camera.GetYaw() + deltaX);
        m_camera.SetPitch(m_camera.GetPitch() - deltaY);

        if (m_camera.GetPitch() > 89.0f)
            m_camera.SetPitch(89.0f);

        if (m_camera.GetPitch() < -89.0f)
            m_camera.SetPitch(-89.0f);

        Vector3f target = m_targetPosition;
        float radius = m_orbitRadius;

        float x = target.GetX() + radius * cos(DegToRad(m_camera.GetYaw())) * cos(DegToRad(m_camera.GetPitch()));
        float y = target.GetY() + radius * sin(DegToRad(m_camera.GetPitch()));
        float z = target.GetZ() + radius * sin(DegToRad(m_camera.GetYaw())) * cos(DegToRad(m_camera.GetPitch()));

        m_camera.SetPosition({ x, y, z });
        Vector3f front = (target - m_camera.GetPosition()).Normalize();
        m_camera.SetFront(front);
        m_camera.UpdateView();
    }

    void CameraController::OnMouseMotion(const MouseMotionEvent& e) noexcept
    {
        float deltaX = static_cast<float>(e.m_targetX - m_lastMouseX);
        float deltaY = static_cast<float>(e.m_targetY - m_lastMouseY);

        m_lastMouseX = e.m_targetX;
        m_lastMouseY = e.m_targetY;

        // For enabling panning
        // TODO: generalize for all input devices, not only keyboard
        if (InputDevice::IsKeyDown(InputType::Keyboard, m_actionMapper.GetKeyCombo(InputAction::Pan, InputType::Keyboard).key))
        {
            Pan(deltaX, deltaY);
            return;
        }

        if (e.m_button != MouseButtons::MouseCursorMoveAround)
            return;

        float sensitivityX = 0.1f;
        float sensitivityY = 0.15f;
        deltaX *= sensitivityX;
        deltaY *= sensitivityY;

        m_camera.SetYaw(m_camera.GetYaw() + deltaX);
        m_camera.SetPitch(m_camera.GetPitch() - deltaY);

        if (m_camera.GetPitch() > 89.0f)
            m_camera.SetPitch(89.0f);

        if (m_camera.GetPitch() < -89.0f)
            m_camera.SetPitch(-89.0f);

        Vector3f target = m_targetPosition;
        float radius = m_orbitRadius;

        float x = target.GetX() + radius * cos(DegToRad(m_camera.GetYaw())) * cos(DegToRad(m_camera.GetPitch()));
        float y = target.GetY() + radius * sin(DegToRad(m_camera.GetPitch()));
        float z = target.GetZ() + radius * sin(DegToRad(m_camera.GetYaw())) * cos(DegToRad(m_camera.GetPitch()));

        m_camera.SetPosition({ x, y, z });
        Vector3f front = (target - m_camera.GetPosition()).Normalize();
        m_camera.SetFront(front);
        m_camera.UpdateView();
    }

    void CameraController::OnMouseScroll(const MouseScrollEvent& e) noexcept
    {
        float zoomAmount = static_cast<float>(e.m_scrollY) * 0.1f;

        if (zoomAmount > 0)
            m_orbitRadius *= 0.9f;
        else
            m_orbitRadius *= 1.1f;

        if (m_orbitRadius < 1.0f)
            m_orbitRadius = 1.0f;

        if (m_orbitRadius > 20.0f)
            m_orbitRadius = 20.0f;

        Vector3f target = m_targetPosition;
        float radius = m_orbitRadius;

        float x = target.GetX() + radius * cos(DegToRad(m_camera.GetYaw())) * cos(DegToRad(m_camera.GetPitch()));
        float y = target.GetY() + radius * sin(DegToRad(m_camera.GetPitch()));
        float z = target.GetZ() + radius * sin(DegToRad(m_camera.GetYaw())) * cos(DegToRad(m_camera.GetPitch()));

        m_camera.SetPosition({ x, y, z });
        m_camera.SetFront((target - m_camera.GetPosition()).Normalize());
        m_camera.UpdateView();
    }

    void CameraController::GamepadLook(float rightX, float rightY, double deltaTime) const noexcept
    {
        float sensitivity = m_cameraRotationSpeed * deltaTime;
        float yawDelta = rightX * sensitivity;
        float pitchDelta = -rightY * sensitivity;

        m_camera.SetYaw(m_camera.GetYaw() + yawDelta);
        m_camera.SetPitch(m_camera.GetPitch() + pitchDelta);

        if (m_camera.GetPitch() > 89.0f)
            m_camera.SetPitch(89.0f);
        if (m_camera.GetPitch() < -89.0f)
            m_camera.SetPitch(-89.0f);

        float x = m_targetPosition.GetX() + m_orbitRadius * cos(DegToRad(m_camera.GetYaw())) * cos(DegToRad(m_camera.GetPitch()));
        float y = m_targetPosition.GetY() + m_orbitRadius * sin(DegToRad(m_camera.GetPitch()));
        float z = m_targetPosition.GetZ() + m_orbitRadius * sin(DegToRad(m_camera.GetYaw())) * cos(DegToRad(m_camera.GetPitch()));

        m_camera.SetPosition({ x, y, z });
        m_camera.SetFront((m_targetPosition - m_camera.GetPosition()).Normalize());
        m_camera.UpdateView();
    }

    void CameraController::GamepadMove(float leftX, float leftY, double deltaTime) noexcept
    {
        auto speed = static_cast<float>(m_cameraTranslationSpeed * m_gamepadSpeedMultiplier * deltaTime);

        Vector3f viewDir = m_camera.GetFront();

        Vector3f rightDir = Cross(viewDir, m_camera.GetUp());
        rightDir.Normalize();

        Vector3f moveVector;

        if (abs(leftY) > 0.1f)
            moveVector = moveVector - viewDir * leftY;

        if (abs(leftX) > 0.1f)
            moveVector = moveVector + rightDir * leftX;

        if (moveVector.Length() > 0.001f)
        {
            moveVector.Normalize();
            moveVector = moveVector * speed;

            m_camera.SetPosition(m_camera.GetPosition() + moveVector);

            Vector3f offset = m_camera.GetPosition() - m_targetPosition;
            m_orbitRadius = offset.Length();
        }
    }

    void CameraController::GamepadZoom(float triggerValue, double) noexcept
    {
        if (abs(triggerValue) > 0.1f) {

            if (m_orbitRadius > 0.1f)
            {
                if (triggerValue > 0)
                    m_orbitRadius *= 0.98f;
                else
                    m_orbitRadius *= 1.02f;

                if (m_orbitRadius < 1.0f)
                    m_orbitRadius = 1.0f;

                if (m_orbitRadius > 20.0f)
                    m_orbitRadius = 20.0f;

                Vector3f target = m_targetPosition;
                float radius = m_orbitRadius;

                float x = target.GetX() + radius * cos(DegToRad(m_camera.GetYaw())) * cos(DegToRad(m_camera.GetPitch()));
                float y = target.GetY() + radius * sin(DegToRad(m_camera.GetPitch()));
                float z = target.GetZ() + radius * sin(DegToRad(m_camera.GetYaw())) * cos(DegToRad(m_camera.GetPitch()));

                m_camera.SetPosition({ x, y, z });
                m_camera.SetFront((target - m_camera.GetPosition()).Normalize());
            }
            else
            {
                float fov = m_camera.GetFov();

                if (triggerValue > 0)
                    fov -= 1.0f;
                else
                    fov += 1.0f;

                if (fov < 1.0f)
                    fov = 1.0f;

                if (fov > 45.0f)
                    fov = 45.0f;

                m_camera.SetFov(fov);
            }
            m_camera.UpdateView();
        }
    }

    void CameraController::Pan(float deltaX, float deltaY) noexcept
    {
        float sensitivity = 0.01f;
        deltaX *= sensitivity;
        deltaY *= sensitivity;

        Vector3f right = Cross(m_camera.GetFront(), m_camera.GetUp()).Normalize();
        Vector3f up = m_camera.GetUp();

        Vector3f panOffset = -right * deltaX + up * deltaY;

        m_camera.SetPosition(m_camera.GetPosition() + panOffset);
        m_targetPosition = m_targetPosition + panOffset;
        m_camera.UpdateView();

    }
}