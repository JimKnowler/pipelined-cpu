module ALU(
    /* verilator lint_off UNUSED */
    input i_clk,
    input i_reset_n,
    /* verilator lint_on UNUSED */

    input [3:0] i_op,
    input [31:0] i_d1,
    input [31:0] i_d2,

    output [31:0] o_result
);

reg [31:0] r_result;

localparam [3:0] OP_PASSTHROUGH = 0;
localparam [3:0] OP_ADD = 1;
localparam [3:0] OP_SUB = 2;

always @(*)
begin
    case (i_op)
        OP_PASSTHROUGH: r_result = i_d1;
        OP_ADD: r_result = i_d1 + i_d2;
        OP_SUB: r_result = i_d1 - i_d2;
        default: r_result = 0;
    endcase
end

assign o_result = r_result;

endmodule
