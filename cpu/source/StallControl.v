module StallControl(
    /* verilator lint_off UNUSED */
    input i_clk,
    input i_reset_n,
    /* verilator lint_on UNUSED */

    // decode stage
    input i_decoder_re1,            // read enabled for read register 1
    input [3:0] i_decoder_rs1,      // read register selector 1
    input i_decoder_re2,            // read enable for read register 2
    input [3:0] i_decoder_rs2,      // read register selector 2

    // execute stage
    input [3:0] i_execute_ws,       // write register selector
    input i_execute_we,             // write enable

    output o_stall

);

reg r_stall;

always @(*)
begin
    r_stall = (
                (
                    ((i_decoder_rs1 == i_execute_ws) && i_execute_we) 
                ) && i_decoder_re1
              ) || 
              (
                (
                    ((i_decoder_rs2 == i_execute_ws) && i_execute_we)
                ) && i_decoder_re2
              );
end

assign o_stall = r_stall;


endmodule
