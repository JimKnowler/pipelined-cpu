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

reg [15:0] r_pc;

always @(posedge i_clk or negedge i_reset_n)
begin
    if (!i_reset_n)
        r_pc <= 0;
    else
        r_pc <= r_pc + 4;
end

assign o_pc = r_pc;

endmodule
