#include <gtest/gtest.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

#include "cpu/RegistersTestBench.h"
using namespace registerstestbench;

namespace {
    class Registers : public ::testing::Test {
    public:
        void SetUp() override {
            auto& core = testBench.core();
            core.i_we = 0;
        }
        
        void TearDown() override {
        }

        RegistersTestBench testBench;
    };
}

TEST_F(Registers, ShouldConstruct) {

}

TEST_F(Registers, ShouldReset) {
    testBench.reset();

    auto& core = testBench.core();

    for (int i=0; i<16; i++) {
        core.i_rs1 = i;
        core.i_rs2 = i;

        core.eval();

        EXPECT_EQ(0, core.o_rd1);
        EXPECT_EQ(0, core.o_rd2);
    }
}

TEST_F(Registers, ShouldWriteToSingleRegister) {
    auto& core = testBench.core();

    const uint32_t TestValue = 0x12345678;
    const uint8_t TestRegister = 12;

    core.i_we = 1;
    core.i_wd = TestValue;
    core.i_ws = TestRegister;
    testBench.tick();

    core.i_we = 0;

    for (int i=0; i<16; i++) {
        core.i_rs1 = i;
        core.i_rs2 = i;

        core.eval();

        if (i == TestRegister) {
            EXPECT_EQ(TestValue, core.o_rd1);
            EXPECT_EQ(TestValue, core.o_rd2);
        } else {
            EXPECT_EQ(0, core.o_rd1);
            EXPECT_EQ(0, core.o_rd2);
        }
    }
}

TEST_F(Registers, ShouldWriteToAllRegisters) {
    auto& core = testBench.core();

    core.i_we = 1;

    for (int i=0; i<16; i++) {
        core.i_ws = i;
        core.i_wd = i+1;
        testBench.tick();
    }

    core.i_we = 0;

    for (int i=0; i<16; i++) {
        core.i_rs1 = i;
        core.i_rs2 = i;

        core.eval();

        EXPECT_EQ(i+1, core.o_rd1);
        EXPECT_EQ(i+1, core.o_rd2);
    }
}


TEST_F(Registers, ShouldReadConcurrentlyFromDifferentRegisters) {
    auto& core = testBench.core();

    core.i_we = 1;

    for (int i=0; i<16; i++) {
        core.i_ws = i;
        core.i_wd = i+1;
        testBench.tick();
    }

    core.i_we = 0;

    for (int i=0; i<8; i++) {
        core.i_rs1 = 2*i;
        core.i_rs2 = (2*i) + 1;

        core.eval();

        EXPECT_EQ((2*i)+1, core.o_rd1);
        EXPECT_EQ(((2*i)+1)+1, core.o_rd2);
    }
}

