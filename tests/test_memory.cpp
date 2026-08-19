extern "C" {
    #include "memory.h"
}
#include <gtest/gtest.h>

class MemoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        memory_init();
    }
    void TearDown() override {
        memory_free();
    }
};

TEST_F(MemoryTest, Placeholder) {
    EXPECT_EQ(1, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
