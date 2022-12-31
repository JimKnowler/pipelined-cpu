module CPU(
    /* verilator lint_off UNUSED */
    /* verilator lint_off UNDRIVEN */
    input i_clk,
    input i_reset_n,
    

    output [15:0] o_instruction_address,
    input [31:0] i_instruction_data,

    output [15:0] o_data_address,
    output o_data_rw,
    output [31:0] o_data_data,
    input [31:0] i_data_data

    /* verilator lint_on UNUSED */
    /* verilator lint_on UNDRIVEN */
);


endmodule
