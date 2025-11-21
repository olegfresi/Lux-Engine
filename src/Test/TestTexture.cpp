#include <gtest/gtest.h>
#include "../../include/Renderer/Texture/Texture2D.hpp"

/*
class TextureTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_Texture = std::make_unique<Texture2D>("assets/mia.png", 2, 2, 3);
    }
    
    void TearDown() override
    {
        m_Texture.reset();
    }
    
    std::unique_ptr<Texture2D> m_Texture;
};

TEST_F(TextureTest, TextureCreation)
{
    EXPECT_TRUE(m_Texture != nullptr);
    EXPECT_NE(m_Texture->GetWidth(), 0);
    EXPECT_NE(m_Texture->GetHeight(), 0);
}

TEST_F(TextureTest, TextureDimensions)
{
    EXPECT_EQ(m_Texture->GetWidth(), 2);
    EXPECT_EQ(m_Texture->GetHeight(), 2);
}

TEST_F(TextureTest, TextureBinding)
{
    EXPECT_NO_THROW(m_Texture->Bind());
    EXPECT_NO_THROW(m_Texture->Unbind());
    
    EXPECT_NO_THROW(m_Texture->Bind(0));
    EXPECT_NO_THROW(m_Texture->Unbind());
}

TEST_F(TextureTest, MipmapGeneration)
{
    EXPECT_NO_THROW(m_Texture->GenerateMipmaps());
}

TEST_F(TextureTest, TextureDrawing)
{
    EXPECT_NO_THROW(m_Texture->Draw());
}

TEST_F(TextureTest, MultipleBindings)
{
    for (int i = 0; i < 3; i++)
    {
        m_Texture->Bind(i);
        m_Texture->Unbind();
    }
}

TEST_F(TextureTest, TextureFormat)
{
    m_Texture->Bind();
    m_Texture->Unbind();
}

TEST_F(TextureTest, TextureDataAccess)
{
    m_Texture->Bind();
    m_Texture->Unbind();
}

TEST_F(TextureTest, TextureCleanup)
{
    auto* texture = new Texture2D("test/textures/test.png", 2, 2, 3);
    EXPECT_NO_THROW(delete texture);
}

TEST_F(TextureTest, InvalidTexturePath)
{
    EXPECT_THROW(Texture2D("invalid/path.png", 2, 2, 3), std::runtime_error);
}

TEST_F(TextureTest, ZeroDimensions)
{
    EXPECT_THROW(Texture2D("test/textures/test.png", 0, 2, 3), std::invalid_argument);
    EXPECT_THROW(Texture2D("test/textures/test.png", 2, 0, 3), std::invalid_argument);
}

TEST_F(TextureTest, InvalidChannels)
{
    EXPECT_THROW(Texture2D("test/textures/test.png", 2, 2, 0), std::invalid_argument);
    EXPECT_THROW(Texture2D("test/textures/test.png", 2, 2, 5), std::invalid_argument);
}
*/