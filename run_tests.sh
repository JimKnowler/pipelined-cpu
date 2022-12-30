#!/bin/bash
bazel build //cpu:test --incompatible_require_linker_input_cc_api=false && ./bazel-bin/cpu/test