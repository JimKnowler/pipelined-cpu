#include <gtest/gtest.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

#include "cpu/StallControlTestBench.h"
using namespace stallcontroltestbench;

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

TEST_F(StallControl, ShouldNotStallWhenLaterStagesHaveWriteDisabled) {
    auto& core = testBench.core();

    // note: all read are write register selectors are set by default
    //       to register 0
    // note: all write enables are set by default to 0

    core.i_decoder_re1 = 1;
    core.i_decoder_re2 = 1;

    core.eval();

    EXPECT_EQ(0, core.o_stall);
}

TEST_F(StallControl, ShouldNotStallWhenLaterStagesHaveWriteEnabledButDecoderHasReadDisabled) {
    auto& core = testBench.core();

    // note: all read are write register selectors are set by default
    //       to register 0

    core.i_decoder_re1 = 0;
    core.i_decoder_re2 = 0;

    core.eval();

    EXPECT_EQ(0, core.o_stall);
}

TEST_F(StallControl, ShouldStallWhenRs1WrittenByExecuteStage) {
    auto& core = testBench.core();

    const uint8_t kTestRs1 = 12;
    const uint8_t kTestRs2 = 13;

    core.i_decoder_re1 = 1;
    core.i_decoder_rs1 = kTestRs1;
    core.i_decoder_re2 = 1;
    core.i_decoder_rs2 = kTestRs2;

    core.i_execute_ws = kTestRs1;
    core.i_execute_we = 1;

    core.eval();

    EXPECT_EQ(1, core.o_stall);
}

TEST_F(StallControl, ShouldStallWhenRs2WrittenByExecuteStage) {
    auto& core = testBench.core();

    const uint8_t kTestRs1 = 12;
    const uint8_t kTestRs2 = 13;

    core.i_decoder_re1 = 1;
    core.i_decoder_rs1 = kTestRs1;
    core.i_decoder_re2 = 1;
    core.i_decoder_rs2 = kTestRs2;

    core.i_execute_ws = kTestRs2;
    core.i_execute_we = 1;

    core.eval();

    EXPECT_EQ(1, core.o_stall);
}
