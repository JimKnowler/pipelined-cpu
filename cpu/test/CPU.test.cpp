#include <gtest/gtest.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

#include "cpu/CPUTestBench.h"
using namespace cputestbench;

namespace {
    class CPU : public ::testing::Test {
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

        CPUTestBench testBench;
    };
}

TEST_F(CPU, ShouldConstruct) {

}

TEST_F(CPU, ShouldReset) {
    HelperReset();

    auto& core = testBench.core();

    EXPECT_EQ(0, core.o_data);
}

