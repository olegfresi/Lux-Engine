#include <gtest/gtest.h>
#include <../../include/Application/UUID.hpp>
#include <random>
#include <memory>
/*
class UUIDTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        std::mt19937 gen;
        m_generator = std::make_unique<uuids::uuid_random_generator>(gen);
    }

    void TearDown() override
    {
        m_generator.reset();
    }

    std::unique_ptr<uuids::uuid_random_generator> m_generator;
};

TEST_F(UUIDTest, Creation)
{
    uuids::uuid id = (*m_generator)();
    EXPECT_FALSE(id.is_nil());
}

TEST_F(UUIDTest, UniqueGeneration)
{
    uuids::uuid id1 = (*m_generator)();
    uuids::uuid id2 = (*m_generator)();
    EXPECT_NE(id1, id2);
}

TEST_F(UUIDTest, StringConversion)
{
    std::string uuid_str = "550e8400-e29b-41d4-a716-446655440000";
    auto id = uuids::uuid::from_string(uuid_str);
    EXPECT_TRUE(id.has_value());
    EXPECT_EQ(uuids::to_string(id.value()), uuid_str);
}

TEST_F(UUIDTest, Validation)
{
    EXPECT_TRUE(uuids::uuid::is_valid_uuid("550e8400-e29b-41d4-a716-446655440000"));
    
    EXPECT_FALSE(uuids::uuid::is_valid_uuid("invalid-uuid"));
    EXPECT_FALSE(uuids::uuid::is_valid_uuid("550e8400-e29b-41d4-a716-44665544000"));
    EXPECT_FALSE(uuids::uuid::is_valid_uuid("550e8400-e29b-41d4-a716-44665544000g"));
}

TEST_F(UUIDTest, Variants)
{
    uuids::uuid id = (*m_generator)();
    EXPECT_EQ(id.variant(), uuids::uuid_variant::rfc);
}

TEST_F(UUIDTest, Versions)
{
    uuids::uuid id = (*m_generator)();
    EXPECT_EQ(id.version(), uuids::uuid_version::random_number_based);
}

TEST_F(UUIDTest, Comparison)
{
    uuids::uuid id1 = (*m_generator)();
    uuids::uuid id2 = (*m_generator)();
    uuids::uuid id3 = id1;

    EXPECT_NE(id1, id2);
    EXPECT_EQ(id1, id3);
    EXPECT_TRUE(id1 < id2 || id2 < id1);
}

TEST_F(UUIDTest, Swap)
{
    uuids::uuid id1 = (*m_generator)();
    uuids::uuid id2 = (*m_generator)();
    
    uuids::uuid id1_copy = id1;
    uuids::uuid id2_copy = id2;
    
    id1.swap(id2);
    
    EXPECT_EQ(id1, id2_copy);
    EXPECT_EQ(id2, id1_copy);
}

TEST_F(UUIDTest, NilUUID)
{
    uuids::uuid nil;
    EXPECT_TRUE(nil.is_nil());
    EXPECT_EQ(uuids::to_string(nil), "00000000-0000-0000-0000-000000000000");
}

TEST_F(UUIDTest, MassGeneration)
{
    std::set<uuids::uuid> ids;
    for(int i = 0; i < 1000; ++i) {
        ids.insert((*m_generator)());
    }
    EXPECT_EQ(ids.size(), 1000);
}
 */