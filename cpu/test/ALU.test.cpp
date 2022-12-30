#include <gtest/gtest.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

#include "cpu/ALUTestBench.h"
using namespace alutestbench;

namespace {
    class ALU : public ::testing::Test {
    public:
        void SetUp() override {
        }
        
        void TearDown() override {
        }

        ALUTestBench testBench;
    };

    const uint8_t OP_PASSTHROUGH = 0;
    const uint8_t OP_ADD = 1;
    const uint8_t OP_SUB = 2;
}

TEST_F(ALU, ShouldConstruct) {

}

TEST_F(ALU, ShouldAdd) {
    auto& core = testBench.core();

    const uint32_t TestValue1 = 1024;
    const uint32_t TestValue2 = 432;

    core.i_op = OP_ADD;
    core.i_d1 = TestValue1;
    core.i_d2 = TestValue2;
    core.eval();

    EXPECT_EQ(TestValue1 + TestValue2, core.o_result);
}

TEST_F(ALU, ShouldSubtract) {
    auto& core = testBench.core();

    const uint32_t TestValue1 = 100;
    const uint32_t TestValue2 = 25;

    core.i_op = OP_SUB;
    core.i_d1 = TestValue1;
    core.i_d2 = TestValue2;
    core.eval();

    EXPECT_EQ(TestValue1 - TestValue2, core.o_result);
}

TEST_F(ALU, ShouldPassThrough) {
    auto& core = testBench.core();

    const uint32_t TestValue1 = 200;
    const uint32_t TestValue2 = 50;

    core.i_op = OP_PASSTHROUGH;
    core.i_d1 = TestValue1;
    core.i_d2 = TestValue2;
    core.eval();

    EXPECT_EQ(TestValue1, core.o_result);
}
