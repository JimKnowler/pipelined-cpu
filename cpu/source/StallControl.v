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
    input [7:0] i_execute_opcode,

    output o_stall

);

// TODO: define in one file and import into Decoder.v and CPU.v
localparam [7:0] //NOP = 0,
                 LW = 1;
                 //SW = 2,
                 //ADD = 3,
                 //SUB = 4;


reg r_stall;

always @(*)
begin
    r_stall = (
                (i_decoder_rs1 == i_execute_ws)
                && (i_execute_opcode == LW)
                && i_decoder_re1
              ) || 
              (
                (i_decoder_rs2 == i_execute_ws)
                && (i_execute_opcode == LW)
                && i_decoder_re2
              );
end

assign o_stall = r_stall;


endmodule
