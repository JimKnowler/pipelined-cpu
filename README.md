# Pipelined CPU

An experiment in building a simple pipelined CPU in verilog

# Dependencies

- Bazel v5.0.0

   ```bash
   brew install bazel
   ```

- MacOS

# Build and run Tests

``` bash
./run_tests.sh
```

# ISA

- 16 general purpose registers

- 5 stage pipeline
  - fetch
  - decode
  - execute
  - memory
  - write to register file

- Separate memory bus for instructions and data
  - benefit: removes structural hazard for memory access
  - 16bit data addresses
  - 16bit instruction addresses

- 32 bit opcodes
  - LDA = load register from address
  - STA = store register to address
  - ADD = add two registers together and store result in another register
  - SUB = subtract two registers and store result in another register

- opcode format
  - all opcodes
    - [31 ... 24] = opcode *(8 bits)*
        - 00000000 = NOP
        - 00000001 = LDA
        - 00000002 = STA
        - 00000003 = ADD: Rd = Rs1 + Rs2
        - 00000004 = SUB: Rd = Rs1 - Rs2
  - Type I: LDA, STA
    - [23 ... 20] = register *(4 bits)*
    - [19 ... 16] = 0000 *(4 bits)*
    - [15 ... 0] = address *(8 bits)*
  - Type R: ADD, SUB
    - [23 ... 20] = destination register *(4 bits)*
    - [19 ... 8] = 0000 0000 0000 *(12 bits)*
    - [7 ... 4] = source register 1 *(4 bits)*
    - [3 ... 0] = source register 2 *(4 bits)*
