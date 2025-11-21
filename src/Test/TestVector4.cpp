#include <gtest/gtest.h>
#include "../../include/Math/Vector4.hpp"
namespace lux
{
    using namespace math;

    /*
    TEST(Vector4Test, DefaultConstructor)
    {
        Vector4<float> vec;
        EXPECT_EQ(vec.GetX(), 0.0f);
        EXPECT_EQ(vec.GetY(), 0.0f);
        EXPECT_EQ(vec.GetZ(), 0.0f);
        EXPECT_EQ(vec.GetW(), 1.0f);
    }

    TEST(Vector4Test, ParameterizedConstructor)
    {
        Vector4<float> vec(1.0f, 2.0f, 3.0f, 4.0f);
        EXPECT_EQ(vec.GetX(), 1.0f);
        EXPECT_EQ(vec.GetY(), 2.0f);
        EXPECT_EQ(vec.GetZ(), 3.0f);
        EXPECT_EQ(vec.GetW(), 4.0f);
    }

    TEST(Vector4Test, VectorAddition)
    {
        Vector4<float> vec1(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4<float> vec2(5.0f, 6.0f, 7.0f, 8.0f);
        Vector4<float> result = vec1 + vec2;

        EXPECT_EQ(result.GetX(), 6.0f);
        EXPECT_EQ(result.GetY(), 8.0f);
        EXPECT_EQ(result.GetZ(), 10.0f);
        EXPECT_EQ(result.GetW(), 12.0f);
    }

    TEST(Vector4Test, VectorSubtraction)
    {
        Vector4<float> vec1(5.0f, 6.0f, 7.0f, 8.0f);
        Vector4<float> vec2(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4<float> result = vec1 - vec2;

        EXPECT_EQ(result.GetX(), 4.0f);
        EXPECT_EQ(result.GetY(), 4.0f);
        EXPECT_EQ(result.GetZ(), 4.0f);
        EXPECT_EQ(result.GetW(), 4.0f);
    }

    TEST(Vector4Test, ScalarMultiplication)
    {
        Vector4<float> vec(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4<float> result = vec * 2.0f;

        EXPECT_EQ(result.GetX(), 2.0f);
        EXPECT_EQ(result.GetY(), 4.0f);
        EXPECT_EQ(result.GetZ(), 6.0f);
        EXPECT_EQ(result.GetW(), 8.0f);
    }

    TEST(Vector4Test, ScalarDivision)
    {
        Vector4<float> vec(2.0f, 4.0f, 6.0f, 8.0f);
        Vector4<float> result = vec / 2.0f;

        EXPECT_EQ(result.GetX(), 1.0f);
        EXPECT_EQ(result.GetY(), 2.0f);
        EXPECT_EQ(result.GetZ(), 3.0f);
        EXPECT_EQ(result.GetW(), 4.0f);
    }

    TEST(Vector4Test, DotProduct)
    {
        Vector4<float> vec1(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4<float> vec2(5.0f, 6.0f, 7.0f, 8.0f);
        float dot = vec1.Dot(vec2);

        EXPECT_EQ(dot, 70.0f);
    }

    TEST(Vector4Test, Length)
    {
        Vector4<float> vec(1.0f, 2.0f, 3.0f, 4.0f);
        float length = vec.Length();

        EXPECT_FLOAT_EQ(length, sqrt(30.0f));
    }

    TEST(Vector4Test, Normalize)
    {
        Vector4<float> vec(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4<float>::Normalize(vec);
        float length = vec.Length();

        EXPECT_FLOAT_EQ(length, 1.0f);
    }


    TEST(Vector4Test, ComparisonOperators)
    {
        Vector4<float> vec1(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4<float> vec2(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4<float> vec3(5.0f, 6.0f, 7.0f, 8.0f);

        EXPECT_TRUE(vec1 == vec2);
        EXPECT_FALSE(vec1 == vec3);
        EXPECT_TRUE(vec1 != vec3);
    }
     */
}