#include <gtest/gtest.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

#include "cpu/StallControlTestBench.h"
using namespace stallcontroltestbench;

#include "assembler/Opcode.h"

namespace {
    class StallControl : public ::testing::Test {
    public:
        void SetUp() override {
            testBench.reset();
        }
        
        void TearDown() override {
        }

        StallControlTestBench testBench;
    };
}

TEST_F(StallControl, ShouldConstruct) {

}

TEST_F(StallControl, ShouldStallWhenRs1ReadByDecoderAndWrittenByExecuteStageDuringLW) {
    auto& core = testBench.core();

    const uint8_t kTestRs1 = 12;
    const uint8_t kTestRs2 = 13;

    core.i_decoder_re1 = 1;
    core.i_decoder_rs1 = kTestRs1;
    core.i_decoder_re2 = 1;
    core.i_decoder_rs2 = kTestRs2;

    core.i_execute_ws = kTestRs1;
    core.i_execute_opcode = static_cast<uint8_t>(assembler::Opcode::LW);

    core.eval();

    EXPECT_EQ(1, core.o_stall);
}

TEST_F(StallControl, ShouldStallWhenRs2ReadByDecoderAndWrittenByExecuteStageDuringLW) {
    auto& core = testBench.core();

    const uint8_t kTestRs1 = 12;
    const uint8_t kTestRs2 = 13;

    core.i_decoder_re1 = 1;
    core.i_decoder_rs1 = kTestRs1;
    core.i_decoder_re2 = 1;
    core.i_decoder_rs2 = kTestRs2;

    core.i_execute_ws = kTestRs2;
    core.i_execute_opcode = static_cast<uint8_t>(assembler::Opcode::LW);

    core.eval();

    EXPECT_EQ(1, core.o_stall);
}

TEST_F(StallControl, ShouldNotStallWhenRs1WrittenByExecuteStageDuringADD) {
    auto& core = testBench.core();

    const uint8_t kTestRs1 = 12;
    const uint8_t kTestRs2 = 13;

    core.i_decoder_re1 = 1;
    core.i_decoder_rs1 = kTestRs1;
    core.i_decoder_re2 = 1;
    core.i_decoder_rs2 = kTestRs2;

    core.i_execute_ws = kTestRs1;
    core.i_execute_opcode = static_cast<uint8_t>(assembler::Opcode::ADD);

    core.eval();

    EXPECT_EQ(0, core.o_stall);
}

TEST_F(StallControl, ShouldNotStallWhenRs2WrittenByExecuteStageDuringADD) {
    auto& core = testBench.core();

    const uint8_t kTestRs1 = 12;
    const uint8_t kTestRs2 = 13;

    core.i_decoder_re1 = 1;
    core.i_decoder_rs1 = kTestRs1;
    core.i_decoder_re2 = 1;
    core.i_decoder_rs2 = kTestRs2;

    core.i_execute_ws = kTestRs2;
    core.i_execute_opcode = static_cast<uint8_t>(assembler::Opcode::ADD);

    core.eval();

    EXPECT_EQ(0, core.o_stall);
}

TEST_F(StallControl, ShouldNotStallWhenRs1WrittenByExecuteStageDuringLWButDecoderReadDisabled) {
    auto& core = testBench.core();

    const uint8_t kTestRs1 = 12;
    const uint8_t kTestRs2 = 13;

    core.i_decoder_re1 = 0;
    core.i_decoder_rs1 = kTestRs1;
    core.i_decoder_re2 = 1;
    core.i_decoder_rs2 = kTestRs2;

    core.i_execute_ws = kTestRs1;
    core.i_execute_opcode = static_cast<uint8_t>(assembler::Opcode::LW);

    core.eval();

    EXPECT_EQ(0, core.o_stall);
}

TEST_F(StallControl, ShouldStallWhenRs2WrittenByExecuteStageDuringLWButDecoderReadDisabled) {
    auto& core = testBench.core();

    const uint8_t kTestRs1 = 12;
    const uint8_t kTestRs2 = 13;

    core.i_decoder_re1 = 1;
    core.i_decoder_rs1 = kTestRs1;
    core.i_decoder_re2 = 0;
    core.i_decoder_rs2 = kTestRs2;

    core.i_execute_ws = kTestRs2;
    core.i_execute_opcode = static_cast<uint8_t>(assembler::Opcode::LW);

    core.eval();

    EXPECT_EQ(0, core.o_stall);
}


TEST_F(StallControl, ShouldNotStallWhenExecutingLWThatWritesToRegisterThatDecoderIsNotReadingFrom) {
    auto& core = testBench.core();

    const uint8_t kTestRs1 = 12;
    const uint8_t kTestRs2 = 13;

    core.i_decoder_re1 = 1;
    core.i_decoder_rs1 = kTestRs1;
    core.i_decoder_re2 = 1;
    core.i_decoder_rs2 = kTestRs1;

    core.i_execute_ws = kTestRs2;
    core.i_execute_opcode = static_cast<uint8_t>(assembler::Opcode::LW);

    core.eval();

    EXPECT_EQ(0, core.o_stall);
}
