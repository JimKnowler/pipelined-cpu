load("@rules_verilator//verilator:defs.bzl", "verilator_cc_library")
load("@gtestverilog//gtestverilog:defs.bzl", "gtest_verilog_testbench")

verilator_cc_library(
    name = "CPU",
    srcs = [
        "source/CPU.v"
    ]
)

gtest_verilog_testbench(
    name = "CPUTestBench",
    deps = [":CPU"]
)

cc_test(
    name = "test",
    srcs = glob(
        include =[
            "**/*.cpp",
            "**/*.h",
            "**/*.hpp",
            "**/*.inl"
        ],
        exclude = [

        ]
    ) + [
        ":CPUTestBench"
    ],
    deps = [
        "@com_google_googletest//:gtest",
        "@gtestverilog//gtestverilog:lib",
        ":CPU",
    ],
)