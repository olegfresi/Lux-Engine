#include <gtest/gtest.h>
#include "../../include/Math/Matrix4.hpp"

namespace lux::math
{

    auto error = 0.00001;

    TEST(Matrix4Test, MatrixMultiplication)
    {
        Matrix4 mat1(1.0f, 2.0f, 3.0f, 4.0f,
                           5.0f, 6.0f, 7.0f, 8.0f,
                           9.0f, 10.0f, 11.0f, 12.0f,
                           13.0f, 14.0f, 15.0f, 16.0f);

        Matrix4 mat2(16.0f, 15.0f, 14.0f, 13.0f,
                           12.0f, 11.0f, 10.0f, 9.0f,
                           8.0f, 7.0f, 6.0f, 5.0f,
                           4.0f, 3.0f, 2.0f, 1.0f);

        Matrix4<float> result = mat1 * mat2;

        EXPECT_EQ(result[0][0], 386.0f);
        EXPECT_EQ(result[0][3], 560.0f);
        EXPECT_EQ(result[3][0], 50.0f);
        EXPECT_EQ(result[3][3], 80.0f);
    }

    TEST(Matrix4Test, MatrixVectorMultiplication)
    {
        Matrix4 mat(1.0f, 0.0f, 0.0f, 0.0f,
                          0.0f, 1.0f, 0.0f, 0.0f,
                          0.0f, 0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

        Vector4 vec(1.0f, 2.0f, 3.0f, 1.0f);
        Vector4<float> result = mat * vec;

        EXPECT_EQ(result.GetX(), 1.0f);
        EXPECT_EQ(result.GetY(), 2.0f);
        EXPECT_EQ(result.GetZ(), 3.0f);
        EXPECT_EQ(result.GetW(), 1.0f);
    }

    TEST(Matrix4Test, Transpose)
    {
        Matrix4 mat(1.0f, 2.0f, 3.0f, 4.0f,
                          5.0f, 6.0f, 7.0f, 8.0f,
                          9.0f, 10.0f, 11.0f, 12.0f,
                          13.0f, 14.0f, 15.0f, 16.0f);

        Matrix4<float> transposed = mat.Transpose();

        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                EXPECT_EQ(transposed[i][j], mat[j][i]);
    }

    TEST(Matrix4Test, Determinant)
    {
        Matrix4 mat(1.0f, 0.0f, 0.0f, 0.0f,
                          0.0f, 1.0f, 0.0f, 0.0f,
                          0.0f, 0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

        float det = mat.Determinant();
        EXPECT_EQ(det, 1.0f);
    }

    TEST(Matrix4Test, Inverse)
    {
        Matrix4 mat(1.0f, 0.0f, 0.0f, 0.0f,
                          0.0f, 1.0f, 0.0f, 0.0f,
                          0.0f, 0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

        Matrix4<float> inverse = mat.Inverse();

        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                if(i == j)
                    EXPECT_EQ(inverse[i][j], 1.0f);
                else
                    EXPECT_EQ(inverse[i][j], 0.0f);
    }

    TEST(Matrix4Test, TranslationMatrix)
    {
        Vector3 translation(1.0f, 2.0f, 3.0f);
        Matrix4<float> transMat = Matrix4<float>::Translate(translation);

        EXPECT_EQ(transMat[0][3], 1.0f);
        EXPECT_EQ(transMat[1][3], 2.0f);
        EXPECT_EQ(transMat[2][3], 3.0f);

        EXPECT_EQ(transMat[0][0], 1.0f);
        EXPECT_EQ(transMat[1][1], 1.0f);
        EXPECT_EQ(transMat[2][2], 1.0f);
        EXPECT_EQ(transMat[3][3], 1.0f);
    }

    TEST(Matrix4Test, RotationMatrix)
    {
        float angle = 90.0f * M_PI / 180.0f;
        Matrix4<float> rotMat = Matrix4<float>::Rotate(angle);

        EXPECT_EQ(rotMat[0][0], cos(angle));
        EXPECT_EQ(rotMat[0][1], -sin(angle));
        EXPECT_EQ(rotMat[0][2], 0.0f);
        EXPECT_EQ(rotMat[1][0], sin(angle));
        EXPECT_EQ(rotMat[1][1], cos(angle));
        EXPECT_EQ(rotMat[1][2], 0.0f);
        EXPECT_EQ(rotMat[2][0], 0.0f);
        EXPECT_EQ(rotMat[2][1], 0.0f);
        EXPECT_FLOAT_EQ(rotMat[2][2], 1.0f);
        EXPECT_EQ(rotMat[3][3], 1.0f);
    }

    TEST(Matrix4Test, ScaleMatrix) {
        Vector3 scale(2.0f, 3.0f, 4.0f);
        Matrix4<float> scaleMat = Matrix4<float>::Scale(scale);

        EXPECT_EQ(scaleMat[0][0], 2.0f);
        EXPECT_EQ(scaleMat[1][1], 3.0f);
        EXPECT_EQ(scaleMat[2][2], 4.0f);
        EXPECT_EQ(scaleMat[3][3], 1.0f);

        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                if(i != j)
                    EXPECT_EQ(scaleMat[i][j], 0.0f);
    }


    TEST(Matrix4Test, OrthographicMatrix)
    {
        float left = -1.0f;
        float right = 1.0f;
        float bottom = -1.0f;
        float top = 1.0f;
        float near = -1.0f;
        float far = 1.0f;

        Matrix4<float> orthoMat = Matrix4<float>::Orthographic(left, right, bottom, top, near, far);

        EXPECT_FLOAT_EQ(orthoMat[0][0], 2.0f / (right - left));
        EXPECT_FLOAT_EQ(orthoMat[1][1], 2.0f / (top - bottom));
        EXPECT_FLOAT_EQ(orthoMat[2][2], -2.0f / (far - near));
        EXPECT_FLOAT_EQ(orthoMat[3][0], -(right + left) / (right - left));
        EXPECT_FLOAT_EQ(orthoMat[3][1], -(top + bottom) / (top - bottom));
        EXPECT_FLOAT_EQ(orthoMat[3][2], -(far + near) / (far - near));
        EXPECT_FLOAT_EQ(orthoMat[3][3], 1.0f);
    }

    TEST(Matrix4Test, ComparisonOperators)
    {
        Matrix4 mat1(1.0f, 2.0f, 3.0f, 4.0f,
                           5.0f, 6.0f, 7.0f, 8.0f,
                           9.0f, 10.0f, 11.0f, 12.0f,
                           13.0f, 14.0f, 15.0f, 16.0f);

        Matrix4 mat2(1.0f, 2.0f, 3.0f, 4.0f,
                           5.0f, 6.0f, 7.0f, 8.0f,
                           9.0f, 10.0f, 11.0f, 12.0f,
                           13.0f, 14.0f, 15.0f, 16.0f);

        Matrix4 mat3(16.0f, 15.0f, 14.0f, 13.0f,
                           12.0f, 11.0f, 10.0f, 9.0f,
                           8.0f, 7.0f, 6.0f, 5.0f,
                           4.0f, 3.0f, 2.0f, 1.0f);

        EXPECT_TRUE(mat1 == mat2);
        EXPECT_FALSE(mat1 == mat3);
        EXPECT_TRUE(mat1 != mat3);
    }

    TEST(Matrix4Test, AccessOutOfRange)
    {
        Matrix4<float> mat;
        EXPECT_THROW(mat[4][0], std::out_of_range);
        EXPECT_THROW(mat[0][4], std::out_of_range);
    }



    TEST(Matrix4Test, DeterminantIdentityMatrix)
    {
        Matrix4 identity(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            MatOrder::ROW_MAJOR
        );

        Matrix4 identity_col(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            MatOrder::COLUMN_MAJOR
        );

        float expected_det = 1.0f;

        ASSERT_NEAR(identity.Determinant(), expected_det, error)
            << "Det (Identità, ROW_MAJOR) deve essere 1.";
        ASSERT_NEAR(identity_col.Determinant(), expected_det, error)
            << "Det (Identità, COLUMN_MAJOR) deve essere 1.";
    }

    TEST(Matrix4Test, DeterminantGeneralMatrix)
    {

        Matrix4 mat_A(
            2.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 3.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 4.0f, 1.0f,
            0.0f, 0.0f, 2.0f, 5.0f,
            MatOrder::ROW_MAJOR
        );

        float expected_determinant = 108.0f;

        ASSERT_NEAR(mat_A.Determinant(), expected_determinant, error)
            << "Determinante di Matrix A (ROW_MAJOR) non corrisponde a 108.";
    }

    TEST(Matrix4tTest, OneValueConstructor)
    {
        Matrix4 mat_row(3.0f, MatOrder::ROW_MAJOR);
        Matrix4 mat_col(3.0f, MatOrder::COLUMN_MAJOR);

        ASSERT_EQ(mat_col, mat_row.Transpose());
    }

}