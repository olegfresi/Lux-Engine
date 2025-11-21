#include <gtest/gtest.h>
#include "../../include/Renderer/Camera/Camera.hpp"

namespace lux::test
{
    class CameraTest : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            m_Camera = std::make_unique<Camera>();
        }

        void TearDown() override
        {
            m_Camera.reset();
        }

        std::unique_ptr<Camera> m_Camera;
    };

    TEST_F(CameraTest, DefaultConstructor)
    {
        EXPECT_EQ(m_Camera->GetPosition(), math::Vector3f(0.0f, 0.0f, 0.0f));
        EXPECT_EQ(m_Camera->GetFront(), math::Vector3f(0.0f, 0.0f, -1.0f));
        EXPECT_EQ(m_Camera->GetUp(), math::Vector3f(0.0f, 0.0f, 0.0f));
        EXPECT_EQ(m_Camera->GetFov(), 0.0f);
        EXPECT_EQ(m_Camera->GetNear(), 1.0f);
        EXPECT_EQ(m_Camera->GetFar(), 100.0f);
        EXPECT_EQ(m_Camera->GetAspectRatio(), 0.0f);
    }

    TEST_F(CameraTest, PositionSetting)
    {
        math::Vector3f newPos(1.0f, 2.0f, 3.0f);
        m_Camera->SetPosition(newPos);
        EXPECT_EQ(m_Camera->GetPosition(), newPos);
    }

    TEST_F(CameraTest, FrontDirectionSetting)
    {
        math::Vector3f newFront(1.0f, 0.0f, 0.0f);
        m_Camera->SetFront(newFront);
        EXPECT_EQ(m_Camera->GetFront(), newFront);
    }

    TEST_F(CameraTest, UpVectorSetting)
    {
        math::Vector3f newUp(0.0f, 1.0f, 0.0f);
        m_Camera->SetUp(newUp);
        EXPECT_EQ(m_Camera->GetUp(), newUp);
    }

    TEST_F(CameraTest, FovSetting)
    {
        float newFov = 60.0f;
        m_Camera->SetFov(newFov);
        EXPECT_EQ(m_Camera->GetFov(), newFov);
    }

    TEST_F(CameraTest, TransformSetting)
    {
        math::Matrix4f transform;
        transform = math::Matrix4f::Translate(math::Vector3f(1.0f, 2.0f, 3.0f));
        m_Camera->SetTransform(transform);
        EXPECT_EQ(m_Camera->GetTransform(), transform);
    }
}