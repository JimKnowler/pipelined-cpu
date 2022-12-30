#include <gtest/gtest.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

#include "cpu/DecoderTestBench.h"
using namespace decodertestbench;

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

