#include <gtest/gtest.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

#include "cpu/CPUTestBench.h"
using namespace cputestbench;

#include "memory/SRAM.hpp"
#include "assembler/Assembler.h"

namespace {
    class CPU : public ::testing::Test {
    public:
        CPU() : instructionMemory(64 * 1024), dataMemory(64 * 1024) {}

        void SetUp() override {
            dataMemory.clear();
            instructionMemory.clear();

            testBench.setCallbackSimulateCombinatorial( [&] {
                auto& core = testBench.core();

                // instruction bus
                const size_t pc = core.o_pc;
                core.i_instruction = instructionMemory.read32(pc);

                // data bus
                const size_t address = core.o_address;
                if (core.o_rw == RW_WRITE) {
                    // write
                    dataMemory.write(address, core.o_data);
                } else {
                    // read
                    core.i_data = dataMemory.read32(address);
                }
            });
        }
        
        void TearDown() override {
        }

        void HelperReset() {
            testBench.reset();
            testBench.trace.clear();
        }

        CPUTestBench testBench;

        memory::SRAM instructionMemory;
        memory::SRAM dataMemory;

        enum {
            RW_WRITE = 1,
            RW_READ = 0
        };
    };
}

TEST_F(CPU, ShouldConstruct) {

}

TEST_F(CPU, ShouldReset) {
    HelperReset();

    auto& core = testBench.core();

    EXPECT_EQ(0, core.o_pc);
}

TEST_F(CPU, ShouldSequentiallyIncrementPC) {
    HelperReset();

    auto& core = testBench.core();

    const int kNumTicks = 4;

    for (int i=0; i<kNumTicks; i++) {
        EXPECT_EQ((i*4), core.o_pc);
        testBench.tick();
    }
}

TEST_F(CPU, ShouldLoadAndStoreWithoutPipelineHazards) {
    HelperReset();

    const uint8_t kTestReg = 12;
    const uint16_t kTestAddressSrc = 0x1234;
    const uint16_t kTestAddressDst = 0xabcd;
    const uint32_t kTestData = 0x12345678;

    dataMemory.write(kTestAddressSrc, kTestData);

    assembler::Assembler assembler;
    assembler
        .LW().rd(kTestReg).i(kTestAddressSrc)
        .NOP()
        .NOP()
        .NOP()
        .NOP()
        .SW().rs2(kTestReg).i(kTestAddressDst)
        .NOP()
        .NOP()
        .NOP()
        .NOP();

    assembler.Assemble(instructionMemory);

    testBench.tick(10);

    EXPECT_EQ(kTestData, dataMemory.read32(kTestAddressDst));
}

TEST_F(CPU, ShouldAddWithoutPipelineHazards) {
    HelperReset();

    const uint8_t kTestReg1 = 5;
    const uint16_t kTestAddressSrc1 = 0x1234;
    const uint32_t kTestData1 = 0x12345678;

    const uint8_t kTestReg2= 7;
    const uint16_t kTestAddressSrc2 = 0x4321;
    const uint32_t kTestData2 = 0x00000001;

    const uint8_t kTestRegDst = 11; 
    const uint16_t kTestAddressDst = 0xabcd;

    dataMemory.write(kTestAddressSrc1, kTestData1);
    dataMemory.write(kTestAddressSrc2, kTestData2);

    assembler::Assembler assembler;
    assembler
        .LW().rd(kTestReg1).i(kTestAddressSrc1)
        .LW().rd(kTestReg2).i(kTestAddressSrc2)
        .NOP()
        .NOP()
        .NOP()
        .NOP()
        .ADD().rd(kTestRegDst).rs1(kTestReg1).rs2(kTestReg2)
        .NOP()
        .NOP()
        .NOP()
        .NOP()
        .SW().rs2(kTestRegDst).i(kTestAddressDst)
        .NOP()
        .NOP()
        .NOP()
        .NOP();

    assembler.Assemble(instructionMemory);

    testBench.tick(16);

    EXPECT_EQ(kTestData1 + kTestData2, dataMemory.read32(kTestAddressDst));
}

TEST_F(CPU, ShouldSubWithoutPipelineHazards) {
    HelperReset();

    const uint8_t kTestReg1 = 5;
    const uint16_t kTestAddressSrc1 = 0x1234;
    const uint32_t kTestData1 = 0x12345678;

    const uint8_t kTestReg2= 7;
    const uint16_t kTestAddressSrc2 = 0x4321;
    const uint32_t kTestData2 = 0x00000001;

    const uint8_t kTestRegDst = 11; 
    const uint16_t kTestAddressDst = 0xabcd;

    dataMemory.write(kTestAddressSrc1, kTestData1);
    dataMemory.write(kTestAddressSrc2, kTestData2);

    assembler::Assembler assembler;
    assembler
        .LW().rd(kTestReg1).i(kTestAddressSrc1)
        .LW().rd(kTestReg2).i(kTestAddressSrc2)
        .NOP()
        .NOP()
        .NOP()
        .NOP()
        .SUB().rd(kTestRegDst).rs1(kTestReg1).rs2(kTestReg2)
        .NOP()
        .NOP()
        .NOP()
        .NOP()
        .SW().rs2(kTestRegDst).i(kTestAddressDst)
        .NOP()
        .NOP()
        .NOP()
        .NOP();

    assembler.Assemble(instructionMemory);

    testBench.tick(16);

    EXPECT_EQ(kTestData1 - kTestData2, dataMemory.read32(kTestAddressDst));
}

TEST_F(CPU, ShouldAddWithPipelineRegisterHazards) {
    HelperReset();

    const uint8_t kTestReg1 = 5;
    const uint16_t kTestAddressSrc1 = 0x1234;
    const uint32_t kTestData1 = 0x12345678;

    const uint8_t kTestReg2= 7;
    const uint16_t kTestAddressSrc2 = 0x4321;
    const uint32_t kTestData2 = 0x00000001;

    const uint8_t kTestRegDst = 11; 
    const uint16_t kTestAddressDst = 0xabcd;

    dataMemory.write(kTestAddressSrc1, kTestData1);
    dataMemory.write(kTestAddressSrc2, kTestData2);

    assembler::Assembler assembler;
    assembler
        .LW().rd(kTestReg1).i(kTestAddressSrc1)
        .LW().rd(kTestReg2).i(kTestAddressSrc2)
        .ADD().rd(kTestRegDst).rs1(kTestReg1).rs2(kTestReg2)
        .SW().rs2(kTestRegDst).i(kTestAddressDst)
        .NOP()
        .NOP()
        .NOP()
        .NOP();

    assembler.Assemble(instructionMemory);

    testBench.tick(16);

    EXPECT_EQ(kTestData1 + kTestData2, dataMemory.read32(kTestAddressDst));
}

TEST_F(CPU, ShouldBypassFromMemoryToDecoderRs2) {
    HelperReset();

    const uint8_t kTestReg = 5;
    const uint16_t kTestAddressSrc = 0x1234;
    const uint32_t kTestData = 0x12345678;
    const uint16_t kTestAddressDst = 0xabcd;

    dataMemory.write(kTestAddressSrc, kTestData);

    assembler::Assembler assembler;
    assembler
        .LW().rd(kTestReg).i(kTestAddressSrc)
        .SW().rs2(kTestReg).i(kTestAddressDst)
        .NOP()
        .NOP()
        .NOP();

    assembler.Assemble(instructionMemory);

    testBench.tick(5);

    EXPECT_EQ(kTestData, dataMemory.read32(kTestAddressDst));
}

TEST_F(CPU, ShouldBypassFromMemoryToDecoderRs1) {
    HelperReset();

    const uint8_t kTestReg1 = 5;
    const uint16_t kTestAddressSrc1 = 0x1234;
    const uint32_t kTestData1 = 0x12345678;

    const uint8_t kTestReg2 = 8;
    const uint16_t kTestAddressSrc2 = 0x1238;
    const uint32_t kTestData2 = 0x00000001;

    const uint16_t kTestAddressDst = 0xabcd;

    dataMemory.write(kTestAddressSrc1, kTestData1);
    dataMemory.write(kTestAddressSrc2, kTestData2);

    assembler::Assembler assembler;
    assembler
        .LW().rd(kTestReg2).i(kTestAddressSrc2)
        .LW().rd(kTestReg1).i(kTestAddressSrc1)
        .ADD().rs1(kTestReg1).rs2(kTestReg2).rd(kTestReg1)
        .NOP()
        .NOP()
        .NOP()
        .NOP()
        .SW().rs2(kTestReg1).i(kTestAddressDst)
        .NOP()
        .NOP()
        .NOP();

    assembler.Assemble(instructionMemory);

    testBench.tick(11);

    EXPECT_EQ(kTestData1 + kTestData2, dataMemory.read32(kTestAddressDst));
}

TEST_F(CPU, ShouldBypassFromExecuteToDecodeRs1ForAdd) {
    HelperReset();

    const uint8_t kTestReg1 = 5;
    const uint16_t kTestAddressSrc1 = 0x1234;
    const uint32_t kTestData1 = 0x12345678;

    const uint8_t kTestReg2 = 8;
    const uint16_t kTestAddressSrc2 = 0x1238;
    const uint32_t kTestData2 = 0x00000001;

    const uint16_t kTestAddressDst = 0xabcd;

    dataMemory.write(kTestAddressSrc1, kTestData1);
    dataMemory.write(kTestAddressSrc2, kTestData2);

    assembler::Assembler assembler;
    assembler
        .LW().rd(kTestReg2).i(kTestAddressSrc2)
        .LW().rd(kTestReg1).i(kTestAddressSrc1)
        .ADD().rs1(kTestReg1).rs2(kTestReg2).rd(kTestReg1)
        .ADD().rs1(kTestReg1).rs2(kTestReg2).rd(kTestReg1)
        .SW().rs2(kTestReg1).i(kTestAddressDst)
        .NOP()
        .NOP()
        .NOP();

    assembler.Assemble(instructionMemory);

    testBench.tick(8);

    EXPECT_EQ(kTestData1 + kTestData2 + kTestData2, dataMemory.read32(kTestAddressDst));
}

TEST_F(CPU, ShouldBypassFromExecuteToDecodeRs1ForSub) {
    HelperReset();

    const uint8_t kTestReg1 = 5;
    const uint16_t kTestAddressSrc1 = 0x1234;
    const uint32_t kTestData1 = 0x12345678;

    const uint8_t kTestReg2 = 8;
    const uint16_t kTestAddressSrc2 = 0x1238;
    const uint32_t kTestData2 = 0x00000001;

    const uint16_t kTestAddressDst = 0xabcd;

    dataMemory.write(kTestAddressSrc1, kTestData1);
    dataMemory.write(kTestAddressSrc2, kTestData2);

    assembler::Assembler assembler;
    assembler
        .LW().rd(kTestReg2).i(kTestAddressSrc2)
        .LW().rd(kTestReg1).i(kTestAddressSrc1)
        .SUB().rs1(kTestReg1).rs2(kTestReg2).rd(kTestReg1)
        .SUB().rs1(kTestReg1).rs2(kTestReg2).rd(kTestReg1)
        .SW().rs2(kTestReg1).i(kTestAddressDst)
        .NOP()
        .NOP()
        .NOP();

    assembler.Assemble(instructionMemory);

    testBench.tick(8);

    EXPECT_EQ(kTestData1 - kTestData2 - kTestData2, dataMemory.read32(kTestAddressDst));
}

TEST_F(CPU, ShouldBypassFromExecuteToDecodeRs2ForAdd) {
    HelperReset();

    const uint8_t kTestReg1 = 5;
    const uint16_t kTestAddressSrc1 = 0x1234;
    const uint32_t kTestData1 = 0x12345678;

    const uint8_t kTestReg2 = 8;
    const uint16_t kTestAddressSrc2 = 0x1238;
    const uint32_t kTestData2 = 0x00000001;

    const uint16_t kTestAddressDst = 0xabcd;

    dataMemory.write(kTestAddressSrc1, kTestData1);
    dataMemory.write(kTestAddressSrc2, kTestData2);

    assembler::Assembler assembler;
    assembler
        .LW().rd(kTestReg2).i(kTestAddressSrc2)
        .LW().rd(kTestReg1).i(kTestAddressSrc1)
        .ADD().rs1(kTestReg1).rs2(kTestReg2).rd(kTestReg1)
        .ADD().rs1(kTestReg2).rs2(kTestReg1).rd(kTestReg1)
        .SW().rs2(kTestReg1).i(kTestAddressDst)
        .NOP()
        .NOP()
        .NOP();

    assembler.Assemble(instructionMemory);

    testBench.tick(8);

    EXPECT_EQ(kTestData1 + kTestData2 + kTestData2, dataMemory.read32(kTestAddressDst));
}

TEST_F(CPU, ShouldBypassFromExecuteToDecodeRs2ForSub) {
    HelperReset();

    const uint8_t kTestReg1 = 5;
    const uint16_t kTestAddressSrc1 = 0x1234;
    const uint32_t kTestData1 = 10;

    const uint8_t kTestReg2 = 8;
    const uint16_t kTestAddressSrc2 = 0x1238;
    const uint32_t kTestData2 = 2;

    const uint16_t kTestAddressDst = 0xabcd;

    dataMemory.write(kTestAddressSrc1, kTestData1);
    dataMemory.write(kTestAddressSrc2, kTestData2);

    assembler::Assembler assembler;
    assembler
        .LW().rd(kTestReg2).i(kTestAddressSrc2)
        .LW().rd(kTestReg1).i(kTestAddressSrc1)
        .SUB().rs1(kTestReg1).rs2(kTestReg2).rd(kTestReg2)
        .SUB().rs1(kTestReg1).rs2(kTestReg2).rd(kTestReg1)
        .SW().rs2(kTestReg1).i(kTestAddressDst)
        .NOP()
        .NOP()
        .NOP();

    assembler.Assemble(instructionMemory);

    testBench.tick(8);

    EXPECT_EQ(kTestData1 - (kTestData1 - kTestData2), dataMemory.read32(kTestAddressDst));
}
