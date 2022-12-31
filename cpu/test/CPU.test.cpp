#include <gtest/gtest.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

#include "cpu/CPUTestBench.h"
using namespace cputestbench;

#include "memory/SRAM.hpp"

namespace {
    class CPU : public ::testing::Test {
    public:
        CPU() : instructionMemory(16 * 1024), dataMemory(16 * 1024) {}

        void SetUp() override {
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

    // clear instruction memory so it is a sequence of NOPs
    instructionMemory.clear();

    for (int i=0; i<kNumTicks; i++) {
        EXPECT_EQ((i*4), core.o_pc);
        testBench.tick();
    }
}
