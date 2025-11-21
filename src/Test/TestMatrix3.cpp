#include <gtest/gtest.h>
#include "../../include/Math/Matrix3.hpp"

namespace lux
{
    using namespace math;

    TEST(Matrix3Test, ParameterizedConstructor)
    {
        Matrix3 mat(1.0f, 2.0f, 3.0f,
                          4.0f, 5.0f, 6.0f,
                          7.0f, 8.0f, 9.0f, MatOrder::ROW_MAJOR);

        EXPECT_EQ(mat[0][0], 1.0f);
        EXPECT_EQ(mat[0][1], 2.0f);
        EXPECT_EQ(mat[0][2], 3.0f);
        EXPECT_EQ(mat[1][0], 4.0f);
        EXPECT_EQ(mat[1][1], 5.0f);
        EXPECT_EQ(mat[1][2], 6.0f);
        EXPECT_EQ(mat[2][0], 7.0f);
        EXPECT_EQ(mat[2][1], 8.0f);
        EXPECT_EQ(mat[2][2], 9.0f);
    }

    TEST(Matrix3Test, DeterminantCalculation)
    {
        // Matrice A:
        // 2  1  3
        // 0  4 -1
        // 5  2  0

        // Passiamo gli elementi nell'ordine Row-Major (riga per riga)
        Matrix3<float> mat_A(
            2.0f, 1.0f, 3.0f,    // Riga 0
            0.0f, 4.0f, -1.0f,   // Riga 1
            5.0f, 2.0f, 0.0f,    // Riga 2
            MatOrder::ROW_MAJOR  // Forziamo l'uso della logica ROW_MAJOR
        );

        // Il determinante calcolato atteso è -61.0
        float expected_determinant = -61.0f;

        // Calcoliamo il determinante con il metodo della classe
        float actual_determinant = mat_A.Determinant();

        // Usiamo ASSERT_NEAR per i float/double, permettendo un piccolo errore di precisione
        ASSERT_NEAR(actual_determinant, expected_determinant, 1e-5)
            << "Il determinante calcolato non corrisponde al valore atteso.";
    }

    TEST(Matrix3Test, DeterminantIdentityMatrix)
    {
        // Test di base: Matrice Identità (Determinante = 1)
        Matrix3 identity(
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            MatOrder::ROW_MAJOR
        );

        ASSERT_NEAR(identity.Determinant(), 1.0f, 1e-5)
            << "Il determinante della matrice identità deve essere 1.";
    }

    TEST(Matrix3Test, MatrixMultiplication)
    {
        /*
        Matrix3<float> mat1(1.0f, 2.0f, 3.0f,
                           4.0f, 5.0f, 6.0f,
                           7.0f, 8.0f, 9.0f, MatOrder::ROW_MAJOR);

        Matrix3<float> mat2(9.0f, 8.0f, 7.0f,
                           6.0f, 5.0f, 4.0f,
                           3.0f, 2.0f, 1.0f, MatOrder::ROW_MAJOR);

        Matrix3<float> result = mat1 * mat2;

        EXPECT_EQ(result[0][0], 30.0f);
        EXPECT_EQ(result[0][1], 24.0f);
        EXPECT_EQ(result[0][2], 18.0f);
        EXPECT_EQ(result[1][0], 84.0f);
        EXPECT_EQ(result[1][1], 69.0f);
        EXPECT_EQ(result[1][2], 54.0f);
        EXPECT_EQ(result[2][0], 138.0f);
        EXPECT_EQ(result[2][1], 114.0f);
        EXPECT_EQ(result[2][2], 90.0f);
         */
    }

    TEST(Matrix3Test, MatrixVectorMultiplication)
    {
        Matrix3 mat(1.0f, 2.0f, 3.0f,
                          4.0f, 5.0f, 6.0f,
                          7.0f, 8.0f, 9.0f, MatOrder::ROW_MAJOR);

        Vector3 vec(1.0f, 2.0f, 3.0f);
        Vector3<float> result = mat * vec;

        EXPECT_EQ(result.GetX(), 14.0f);
        EXPECT_EQ(result.GetY(), 32.0f);
        EXPECT_EQ(result.GetZ(), 50.0f);
    }

    TEST(Matrix3Test, Transpose)
    {
        Matrix3 mat(1.0f, 2.0f, 3.0f,
                          4.0f, 5.0f, 6.0f,
                          7.0f, 8.0f, 9.0f);

        Matrix3<float> transposed = mat.Transpose();

        EXPECT_EQ(transposed[0][0], 1.0f);
        EXPECT_EQ(transposed[0][1], 4.0f);
        EXPECT_EQ(transposed[0][2], 7.0f);
        EXPECT_EQ(transposed[1][0], 2.0f);
        EXPECT_EQ(transposed[1][1], 5.0f);
        EXPECT_EQ(transposed[1][2], 8.0f);
        EXPECT_EQ(transposed[2][0], 3.0f);
        EXPECT_EQ(transposed[2][1], 6.0f);
        EXPECT_EQ(transposed[2][2], 9.0f);
    }

    TEST(Matrix3Test, Determinant)
    {
        Matrix3 mat(1.0f, 2.0f, 3.0f,
                          4.0f, 5.0f, 6.0f,
                          7.0f, 8.0f, 9.0f);

        float det = mat.Determinant();
        EXPECT_EQ(det, 0.0f);

        Matrix3 mat2(1.0f, 0.0f, 0.0f,
                           0.0f, 1.0f, 0.0f,
                           0.0f, 0.0f, 1.0f);

        float det2 = mat2.Determinant();
        EXPECT_EQ(det2, 1.0f);
    }

    TEST(Matrix3Test, Inverse)
    {
        Matrix3 mat(1.0f, 0.0f, 0.0f,
                          0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 1.0f);

        Matrix3<float> inverse = mat.Inverse();

        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
            {
                if(i == j)
                    EXPECT_EQ(inverse[i][j], 1.0f);
                else
                    EXPECT_EQ(inverse[i][j], 0.0f);
            }
    }

    TEST(Matrix3Test, RotationMatrix)
    {
        float angle = 90.0f * M_PI / 180.0f;
        Matrix3<float> rotMat = Matrix3<float>::Rotate(angle);

        EXPECT_FLOAT_EQ(rotMat[0][0], cos(angle));
        EXPECT_FLOAT_EQ(rotMat[0][1], -sin(angle));
        EXPECT_FLOAT_EQ(rotMat[0][2], 0.0f);
        EXPECT_FLOAT_EQ(rotMat[1][0], sin(angle));
        EXPECT_FLOAT_EQ(rotMat[1][1], cos(angle));
        EXPECT_FLOAT_EQ(rotMat[1][2], 0.0f);
        EXPECT_FLOAT_EQ(rotMat[2][0], 0.0f);
        EXPECT_FLOAT_EQ(rotMat[2][1], 0.0f);
        EXPECT_FLOAT_EQ(rotMat[2][2], 1.0f);
    }

    TEST(Matrix3Test, ScaleMatrix)
    {
        Vector2 scale(2.0f, 3.0f);
        Matrix3<float> scaleMat = Matrix3<float>::Scale(scale);
        EXPECT_EQ(scaleMat[0][0], 2.0f);
        EXPECT_EQ(scaleMat[1][1], 3.0f);
        EXPECT_EQ(scaleMat[2][2], 1.0f);
    }

    TEST(Matrix3Test, ComparisonOperators)
    {
        Matrix3 mat1(1.0f, 2.0f, 3.0f,
                           4.0f, 5.0f, 6.0f,
                           7.0f, 8.0f, 9.0f);

        Matrix3 mat2(1.0f, 2.0f, 3.0f,
                           4.0f, 5.0f, 6.0f,
                           7.0f, 8.0f, 9.0f);

        Matrix3 mat3(9.0f, 8.0f, 7.0f,
                           6.0f, 5.0f, 4.0f,
                           3.0f, 2.0f, 1.0f);

        EXPECT_TRUE(mat1 == mat2);
        EXPECT_FALSE(mat1 == mat3);
        EXPECT_TRUE(mat1 != mat3);
    }

    TEST(Matrix3Test, AccessOutOfRange)
    {
        Matrix3<float> mat{};
        EXPECT_THROW(mat[3][0], std::out_of_range);
        EXPECT_THROW(mat[0][3], std::out_of_range);
    }
}