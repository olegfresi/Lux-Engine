#include <gtest/gtest.h>
#include "../../include/Math/Color.hpp"

namespace lux
{
    using namespace math;

    TEST(ColorTest, DefaultConstructor)
    {
        Color color{};
        EXPECT_EQ(color.GetR(), 0.0f);
        EXPECT_EQ(color.GetG(), 0.0f);
        EXPECT_EQ(color.GetB(), 0.0f);
        EXPECT_EQ(color.GetA(), 1.0f);
    }

    TEST(ColorTest, ParameterizedConstructor)
    {
        Color color(1.0f, 0.5f, 0.25f, 0.75f);
        EXPECT_EQ(color.GetR(), 1.0f);
        EXPECT_EQ(color.GetG(), 0.5f);
        EXPECT_EQ(color.GetB(), 0.25f);
        EXPECT_EQ(color.GetA(), 0.75f);
    }

    TEST(ColorTest, ColorAddition)
    {
        Color color1(0.5f, 0.5f, 0.5f, 1.0f);
        Color color2(0.5f, 0.5f, 0.5f, 1.0f);
        Color result = color1 + color2;

        EXPECT_EQ(result.GetR(), 1.0f);
        EXPECT_EQ(result.GetG(), 1.0f);
        EXPECT_EQ(result.GetB(), 1.0f);
        EXPECT_EQ(result.GetA(), 2.0f);
    }

    TEST(ColorTest, ColorSubtraction)
    {
        Color color1(1.0f, 1.0f, 1.0f, 1.0f);
        Color color2(0.5f, 0.5f, 0.5f, 1.0f);
        Color result = color1 - color2;

        EXPECT_EQ(result.GetR(), 0.5f);
        EXPECT_EQ(result.GetG(), 0.5f);
        EXPECT_EQ(result.GetB(), 0.5f);
        EXPECT_EQ(result.GetA(), 0.0f);
    }

    TEST(ColorTest, ScalarMultiplication)
    {
        Color color(0.5f, 0.5f, 0.5f, 1.0f);
        Color result = color * 2.0f;

        EXPECT_EQ(result.GetR(), 1.0f);
        EXPECT_EQ(result.GetG(), 1.0f);
        EXPECT_EQ(result.GetB(), 1.0f);
        EXPECT_EQ(result.GetA(), 2.0f);
    }

    TEST(ColorTest, ColorMultiplication)
    {
        Color color1(0.5f, 0.5f, 0.5f, 1.0f);
        Color color2(0.5f, 0.5f, 0.5f, 1.0f);
        Color result = color1 * color2;

        EXPECT_EQ(result.GetR(), 0.25f);
        EXPECT_EQ(result.GetG(), 0.25f);
        EXPECT_EQ(result.GetB(), 0.25f);
        EXPECT_EQ(result.GetA(), 1.0f);
    }

    TEST(ColorTest, Clamp)
    {
        Color color(1.5f, -0.5f, 0.5f, 2.0f);
        color.Clamp();

        EXPECT_EQ(color.GetR(), 1.0f);
        EXPECT_EQ(color.GetG(), 0.0f);
        EXPECT_EQ(color.GetB(), 0.5f);
        EXPECT_EQ(color.GetA(), 1.0f);
    }

    TEST(ColorTest, Lerp)
    {
        Color color1(0.0f, 0.0f, 0.0f, 1.0f);
        Color color2(1.0f, 1.0f, 1.0f, 1.0f);
        Color result = Color::Lerp(color1, color2, 0.5f);

        EXPECT_EQ(result.GetR(), 0.5f);
        EXPECT_EQ(result.GetG(), 0.5f);
        EXPECT_EQ(result.GetB(), 0.5f);
        EXPECT_EQ(result.GetA(), 1.0f);
    }

    TEST(ColorTest, ComparisonOperators)
    {
        Color color1(1.0f, 0.5f, 0.25f, 0.75f);
        Color color2(1.0f, 0.5f, 0.25f, 0.75f);
        Color color3(0.0f, 0.0f, 0.0f, 1.0f);

        EXPECT_TRUE(color1 == color2);
        EXPECT_FALSE(color1 == color3);
        EXPECT_TRUE(color1 != color3);
    }

    TEST(ColorTest, SetComponents)
    {
        Color color;
        color.SetR(1.0f);
        color.SetG(0.5f);
        color.SetB(0.25f);
        color.SetA(0.75f);

        EXPECT_EQ(color.GetR(), 1.0f);
        EXPECT_EQ(color.GetG(), 0.5f);
        EXPECT_EQ(color.GetB(), 0.25f);
        EXPECT_EQ(color.GetA(), 0.75f);
    }

    TEST(ColorTest, PredefinedColors)
    {
        Color red = Color::Red();
        Color green = Color::Green();
        Color blue = Color::Blue();
        Color white = Color::White();
        Color black = Color::Black();

        EXPECT_EQ(red.GetR(), 1.0f);
        EXPECT_EQ(red.GetG(), 0.0f);
        EXPECT_EQ(red.GetB(), 0.0f);

        EXPECT_EQ(green.GetR(), 0.0f);
        EXPECT_EQ(green.GetG(), 1.0f);
        EXPECT_EQ(green.GetB(), 0.0f);

        EXPECT_EQ(blue.GetR(), 0.0f);
        EXPECT_EQ(blue.GetG(), 0.0f);
        EXPECT_EQ(blue.GetB(), 1.0f);

        EXPECT_EQ(white.GetR(), 1.0f);
        EXPECT_EQ(white.GetG(), 1.0f);
        EXPECT_EQ(white.GetB(), 1.0f);

        EXPECT_EQ(black.GetR(), 0.0f);
        EXPECT_EQ(black.GetG(), 0.0f);
        EXPECT_EQ(black.GetB(), 0.0f);
    }
}