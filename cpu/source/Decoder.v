module Decoder(
    /* verilator lint_off UNUSED */
    input i_clk,
    input i_reset_n,
    /* verilator lint_on UNUSED */

    /* verilator lint_off UNUSED */
    input [31:0] i_ir,
    /* verilator lint_on UNUSED */

    output [7:0] o_opcode,
    output [3:0] o_rs1,
    output [3:0] o_rs2,
    output [3:0] o_rd,
    output [15:0] o_i
);

localparam [7:0] //NOP = 0,
                 LDA = 1,
                 STA = 2,
                 ADD = 3,
                 SUB = 4;

reg [7:0] r_opcode;
reg [3:0] r_rs1;
reg [3:0] r_rs2;
reg [3:0] r_rd;
reg [15:0] r_i;

always @(*)
begin
    r_rs1 = 0;
    r_rs2 = 0;
    r_rd = 0;
    r_i = 0;

    r_opcode = i_ir[31:24];

    case (r_opcode)
        LDA: begin
            r_rd = i_ir[23:20];
            r_i = i_ir[15:0];
        end
        STA: begin
            r_rs1 = i_ir[23:20];
            r_i = i_ir[15:0];
        end
        ADD, SUB: begin
            r_rs1 = i_ir[7:4];
            r_rs2 = i_ir[3:0];
            r_rd = i_ir[23:20];
        end
        default: begin
        end
    endcase
end

assign o_opcode = r_opcode;
assign o_rs1 = r_rs1;
assign o_rs2 = r_rs2;
assign o_rd = r_rd;
assign o_i = r_i;

endmodule
