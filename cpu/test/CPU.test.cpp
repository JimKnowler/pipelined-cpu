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

                // instruction read
                const size_t instructionAddress = core.o_instruction_address;
                core.i_instruction_data = instructionMemory.read32(instructionAddress);

                // data 
                const size_t dataAddress = core.o_data_address;
                if (core.o_data_rw == RW_WRITE) {
                    // write
                    dataMemory.write(dataAddress, core.o_data_data);
                } else {
                    // read
                    core.i_data_data = dataMemory.read32(dataAddress);
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

    EXPECT_EQ(0, core.o_instruction_address);
}

