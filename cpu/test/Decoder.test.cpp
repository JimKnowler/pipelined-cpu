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
    EXPECT_EQ(0, core.o_rs1);
    EXPECT_EQ(0, core.o_rs2);
    EXPECT_EQ(0, core.o_rd);
    EXPECT_EQ(0, core.o_i);
}

TEST_F(Decoder, ShouldDecodeLDA) {
    const uint8_t TestRegister = 12;
    const uint16_t TestAddress = 0xabcd;
    const uint32_t LDA = Assembler().LDA().r(TestRegister).i(TestAddress).AssembleCurrentOpcode();

    auto& core = testBench.core();
    core.i_ir = LDA;
    core.eval();

    EXPECT_EQ(static_cast<uint8_t>(Opcode::LDA), core.o_opcode);
    EXPECT_EQ(0, core.o_rs1);
    EXPECT_EQ(0, core.o_rs2);
    EXPECT_EQ(TestRegister, core.o_rd);
    EXPECT_EQ(TestAddress, core.o_i);
}

TEST_F(Decoder, ShouldDecodeSTA) {
    const uint8_t TestRegister = 14;
    const uint16_t TestAddress = 0xcdef;
    const uint32_t LDA = Assembler().STA().r(TestRegister).i(TestAddress).AssembleCurrentOpcode();

    auto& core = testBench.core();
    core.i_ir = LDA;
    core.eval();

    EXPECT_EQ(static_cast<uint8_t>(Opcode::STA), core.o_opcode);
    EXPECT_EQ(TestRegister, core.o_rs1);
    EXPECT_EQ(0, core.o_rs2);
    EXPECT_EQ(0, core.o_rd);
    EXPECT_EQ(TestAddress, core.o_i);
}

TEST_F(Decoder, ShouldDecodeADD) {
    const uint8_t TestRs1 = 8;
    const uint8_t TestRs2 = 7;
    const uint8_t TestRd = 6;
    const uint32_t ADD = Assembler().ADD().rs1(TestRs1).rs2(TestRs2).rd(TestRd).AssembleCurrentOpcode();

    auto& core = testBench.core();
    core.i_ir = ADD;
    core.eval();

    EXPECT_EQ(static_cast<uint8_t>(Opcode::ADD), core.o_opcode);
    EXPECT_EQ(TestRs1, core.o_rs1);
    EXPECT_EQ(TestRs2, core.o_rs2);
    EXPECT_EQ(TestRd, core.o_rd);
    EXPECT_EQ(0, core.o_i);
}

TEST_F(Decoder, ShouldDecodeSUB) {
    const uint8_t TestRs1 = 15;
    const uint8_t TestRs2 = 14;
    const uint8_t TestRd = 13;
    const uint32_t SUB = Assembler().SUB().rs1(TestRs1).rs2(TestRs2).rd(TestRd).AssembleCurrentOpcode();

    auto& core = testBench.core();
    core.i_ir = SUB;
    core.eval();

    EXPECT_EQ(static_cast<uint8_t>(Opcode::SUB), core.o_opcode);
    EXPECT_EQ(TestRs1, core.o_rs1);
    EXPECT_EQ(TestRs2, core.o_rs2);
    EXPECT_EQ(TestRd, core.o_rd);
    EXPECT_EQ(0, core.o_i);
}
