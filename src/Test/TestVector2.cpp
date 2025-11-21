#include <gtest/gtest.h>
#include <../../include/Math/Vector2.hpp>
#include <cmath>
namespace  lux
{
    using namespace math;

    /*
    class Vector2Test : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            v1 = Vector2<float>(1.0f, 2.0f);
            v2 = Vector2<float>(3.0f, 4.0f);
            v3 = Vector2<float>(0.0f, 0.0f);
        }

        Vector2<float> v1;
        Vector2<float> v2;
        Vector2<float> v3;
    };

    TEST_F(Vector2Test, Constructors)
    {
        Vector2<float> v;
        EXPECT_EQ(v.GetX(), 0.0f);
        EXPECT_EQ(v.GetY(), 0.0f);

        Vector2<float> v4(5.0f, 6.0f);
        EXPECT_EQ(v4.GetX(), 5.0f);
        EXPECT_EQ(v4.GetY(), 6.0f);

        Vector2<float> v5{7.0f, 8.0f};
        EXPECT_EQ(v5.GetX(), 7.0f);
        EXPECT_EQ(v5.GetY(), 8.0f);

        Vector2<float> v6(v4);
        EXPECT_EQ(v6.GetX(), 5.0f);
        EXPECT_EQ(v6.GetY(), 6.0f);
    }

    TEST_F(Vector2Test, ArithmeticOperators)
    {
        Vector2<float> sum = v1 + v2;
        EXPECT_EQ(sum.GetX(), 4.0f);
        EXPECT_EQ(sum.GetY(), 6.0f);

        Vector2<float> diff = v2 - v1;
        EXPECT_EQ(diff.GetX(), 2.0f);
        EXPECT_EQ(diff.GetY(), 2.0f);

        Vector2<float> scaled = v1 * 2.0f;
        EXPECT_EQ(scaled.GetX(), 2.0f);
        EXPECT_EQ(scaled.GetY(), 4.0f);

        Vector2<float> divided = v2 / 2.0f;
        EXPECT_EQ(divided.GetX(), 1.5f);
        EXPECT_EQ(divided.GetY(), 2.0f);
    }

    TEST_F(Vector2Test, CompoundAssignmentOperators)
    {
        Vector2<float> v = v1;

        v += v2;
        EXPECT_EQ(v.GetX(), 4.0f);
        EXPECT_EQ(v.GetY(), 6.0f);

        v -= v1;
        EXPECT_EQ(v.GetX(), 3.0f);
        EXPECT_EQ(v.GetY(), 4.0f);

        v *= 2.0f;
        EXPECT_EQ(v.GetX(), 6.0f);
        EXPECT_EQ(v.GetY(), 8.0f);

        v /= 2.0f;
        EXPECT_EQ(v.GetX(), 3.0f);
        EXPECT_EQ(v.GetY(), 4.0f);
    }

    TEST_F(Vector2Test, ComparisonOperators) {
        Vector2<float> v1_copy = v1;

        EXPECT_TRUE(v1 == v1_copy);
        EXPECT_FALSE(v1 == v2);

        EXPECT_TRUE(v1 != v2);
        EXPECT_FALSE(v1 != v1_copy);
    }

    TEST_F(Vector2Test, MathematicalOperations)
    {
        EXPECT_FLOAT_EQ(v1.Length(), std::sqrt(5.0f));
        EXPECT_FLOAT_EQ(v2.Length(), 5.0f);

        EXPECT_FLOAT_EQ(v1.Dot(v2), 11.0f);

        Vector2<float> v = v1;
        Vector2<float>::Normalize(v);
        EXPECT_FLOAT_EQ(v.Length(), 1.0f);
    }

    TEST_F(Vector2Test, ElementAccess)
    {
        EXPECT_EQ(v1.GetX(), 1.0f);
        EXPECT_EQ(v1.GetY(), 2.0f);

        EXPECT_EQ(v1.GetU(), 1.0f);
        EXPECT_EQ(v1.GetV(), 2.0f);

        EXPECT_EQ(v1[0], 1.0f);
        EXPECT_EQ(v1[1], 2.0f);
    }

    TEST_F(Vector2Test, ErrorHandling)
    {
        EXPECT_THROW(v1 / 0.0f, std::runtime_error);

        EXPECT_THROW(v1[2], std::out_of_range);

        EXPECT_THROW(Vector2<float>({1.0f, 2.0f, 3.0f}), std::invalid_argument);
    }

    TEST_F(Vector2Test, DifferentTypes)
    {
        Vector2<int> vi(1, 2);
        EXPECT_EQ(vi.GetX(), 1);
        EXPECT_EQ(vi.GetY(), 2);

        Vector2<double> vd(1.0, 2.0);
        EXPECT_DOUBLE_EQ(vd.GetX(), 1.0);
        EXPECT_DOUBLE_EQ(vd.GetY(), 2.0);
    }
     */
}