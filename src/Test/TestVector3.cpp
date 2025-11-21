#include <gtest/gtest.h>
#include "../../include/Math/Vector3.hpp"
namespace lux
{
    using namespace math;

    TEST(Vector3Test, DefaultConstructor) {
        Vector3<float> vec;
        EXPECT_EQ(vec.GetX(), 0.0f);
        EXPECT_EQ(vec.GetY(), 0.0f);
        EXPECT_EQ(vec.GetZ(), 0.0f);
    }

    TEST(Vector3Test, ParameterizedConstructor) {
        Vector3<float> vec(1.0f, 2.0f, 3.0f);
        EXPECT_EQ(vec.GetX(), 1.0f);
        EXPECT_EQ(vec.GetY(), 2.0f);
        EXPECT_EQ(vec.GetZ(), 3.0f);
    }

    TEST(Vector3Test, InitializerListConstructor) {
        Vector3<float> vec({1.0f, 2.0f, 3.0f});
        EXPECT_EQ(vec.GetX(), 1.0f);
        EXPECT_EQ(vec.GetY(), 2.0f);
        EXPECT_EQ(vec.GetZ(), 3.0f);
    }

    TEST(Vector3Test, OperatorAdd) {
        Vector3<float> vec1(1.0f, 2.0f, 3.0f);
        Vector3<float> vec2(4.0f, 5.0f, 6.0f);
        Vector3<float> result = vec1 + vec2;
        EXPECT_EQ(result.GetX(), 5.0f);
        EXPECT_EQ(result.GetY(), 7.0f);
        EXPECT_EQ(result.GetZ(), 9.0f);
    }

    TEST(Vector3Test, OperatorSub) {
        Vector3<float> vec1(1.0f, 2.0f, 3.0f);
        Vector3<float> vec2(4.0f, 5.0f, 6.0f);
        Vector3<float> result = vec1 - vec2;
        EXPECT_EQ(result.GetX(), -3.0f);
        EXPECT_EQ(result.GetY(), -3.0f);
        EXPECT_EQ(result.GetZ(), -3.0f);
    }

    TEST(Vector3Test, OperatorMul) {
        Vector3<float> vec(1.0f, 2.0f, 3.0f);
        Vector3<float> result = vec * 2.0f;
        EXPECT_EQ(result.GetX(), 2.0f);
        EXPECT_EQ(result.GetY(), 4.0f);
        EXPECT_EQ(result.GetZ(), 6.0f);
    }

    TEST(Vector3Test, OperatorDiv) {
        Vector3<float> vec(1.0f, 2.0f, 3.0f);
        Vector3<float> result = vec / 2.0f;
        EXPECT_EQ(result.GetX(), 0.5f);
        EXPECT_EQ(result.GetY(), 1.0f);
        EXPECT_EQ(result.GetZ(), 1.5f);
    }

    TEST(Vector3Test, Length) {
        Vector3<float> vec(3.0f, 4.0f, 0.0f);
        EXPECT_FLOAT_EQ(vec.Length(), 5.0f);
    }

    TEST(Vector3Test, Normalize) {
        Vector3<float> vec(3.0f, 4.0f, 0.0f);
        vec.Normalize();
        EXPECT_FLOAT_EQ(vec.Length(), 1.0f);
    }

    TEST(Vector3Test, DotProduct) {
        Vector3<float> vec1(1.0f, 0.0f, 0.0f);
        Vector3<float> vec2(0.0f, 1.0f, 0.0f);
        EXPECT_EQ(vec1.Dot(vec2), 0.0f);

        Vector3<float> vec3(1.0f, 2.0f, 3.0f);
        Vector3<float> vec4(4.0f, 5.0f, 6.0f);
        EXPECT_EQ(vec3.Dot(vec4), 32.0f); // (1*4 + 2*5 + 3*6)
    }

    TEST(Vector3Test, CrossProduct) {
        Vector3<float> vec1(1.0f, 2.0f, 3.0f);
        Vector3<float> vec2(4.0f, 5.0f, 6.0f);
        Vector3<float> result = Cross(vec1, vec2);
        EXPECT_EQ(result.GetX(), -3.0f);
        EXPECT_EQ(result.GetY(), 6.0f);
        EXPECT_EQ(result.GetZ(), -3.0f);
    }

    TEST(Vector3Test, ComparisonOperators)
    {
        Vector3<float> vec1(1.0f, 2.0f, 3.0f);
        Vector3<float> vec2(1.0f, 2.0f, 3.0f);
        Vector3<float> vec3(3.0f, 2.0f, 1.0f);

        EXPECT_TRUE(vec1 == vec2);
        EXPECT_FALSE(vec1 == vec3);
        EXPECT_TRUE(vec1 != vec3);
    }

    TEST(Vector3Test, AccessOutOfRange)
    {
        Vector3<float> vec(1.0f, 2.0f, 3.0f);
        EXPECT_THROW(vec[3], std::out_of_range);
    }
}

