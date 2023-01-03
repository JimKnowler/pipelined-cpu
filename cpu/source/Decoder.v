module Decoder(
    /* verilator lint_off UNUSED */
    input i_clk,
    input i_reset_n,
    /* verilator lint_on UNUSED */

    input [31:0] i_ir,                  // instruction register

    output [7:0] o_opcode,              // opcode
    
    output o_re1,                       // read enable register 1
    output [3:0] o_rs1,                 // read register selector 1
    
    output o_re2,                       // read enable register 2
    output [3:0] o_rs2,                 // read register selector 2

    output [3:0] o_ws,                  // write register selector
    output o_we,                        // write register enable

    output o_ie,                        // immediate enable
    output [15:0] o_id                  // immediate data
);

localparam [7:0] //NOP = 0,
                 LW = 1,
                 SW = 2,
                 ADD = 3,
                 SUB = 4;

reg [7:0] r_opcode;
reg r_re1;
reg [3:0] r_rs1;
reg r_re2;
reg [3:0] r_rs2;
reg [3:0] r_ws;
reg r_we;
reg r_ie;
reg [15:0] r_id;

always @(*)
begin
    r_re1 = 0;
    r_rs1 = 0;
    r_re2 = 0;
    r_rs2 = 0;
    r_ws = 0;
    r_we = 0;
    r_id = 0;
    r_ie = 0;

    r_opcode = i_ir[31:24];

    case (r_opcode)
        LW: begin
            r_ws = i_ir[23:20];
            r_ie = 1;
            r_id = i_ir[19:4];
            r_re1 = 1;
            r_rs1 = i_ir[3:0];
        end
        SW: begin
            r_ie = 1;
            r_id = i_ir[23:8];
            r_re1 = 1;
            r_rs1 = i_ir[7:4];
            r_re2 = 1;
            r_rs2 = i_ir[3:0];
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
        ADD, SUB, LW: r_we = 1;
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
assign o_ie = r_ie;
assign o_id = r_id;

endmodule
