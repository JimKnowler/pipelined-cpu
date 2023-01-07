# Pipelined CPU

An experiment in building a simple pipelined CPU in verilog.

Based on the pipeline CPU design that is described in week 1 of this [Computer Architecture course on Coursera](https://www.coursera.org/learn/comparch/home/week/1).
- The [Lecture Notes](https://d396qusza40orc.cloudfront.net/comparch%2Fcomparch-002%2Flecture_slides%2FSD2_XAek1vrBgk.pdf) contain diagrams for the pipelined CPU design

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
  - Fetch
  - Decode
  - Execute
  - Memory
  - Writeback (to register file)

- automated pipeline stall
  - automatically stall the pipeline when Decode stage needs to read from a register that the Execute, Memory or Writeback stages could write to

- Independent memory bus for instructions and data
  - benefit: removes structural hazard for memory access, to simplify this first attempt at pipelining
  - 16bit data addresses
  - 16bit instruction addresses

- 32 bit opcodes
  - LW = load word into register from address stored in regster, offset by immediate value
  - SW = store word from register to address stored in register, offset by immediate value
  - ADD = add two registers together and store result in another register
  - SUB = subtract two registers and store result in another register

- opcode format
  - all opcodes
    - [31 ... 24] = opcode *(8 bits)*
        - 00000000 = NOP
        - 00000001 = LW: Rd = mem[Rs1 + immediate]
        - 00000002 = SW: mem[Rs1 + immediate] = Rs2
        - 00000003 = ADD: Rd = Rs1 + Rs2
        - 00000004 = SUB: Rd = Rs1 - Rs2
  - Type LOAD: LW
    - [23 ... 20] = destination register *(4 bits)*
    - [19 ... 4] = immediate value *(16 bits)*
    - [3 ... 0] = source register 1 *(4 bits)*
  - Type STORE: SW
    - [23 ... 8] = immediate value *(16 bits)*
    - [7 ... 4] = source register 1 *(4 bits)*
    - [3 ... 0] = source register 2 *(4 bits)*
  - Type ALU: ADD, SUB
    - [23 ... 20] = destination register *(4 bits)*
    - [19 ... 8] = 0000 0000 0000 *(12 bits)*
    - [7 ... 4] = source register 1 *(4 bits)*
    - [3 ... 0] = source register 2 *(4 bits)*

# TODO

- add forwarding/bypassing from Execute, Memory and Writeback stages to Decoder, to avoid stalling 
- expand memory bus to 32 bits
- add branch
- add conditional branch
- add control hazards for branch (branch delay slot?)
- treat r0 as const 0?
