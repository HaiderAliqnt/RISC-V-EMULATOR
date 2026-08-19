extern "C" {
    #include "memory.h"
    #include "cpu.h"
    #include "decoder.h"
    #include "executor.h"
}
#include <gtest/gtest.h>

class ExecutorTest : public ::testing::Test {
protected:
    CPU cpu;
    void SetUp() override {
        memory_init();
        cpu_init(&cpu);
    }
    void TearDown() override {
        memory_free();
    }
};

TEST_F(ExecutorTest, Placeholder) {
    EXPECT_EQ(1, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
