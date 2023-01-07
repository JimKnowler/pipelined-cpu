#include <gtest/gtest.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

#include "cpu/RegisterFileTestBench.h"
using namespace registerfiletestbench;

namespace {
    class RegisterFile : public ::testing::Test {
    public:
        void SetUp() override {
            auto& core = testBench.core();
            core.i_we = 0;
        }
        
        void TearDown() override {
        }

        RegisterFileTestBench testBench;
    };
}

TEST_F(RegisterFile, ShouldConstruct) {

}

TEST_F(RegisterFile, ShouldReset) {
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

TEST_F(RegisterFile, ShouldWriteToSingleRegister) {
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

TEST_F(RegisterFile, ShouldWriteToAllRegisters) {
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


TEST_F(RegisterFile, ShouldReadConcurrentlyFromMultipleRegisters) {
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

TEST_F(RegisterFile, ShouldBypassReadRegisterFromWriteRegister) {
    testBench.reset();

    auto& core = testBench.core();

    const int kTestRegister = 12;
    const int kZeroRegister = 13;          // we expect this register to hold value of 0 after reset

    const int kTestValue = 0x12345678;

    // prepare write to kTestRegister
    core.i_we = 1;
    core.i_ws = kTestRegister;
    core.i_wd = kTestValue;

    // read from kTestRegister, only on rs1
    core.i_rs1 = kTestRegister;
    core.i_rs2 = kZeroRegister;
    core.eval();
    EXPECT_EQ(kTestValue, core.o_rd1);
    EXPECT_EQ(0, core.o_rd2);

    // read from kTestRegister, only on rs2
    core.i_rs1 = kZeroRegister;
    core.i_rs2 = kTestRegister;
    core.eval();
    EXPECT_EQ(0, core.o_rd1);
    EXPECT_EQ(kTestValue, core.o_rd2);

    // read from kTestRegister, on both rs1 and rs2
    core.i_rs1 = kTestRegister;
    core.i_rs2 = kTestRegister;
    core.eval();
    EXPECT_EQ(kTestValue, core.o_rd1);
    EXPECT_EQ(kTestValue, core.o_rd2);
}

TEST_F(RegisterFile, ShouldNotBypassReadRegisterFromWriteRegisterWhenWriteDisabled) {
    testBench.reset();

    auto& core = testBench.core();

    const int kTestRegister = 12;
    const int kZeroRegister = 13;          // we expect this register to hold value of 0 after reset

    const int kTestValue = 0x12345678;

    // prepare write to kTestRegister, but leave Write DISABLED
    core.i_we = 0;
    core.i_ws = kTestRegister;
    core.i_wd = kTestValue;

    // write selector matches rs1, but write is disabled
    core.i_rs1 = kTestRegister;
    core.i_rs2 = kZeroRegister;
    core.eval();
    EXPECT_EQ(0, core.o_rd1);
    EXPECT_EQ(0, core.o_rd2);

    // write selector matches rs2, but write is disabled
    core.i_rs1 = kZeroRegister;
    core.i_rs2 = kTestRegister;
    core.eval();
    EXPECT_EQ(0, core.o_rd1);
    EXPECT_EQ(0, core.o_rd2);

    // write selector matches both read selectors, but write is disabled
    core.i_rs1 = kTestRegister;
    core.i_rs2 = kTestRegister;
    core.eval();
    EXPECT_EQ(0, core.o_rd1);
    EXPECT_EQ(0, core.o_rd2);
}