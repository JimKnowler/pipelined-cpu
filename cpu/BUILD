load("@rules_verilator//verilator:defs.bzl", "verilator_cc_library")
load("@gtestverilog//gtestverilog:defs.bzl", "gtest_verilog_testbench")

# Integration test of CPU

verilator_cc_library(
    name = "CPU",
    srcs = [
        "source/ALU.v",
        "source/CPU.v",
        "source/Decoder.v",
        "source/RegisterFile.v"
    ]
)

gtest_verilog_testbench(
    name = "CPUTestBench",
    deps = [":CPU"]
)

# Test individual components of CPU

verilator_cc_library(
    name = "Decoder",
    srcs = [
        "source/Decoder.v"
    ]
)

gtest_verilog_testbench(
    name = "DecoderTestBench",
    deps = [":Decoder"]
)

verilator_cc_library(
    name = "RegisterFile",
    srcs = [
        "source/RegisterFile.v"
    ]
)

gtest_verilog_testbench(
    name = "RegisterFileTestBench",
    deps = [":RegisterFile"]
)

verilator_cc_library(
    name = "ALU",
    srcs = [
        "source/ALU.v"
    ]
)

gtest_verilog_testbench(
    name = "ALUTestBench",
    deps = [":ALU"]
)

# CPP Test executable

cc_test(
    name = "test",
    srcs = glob(
        include = [
            "**/*.cpp",
            "**/*.h",
            "**/*.hpp",
            "**/*.inl"
        ],
        exclude = [
        ]
    ) + [
        ":CPUTestBench",
        ":DecoderTestBench",
        ":RegisterFileTestBench",
        ":ALUTestBench"
    ],
    deps = [
        "@com_google_googletest//:gtest",
        "@gtestverilog//gtestverilog:lib",
        ":CPU",
        ":Decoder",
        ":RegisterFile",
        ":ALU"
    ],
)