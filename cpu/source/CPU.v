module CPU(
    input i_clk,
    input i_reset_n,

    // debugging - pipeline
    output o_debug_stall,

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
    output [7:0] o_debug_execute_opcode,

    // debugging - stage 3
    output [3:0] o_debug_alu_op,
    output [31:0] o_debug_alu_rd1,
    output [31:0] o_debug_alu_rd2,
    output [31:0] o_debug_alu_y,
    output [7:0] o_debug_memory_opcode,

    // debugging - stage 4
    output [7:0] o_debug_writeback_opcode,

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
                 LW = 1,
                 SW = 2,
                 ADD = 3,
                 SUB = 4;

// ----------------------------------------------------------
// Pipeline registers

wire w_stall;

reg [7:0] r_execute_opcode;
reg [3:0] r_execute_ws;
reg r_execute_we;
reg [31:0] r_execute_rd1;
reg [31:0] r_execute_rd2;
reg [31:0] r_execute_write_data;

reg [7:0] r_memory_opcode;
reg [3:0] r_memory_ws;
reg r_memory_we;
reg [31:0] r_memory_alu_y;
reg [31:0] r_memory_write_data;

reg [7:0] r_writeback_opcode;
reg [3:0] r_writeback_ws;
reg r_writeback_we;
reg [31:0] r_writeback_alu_y;
reg [31:0] r_writeback_data;

// ----------------------------------------------------------
// STAGE 1 - Fetch

reg [15:0] r_pc;            // Program counter
reg [31:0] r_ir;            // Instruction Register

always @(posedge i_clk or negedge i_reset_n)
begin
    if (!i_reset_n)
        r_pc <= 0;
    else if (!w_stall) begin
        r_pc <= r_pc + 4;
        r_ir <= i_instruction;
    end
end

assign o_pc = r_pc;

// ----------------------------------------------------------
// STAGE 2 - Decode

wire [7:0] w_decoder_opcode;
wire w_decoder_re1;
wire [3:0] w_decoder_rs1;
wire w_decoder_re2;
wire [3:0] w_decoder_rs2;
wire [3:0] w_decoder_ws;
wire w_decoder_we;
wire w_decoder_ie;
wire [15:0] w_decoder_id;

reg w_registerfile_we;             // write enable
reg [3:0] w_registerfile_ws;       // write register select
reg [31:0] w_registerfile_wd;      // write data

wire [31:0] w_registerfile_rd1;
wire [31:0] w_registerfile_rd2;

Decoder decoder(
    .i_clk(i_clk),
    .i_reset_n(i_reset_n),

    .i_ir(r_ir),
    .o_opcode(w_decoder_opcode),
    .o_re1(w_decoder_re1),
    .o_rs1(w_decoder_rs1),
    .o_re2(w_decoder_re2),
    .o_rs2(w_decoder_rs2),
    .o_ws(w_decoder_ws),
    .o_we(w_decoder_we),
    .o_ie(w_decoder_ie),
    .o_id(w_decoder_id)
);

RegisterFile registerFile(
    .i_clk(i_clk),
    .i_reset_n(i_reset_n),

    .i_we(w_registerfile_we),
    .i_ws(w_registerfile_ws),
    .i_wd(w_registerfile_wd),

    .i_rs1(w_decoder_rs1),
    .i_rs2(w_decoder_rs2),
    
    .o_rd1(w_registerfile_rd1),
    .o_rd2(w_registerfile_rd2)
);

StallControl stallControl(
    .i_clk(i_clk),
    .i_reset_n(i_reset_n),

    .i_decoder_re1(w_decoder_re1),
    .i_decoder_rs1(w_decoder_rs1),
    .i_decoder_re2(w_decoder_re2),
    .i_decoder_rs2(w_decoder_rs2),

    .i_execute_ws(r_execute_ws),
    .i_execute_we(r_execute_we),

    .i_memory_ws(r_memory_ws),
    .i_memory_we(r_memory_we),

    .i_writeback_ws(r_writeback_ws),
    .i_writeback_we(r_writeback_we),

    .o_stall(w_stall)
);

always @(posedge i_clk)
begin
    if (!w_stall) 
    begin
        r_execute_opcode <= w_decoder_opcode;
        r_execute_ws <= w_decoder_ws;
        r_execute_we <= w_decoder_we;
        r_execute_rd1 <= w_registerfile_rd1;
        if (w_decoder_ie)
            r_execute_rd2 <= { 16'b0, w_decoder_id };
        else
            r_execute_rd2 <= w_registerfile_rd2;
        r_execute_write_data <= w_registerfile_rd2;
    end
    else
    begin
        r_execute_opcode <= 0;
        r_execute_ws <= 0;
        r_execute_we <= 0;
        r_execute_rd1 <= 0;
        r_execute_rd2 <= 0;
        r_execute_write_data <= 0;
    end
end

// ----------------------------------------------------------
// STAGE 3 - Execute

reg [3:0] r_alu_op;
reg [31:0] r_alu_a;
reg [31:0] r_alu_b;
wire [31:0] w_alu_y;

ALU alu(
    .i_clk(i_clk),
    .i_reset_n(i_reset_n),
    .i_op(r_alu_op),
    .i_d1(r_alu_a),
    .i_d2(r_alu_b),
    .o_result(w_alu_y)
);

// TODO: define once and share between CPU.v and ALU.v
localparam [3:0] OP_PASSTHROUGH = 0;
localparam [3:0] OP_ADD = 1;
localparam [3:0] OP_SUB = 2;

always @(*)
begin
    case (r_execute_opcode)
        ADD, LW, SW: r_alu_op = OP_ADD;
        SUB: r_alu_op = OP_SUB;
        default: r_alu_op = OP_PASSTHROUGH;
    endcase

    r_alu_a = r_execute_rd1;
    r_alu_b = r_execute_rd2;
end

always @(posedge i_clk)
begin
    r_memory_opcode <= r_execute_opcode;
    r_memory_ws <= r_execute_ws;
    r_memory_we <= r_execute_we;
    r_memory_write_data <= r_execute_write_data;
    r_memory_alu_y <= w_alu_y;
end

// ----------------------------------------------------------
// STAGE 4 - Data Memory

reg r_rw;
reg [31:0] r_data;
reg [15:0] r_address;

always @(*)
begin
    case (r_memory_opcode)
        SW: begin
            r_rw = RW_WRITE;
            r_data = r_memory_write_data;
        end
        default: begin
            r_rw = RW_READ;
            r_data = 0;
        end
    endcase

    // TODO: expand memory bus (and simulation) to 32 bits?
    r_address = r_memory_alu_y[15:0];
end

always @(posedge i_clk)
begin
    r_writeback_opcode <= r_memory_opcode;
    r_writeback_ws <= r_memory_ws;
    r_writeback_we <= r_memory_we;
    r_writeback_alu_y <= r_memory_alu_y;
    r_writeback_data <= i_data;
end

// ----------------------------------------------------------
// STAGE 5 - Write back to Register File

always @(*)
begin
    w_registerfile_we = r_writeback_we;
    w_registerfile_ws = r_writeback_ws;

    case (r_writeback_opcode)
        ADD, SUB: w_registerfile_wd = r_writeback_alu_y;
        LW: w_registerfile_wd = r_writeback_data;
        default: w_registerfile_wd = 0;
    endcase
    
end

// ----------------------------------------------------------
// Module outputs

assign o_address = r_address;
assign o_data = r_data;
assign o_rw = r_rw;

// ----------------------------------------------------------
// Debug outputs

assign o_debug_stall = w_stall;
assign o_debug_ir = r_ir;

assign o_debug_decoder_rs1 = w_decoder_rs1;
assign o_debug_decoder_rs2 = w_decoder_rs2;
assign o_debug_decoder_rd = w_decoder_ws;
assign o_debug_writeback_we = w_registerfile_we;
assign o_debug_writeback_ws = w_registerfile_ws;
assign o_debug_writeback_wd = w_registerfile_wd;
assign o_debug_registerfile_rd1 = w_registerfile_rd1;
assign o_debug_registerfile_rd2 = w_registerfile_rd2;
assign o_debug_execute_opcode = r_execute_opcode;

assign o_debug_alu_op = r_alu_op;
assign o_debug_alu_rd1 = r_alu_a;
assign o_debug_alu_rd2 = r_alu_b;
assign o_debug_alu_y = w_alu_y;
assign o_debug_memory_opcode = r_memory_opcode;

assign o_debug_writeback_opcode = r_writeback_opcode;

endmodule
