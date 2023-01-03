#include <gtest/gtest.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

#include "cpu/DecoderTestBench.h"
using namespace decodertestbench;

#include "assembler/Assembler.h"
using namespace assembler;

namespace {
    class Decoder : public ::testing::Test {
    public:
        void SetUp() override {
            //testBench.setClockPolarity(1);
            //testBench.core().i_clk_en = 1;
        }
        
        void TearDown() override {
        }

        void HelperReset() {
            testBench.reset();
        }

        DecoderTestBench testBench;
    };
}

TEST_F(Decoder, ShouldConstruct) {
}

TEST_F(Decoder, ShouldDecodeNOP) {
    const uint8_t TestRegister = 0xff;
    const uint16_t TestAddress = 0xabcd;
    uint32_t NOP = Assembler().NOP().r(TestRegister).i(TestAddress).AssembleCurrentOpcode();

    auto& core = testBench.core();
    core.i_ir = NOP;
    core.eval();

    EXPECT_EQ(static_cast<uint8_t>(Opcode::NOP), core.o_opcode);
    EXPECT_EQ(0, core.o_re1);
    EXPECT_EQ(0, core.o_rs1);
    EXPECT_EQ(0, core.o_re2);
    EXPECT_EQ(0, core.o_rs2);
    EXPECT_EQ(0, core.o_we);
    EXPECT_EQ(0, core.o_ws);
    EXPECT_EQ(0, core.o_ie);
    EXPECT_EQ(0, core.o_id);
}

TEST_F(Decoder, ShouldDecodeLW) {
    const uint8_t TestRegisterSrc = 15;
    const uint8_t TestRegisterDest = 12;
    const uint16_t TestAddress = 0xabcd;
    const uint32_t LW = Assembler().LW().rd(TestRegisterDest).rs1(TestRegisterSrc).i(TestAddress).AssembleCurrentOpcode();

    auto& core = testBench.core();
    core.i_ir = LW;
    core.eval();

    EXPECT_EQ(static_cast<uint8_t>(Opcode::LW), core.o_opcode);
    EXPECT_EQ(1, core.o_re1);
    EXPECT_EQ(TestRegisterSrc, core.o_rs1);
    EXPECT_EQ(0, core.o_re2);
    EXPECT_EQ(0, core.o_rs2);
    EXPECT_EQ(1, core.o_we);
    EXPECT_EQ(TestRegisterDest, core.o_ws);
    EXPECT_EQ(1, core.o_ie);
    EXPECT_EQ(TestAddress, core.o_id);
}

TEST_F(Decoder, ShouldDecodeSW) {
    const uint8_t TestRegisterSrc1 = 14;
    const uint8_t TestRegisterSrc2 = 12;
    const uint16_t TestAddress = 0xcdef;
    const uint32_t SW = Assembler().SW().rs1(TestRegisterSrc1).rs2(TestRegisterSrc2).i(TestAddress).AssembleCurrentOpcode();

    auto& core = testBench.core();
    core.i_ir = SW;
    core.eval();

    EXPECT_EQ(static_cast<uint8_t>(Opcode::SW), core.o_opcode);
    EXPECT_EQ(1, core.o_re1);
    EXPECT_EQ(TestRegisterSrc1, core.o_rs1);
    EXPECT_EQ(1, core.o_re2);
    EXPECT_EQ(TestRegisterSrc2, core.o_rs2);
    EXPECT_EQ(0, core.o_we);
    EXPECT_EQ(0, core.o_ws);
    EXPECT_EQ(1, core.o_ie);
    EXPECT_EQ(TestAddress, core.o_id);
}

TEST_F(Decoder, ShouldDecodeADD) {
    const uint8_t TestRs1 = 8;
    const uint8_t TestRs2 = 7;
    const uint8_t TestWs = 6;
    const uint32_t ADD = Assembler().ADD().rs1(TestRs1).rs2(TestRs2).rd(TestWs).AssembleCurrentOpcode();

    auto& core = testBench.core();
    core.i_ir = ADD;
    core.eval();

    EXPECT_EQ(static_cast<uint8_t>(Opcode::ADD), core.o_opcode);
    EXPECT_EQ(1, core.o_re1);
    EXPECT_EQ(TestRs1, core.o_rs1);
    EXPECT_EQ(1, core.o_re2);
    EXPECT_EQ(TestRs2, core.o_rs2);
    EXPECT_EQ(1, core.o_we);
    EXPECT_EQ(TestWs, core.o_ws);
    EXPECT_EQ(0, core.o_ie);
    EXPECT_EQ(0, core.o_id);
}

TEST_F(Decoder, ShouldDecodeSUB) {
    const uint8_t TestRs1 = 15;
    const uint8_t TestRs2 = 14;
    const uint8_t TestWs = 13;
    const uint32_t SUB = Assembler().SUB().rs1(TestRs1).rs2(TestRs2).rd(TestWs).AssembleCurrentOpcode();

    auto& core = testBench.core();
    core.i_ir = SUB;
    core.eval();

    EXPECT_EQ(static_cast<uint8_t>(Opcode::SUB), core.o_opcode);
    EXPECT_EQ(1, core.o_re1);
    EXPECT_EQ(TestRs1, core.o_rs1);
    EXPECT_EQ(1, core.o_re2);
    EXPECT_EQ(TestRs2, core.o_rs2);
    EXPECT_EQ(1, core.o_we);
    EXPECT_EQ(TestWs, core.o_ws);
    EXPECT_EQ(0, core.o_ie);
    EXPECT_EQ(0, core.o_id);
}
