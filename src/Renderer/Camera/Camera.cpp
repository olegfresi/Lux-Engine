#include "../../include/Renderer/Camera/Camera.hpp"

namespace lux
{
    Camera::Camera()
    {
        m_pos = Vector3f(0.0f, 0.0f, 0.0f);
        m_front = Vector3f(0.0f, 0.0f, -1.0f);
        m_up = Vector3f(0.0f, 0.0f, 0.0f);
        m_right = Cross(GetFront(), GetUp()).Normalize();

        m_yaw = 0.0f;
        m_pitch = 0.0f;
        m_fov = 0.0f;

        m_view = Matrix4f::LookAt( m_pos, m_pos + m_front, m_up);

        m_projection = Matrix4f::Perspective( m_fov, 0, 1.0f, 100.0f );
    }

    Camera::Camera(const Vector3f& pos, const Vector3f& front, const Vector3f& up, float aspectRatio, float fov, float yaw, float pitch, float near, float far) noexcept :
                m_pos{ pos }, m_front{ front }, m_up{ up }, m_right{Cross(GetFront(), GetUp()).Normalize() },
                m_aspectRatio{ aspectRatio }, m_fov{ fov }, m_yaw{ yaw }, m_pitch{ pitch }
    {
        m_view = Matrix4f::LookAt( pos, pos + front, up).Transpose();

        m_projection = Matrix4f::Perspective( m_fov, aspectRatio, near, far );
    }

    void Camera::SetPosition(const Vector3f &position) noexcept
    {
        m_pos = position;
        UpdateView();
    }

    void Camera::SetFront(const Vector3f &front) noexcept
    {
        m_front = front;
        UpdateView();
    }

    void Camera::SetUp(const Vector3f &up) noexcept
    {
        m_up = up;
        UpdateView();
    }

    void Camera::SetRight(const Vector3f &right) noexcept
    {
        m_right = right;
        UpdateView();
    }

    void Camera::SetFov(float fov) noexcept
    {
        m_fov = fov;
        UpdateProjection();
    }

    void Camera::SetYaw(float yaw) noexcept
    {
        m_yaw = yaw;
        UpdateView();
    }

    void Camera::SetPitch(float pitch) noexcept
    {
        m_pitch = pitch;
        UpdateView();
    }

    void Camera::UpdateView() noexcept
    {
        m_view = Matrix4f::LookAt(m_pos, m_pos + m_front, m_up).Transpose();
    }

    void Camera::UpdateProjection() noexcept
    {
        m_projection = Matrix4f::Perspective(m_fov, m_aspectRatio, m_near, m_far);
    }

    void Camera::SetTransform(const math::Matrix4f &transform) noexcept
    {
        m_transform = transform;
        UpdateView();
    }

    void Camera::SetAspectRatio(float aspectRatio) noexcept
    {
        m_aspectRatio = aspectRatio;
        UpdateProjection();
    }

    void Camera::SetNear(float near) noexcept
    {
        m_near = near;
    }

    void Camera::SetFar(float far) noexcept
    {
        m_far = far;
    }
}