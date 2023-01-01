module CPU(
    input i_clk,
    input i_reset_n,

    // debugging - stage 1
    output [31:0] o_debug_ir,

    // debugging - stage 2
    output [3:0] o_debug_decoder_rs1,
    output [3:0] o_debug_decoder_rs2,
    output [3:0] o_debug_decoder_rd,
    output o_debug_writeback_we,
    output [3:0] o_debug_writeback_ws,
    output [31:0] o_debug_writeback_wd,
    output [31:0] o_debug_registerfile_rd1,
    output [31:0] o_debug_registerfile_rd2,

    // debugging - stage 3
    output [3:0] o_debug_alu_op,
    output [31:0] o_debug_alu_result,

    // instruction bus
    output [15:0] o_pc,
    input [31:0] i_instruction,

    // data bus
    output [15:0] o_address,
    output o_rw,
    output [31:0] o_data,
    input [31:0] i_data
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
// STAGE 1 - Fetch

reg [15:0] r_pc;            // Program counter
reg [31:0] r_ir;            // Instruction Register

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

wire [7:0] w_decoder_opcode;
wire [3:0] w_decoder_rs1;
wire [3:0] w_decoder_rs2;
wire [3:0] w_decoder_rd;
wire [15:0] w_decoder_immediate;

reg w_writeback_we;             // write enable
reg [3:0] w_writeback_ws;       // write register select
reg [31:0] w_writeback_wd;      // write data

wire [31:0] w_registerfile_rd1;
wire [31:0] w_registerfile_rd2;

Decoder decoder(
    .i_clk(i_clk),
    .i_reset_n(i_reset_n),

    .i_ir(r_ir),
    .o_opcode(w_decoder_opcode),
    .o_rs1(w_decoder_rs1),
    .o_rs2(w_decoder_rs2),
    .o_rd(w_decoder_rd),
    .o_i(w_decoder_immediate)
);

RegisterFile registerFile(
    .i_clk(i_clk),
    .i_reset_n(i_reset_n),

    .i_we(w_writeback_we),
    .i_ws(w_writeback_ws),
    .i_wd(w_writeback_wd),

    .i_rs1(w_decoder_rs1),
    .i_rs2(w_decoder_rs2),
    
    .o_rd1(w_registerfile_rd1),
    .o_rd2(w_registerfile_rd2)
);

reg [7:0] r_stage2_opcode;
reg [3:0] r_stage2_rd;
reg [31:0] r_stage2_rd1;
reg [31:0] r_stage2_rd2;
reg [15:0] r_stage2_immediate;

always @(posedge i_clk)
begin
    r_stage2_opcode <= w_decoder_opcode;
    r_stage2_rd <= w_decoder_rd;
    r_stage2_rd1 <= w_registerfile_rd1;
    r_stage2_rd2 <= w_registerfile_rd2;
    r_stage2_immediate <= w_decoder_immediate;
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

// TODO: define once and share between CPU.v and ALU.v
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
reg [15:0] r_stage3_immediate;
reg [31:0] r_stage3_alu_result;

always @(posedge i_clk)
begin
    r_stage3_opcode <= r_stage2_opcode;
    r_stage3_rd <= r_stage2_rd;
    r_stage3_immediate <= r_stage2_immediate;
    r_stage3_alu_result <= w_alu_result;
end

// ----------------------------------------------------------
// STAGE 4 - Data Memory

reg [7:0] r_stage4_opcode;
reg [3:0] r_stage4_rd;
reg [31:0] r_stage4_alu_result;
reg [31:0] r_stage4_data;

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
    r_stage4_alu_result <= r_stage3_alu_result;
    r_stage4_data <= i_data;
end

// ----------------------------------------------------------
// STAGE 5 - Write back to Register File

always @(*)
begin
    case (r_stage4_opcode)
        ADD, SUB, LDA: w_writeback_we = 1;
        default: w_writeback_we = 0;
    endcase

    w_writeback_ws = r_stage4_rd;

    case (r_stage4_opcode)
        ADD, SUB: w_writeback_wd = r_stage4_alu_result;
        LDA: w_writeback_wd = r_stage4_data;
        default: w_writeback_wd = 0;
    endcase
    
end

// ----------------------------------------------------------
// Module outputs

assign o_address = r_address;
assign o_data = r_data;
assign o_rw = r_rw;

// ----------------------------------------------------------
// Debug outputs

assign o_debug_ir = r_ir;

assign o_debug_decoder_rs1 = w_decoder_rs1;
assign o_debug_decoder_rs2 = w_decoder_rs2;
assign o_debug_decoder_rd = w_decoder_rd;
assign o_debug_writeback_we = w_writeback_we;
assign o_debug_writeback_ws = w_writeback_ws;
assign o_debug_writeback_wd = w_writeback_wd;
assign o_debug_registerfile_rd1 = w_registerfile_rd1;
assign o_debug_registerfile_rd2 = w_registerfile_rd2;

assign o_debug_alu_op = r_alu_op;
assign o_debug_alu_result = w_alu_result;

endmodule
