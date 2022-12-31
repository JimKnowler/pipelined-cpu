module CPU(
    /* verilator lint_off UNUSED */
    /* verilator lint_off UNDRIVEN */
    input i_clk,
    input i_reset_n,
    
    // instruction bus
    output [15:0] o_pc,
    input [31:0] i_instruction,

    // data bus
    output [15:0] o_address,
    output o_rw,
    output [31:0] o_data,
    input [31:0] i_data

    /* verilator lint_on UNUSED */
    /* verilator lint_on UNDRIVEN */
);

localparam RW_READ = 0;
localparam RW_WRITE = 1;

// TODO: define in one file and import into Decoder.v and CPU.v
localparam [7:0] //NOP = 0,
                 LDA = 1,
                 STA = 2,
                 ADD = 3,
                 SUB = 4;

// ----------------------------------------------------------
// Components

reg [15:0] r_pc;            // Program counter

reg [31:0] r_ir;            // Instruction Register
wire [7:0] w_ir_opcode;     // Opcode
wire [15:0] w_ir_immediate;         // 'immediate' value
wire [3:0] w_ir_rd;         // destination register

reg r_we;                  // register file write enable
reg [3:0] r_ws;            // register file write register selector
reg [31:0] r_wd;           // register file write data
wire [3:0] w_rs1;           // register file read register selector 1
wire [3:0] w_rs2;           // register file read register selector 2
wire [31:0] w_rd1;          // register file read register data 1
wire [31:0] w_rd2;          // register file read register data 1

Decoder decoder(
    .i_clk(i_clk),
    .i_reset_n(i_reset_n),

    .i_ir(r_ir),
    .o_opcode(w_ir_opcode),
    .o_rs1(w_rs1),
    .o_rs2(w_rs2),
    .o_rd(w_ir_rd),
    .o_i(w_ir_immediate)
);

RegisterFile registerFile(
    .i_clk(i_clk),
    .i_reset_n(i_reset_n),

    .i_we(r_we),
    .i_ws(r_ws),
    .i_wd(r_wd),

    .i_rs1(w_rs1),
    .i_rs2(w_rs2),
    
    .o_rd1(w_rd1),
    .o_rd2(w_rd2)
);

// ----------------------------------------------------------
// STAGE 1 - Fetch

always @(posedge i_clk or negedge i_reset_n)
begin
    if (!i_reset_n)
        r_pc <= 0;
    else begin
        r_pc <= r_pc + 4;
        r_ir <= i_instruction;
    end
end

assign o_pc = r_pc;

// ----------------------------------------------------------
// STAGE 2 - Decode

reg [7:0] r_stage2_opcode;
reg [3:0] r_stage2_rd;      // dest register
reg [31:0] r_stage2_rd1;    // read register 1 data
reg [31:0] r_stage2_rd2;    // read register 2 data
reg [15:0] r_stage2_immediate;

always @(posedge i_clk)
begin
    r_stage2_opcode <= w_ir_opcode;
    r_stage2_rd <= w_ir_rd;
    r_stage2_rd1 <= w_rd1;
    r_stage2_rd2 <= w_rd2;
    r_stage2_immediate <= w_ir_immediate;
end

// ----------------------------------------------------------
// STAGE 3 - Execute

reg [3:0] r_alu_op;
reg [31:0] r_alu_d1;
reg [31:0] r_alu_d2;
wire [31:0] w_alu_result;

ALU alu(
    .i_clk(i_clk),
    .i_reset_n(i_reset_n),
    .i_op(r_alu_op),
    .i_d1(r_alu_d1),
    .i_d2(r_alu_d2),
    .o_result(w_alu_result)
);

localparam [3:0] OP_PASSTHROUGH = 0;
localparam [3:0] OP_ADD = 1;
localparam [3:0] OP_SUB = 2;

always @(*)
begin
    case (r_stage2_opcode)
        ADD: r_alu_op = OP_ADD;
        SUB: r_alu_op = OP_SUB;
        default: r_alu_op = OP_PASSTHROUGH;
    endcase

    r_alu_d1 = r_stage2_rd1;
    r_alu_d2 = r_stage2_rd2;
end

reg [7:0] r_stage3_opcode;
reg [3:0] r_stage3_rd;
reg [31:0] r_stage3_alu_result;
reg [15:0] r_stage3_immediate;

always @(posedge i_clk)
begin
    r_stage3_opcode <= r_stage2_opcode;
    r_stage3_rd <= r_stage2_rd;
    r_stage3_alu_result <= w_alu_result;
    r_stage3_immediate <= r_stage2_immediate;
end

// ----------------------------------------------------------
// STAGE 4 - Data Memory

reg [7:0] r_stage4_opcode;
reg [3:0] r_stage4_rd;
reg [31:0] r_stage4_data;
reg [31:0] r_stage4_alu_result;

reg r_rw;
reg [31:0] r_data;
reg [15:0] r_address;

always @(*)
begin
    case (r_stage3_opcode)
        STA: begin
            r_rw = RW_WRITE;
            r_data = r_stage3_alu_result;
        end
        default: begin
            r_rw = RW_READ;
            r_data = 0;
        end
    endcase

    r_address = r_stage3_immediate;
end

always @(posedge i_clk)
begin
    r_stage4_opcode <= r_stage3_opcode;
    r_stage4_rd <= r_stage3_rd;
    r_stage4_data <= i_data;
    r_stage4_alu_result <= r_stage3_alu_result;
end

// ----------------------------------------------------------
// STAGE 5 - Write back to Register File

always @(*)
begin
    case (r_stage4_opcode)
        ADD, SUB, LDA: r_we = 1;
        default: r_we = 0;
    endcase

    r_ws = r_stage4_rd;

    case (r_stage4_opcode)
        ADD, SUB: r_wd = r_stage4_alu_result;
        LDA: r_wd = r_stage4_data;
        default: r_wd = 0;
    endcase
    
end

// ----------------------------------------------------------
// Module outputs
assign o_address = r_address;
assign o_data = r_data;
assign o_rw = r_rw;

endmodule
