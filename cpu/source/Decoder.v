module Decoder(
    /* verilator lint_off UNUSED */
    input i_clk,
    input i_reset_n,
    /* verilator lint_on UNUSED */

    /* verilator lint_off UNUSED */
    input [31:0] i_ir,
    /* verilator lint_on UNUSED */

    output [7:0] o_opcode,
    output o_re1,                       // read enable register 1
    output [3:0] o_rs1,                 // read register selector 1
    output o_re2,                       // read enable register 2
    output [3:0] o_rs2,                 // read register selector 2
    output [3:0] o_ws,                  // write register selector
    output o_we,                        // write register enable
    output [15:0] o_i
);

localparam [7:0] //NOP = 0,
                 LDA = 1,
                 STA = 2,
                 ADD = 3,
                 SUB = 4;

reg [7:0] r_opcode;
reg r_re1;
reg [3:0] r_rs1;
reg r_re2;
reg [3:0] r_rs2;
reg [3:0] r_ws;
reg r_we;
reg [15:0] r_i;

always @(*)
begin
    r_re1 = 0;
    r_rs1 = 0;
    r_re2 = 0;
    r_rs2 = 0;
    r_ws = 0;
    r_we = 0;
    r_i = 0;

    r_opcode = i_ir[31:24];

    case (r_opcode)
        LDA: begin
            r_ws = i_ir[23:20];
            r_i = i_ir[15:0];
        end
        STA: begin
            r_re1 = 1;
            r_rs1 = i_ir[23:20];
            r_i = i_ir[15:0];
        end
        ADD, SUB: begin
            r_re1 = 1;
            r_rs1 = i_ir[7:4];
            r_re2 = 1;
            r_rs2 = i_ir[3:0];
            r_ws = i_ir[23:20];
        end
        default: begin
        end
    endcase

    case (r_opcode)
        ADD, SUB, LDA: r_we = 1;
        default: begin
        end
    endcase
end

assign o_opcode = r_opcode;
assign o_re1 = r_re1;
assign o_rs1 = r_rs1;
assign o_re2 = r_re2;
assign o_rs2 = r_rs2;
assign o_ws = r_ws;
assign o_we = r_we;
assign o_i = r_i;

endmodule
