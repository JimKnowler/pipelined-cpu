module DecoderReadRegisterMux(
    input [3:0] i_rs,
    input [31:0] i_rd,
    
    input [7:0] i_memory_opcode,
    input [3:0] i_memory_ws,
    input [31:0] i_memory_data,

    input [7:0] i_execute_opcode,
    input [3:0] i_execute_ws,
    input [31:0] i_execute_data,

    output [31:0] o_rd
);

// TODO: define in one file and import into Decoder.v and CPU.v
localparam [7:0] //NOP = 0,
                 LW = 1,
                // SW = 2,
                ADD = 3,
                SUB = 4;

reg [31:0] r_rd;

always @(*)
begin
    r_rd = i_rd;

    if ((i_memory_opcode == LW) && (i_rs == i_memory_ws))
        r_rd = i_memory_data;
    else begin
        case (i_execute_opcode)
        ADD, SUB: begin
        if (i_rs == i_execute_ws)
            r_rd = i_execute_data;
        end
        default: begin
        end
        endcase
    end
end

assign o_rd = r_rd;

endmodule
