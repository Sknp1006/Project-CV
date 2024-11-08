#include <gtest/gtest.h>
#include <string>
#include "utils.h"

/// @brief 测试 GetExecutablePath 函数
TEST(UtilsTest, GetExecutablePath)
{
    std::string path;
    GetExecutablePath(path);

    EXPECT_FALSE(path.empty());
}

/// @brief 测试 GetModulePath 函数
TEST(UtilsTest, GetModulePath)
{
    std::string path;
    GetModulePath(path);

    EXPECT_FALSE(path.empty());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}