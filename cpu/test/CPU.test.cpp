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

TEST_F(CPU, ShouldLoadAndStoreWithoutHazards) {
    HelperReset();

    const uint8_t kTestReg = 12;
    const uint16_t kTestAddressSrc = 0x1234;
    const uint16_t kTestAddressDst = 0xabcd;
    const uint32_t kTestData = 0x12345678;

    dataMemory.write(kTestAddressSrc, kTestData);

    assembler::Assembler assembler;
    assembler
        .LDA().r(kTestReg).i(kTestAddressSrc)
        .NOP()
        .NOP()
        .NOP()
        .NOP()
        .STA().r(kTestReg).i(kTestAddressDst)
        .NOP()
        .NOP()
        .NOP()
        .NOP();

    assembler.Assemble(instructionMemory);

    const int kNumTicks = 10;

    for (int i=0; i<kNumTicks; i++) {
        testBench.tick();
    }

    EXPECT_EQ(kTestData, dataMemory.read32(kTestAddressDst));
}
