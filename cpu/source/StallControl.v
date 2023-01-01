module StallControl(
    /* verilator lint_off UNUSED */
    input i_clk,
    input i_reset_n,
    /* verilator lint_on UNUSED */

/* verilator lint_off UNUSED */

    // decode stage
    input [3:0] i_decoder_rs1,      // read register selector 1
    input [3:0] i_decoder_rs2,      // read register selector 2

    // execute stage
    input [3:0] i_execute_ws,       // write register selector
    input i_execute_we,             // write enable

    // memory stage
    input [3:0] i_memory_ws,        // write register selector
    input i_memory_we,              // write enable

    // writeback stage
    input [3:0] i_writeback_ws,     // write register selector
    input i_writeback_we,           // write enable


/* verilator lint_on UNUSED */

    output o_stall

);

reg r_stall;

always @(*)
begin
    r_stall = (((i_decoder_rs1 == i_execute_ws) || (i_decoder_rs2 == i_execute_ws)) && i_execute_we)
              || (((i_decoder_rs1 == i_memory_ws) || (i_decoder_rs2 == i_memory_ws)) && i_memory_we)
              || (((i_decoder_rs1 == i_writeback_ws) || (i_decoder_rs2 == i_writeback_ws)) && i_writeback_we);
end

assign o_stall = r_stall;


endmodule
