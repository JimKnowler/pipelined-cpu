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
            //testBench.setClockPolarity(1);
            //testBench.core().i_clk_en = 1;
        }
        
        void TearDown() override {
        }

        void HelperReset() {
            testBench.reset();
        }

        RegistersTestBench testBench;
    };
}

TEST_F(Registers, ShouldConstruct) {

}

