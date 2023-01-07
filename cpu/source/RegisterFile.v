module RegisterFile(
    input i_clk,
    input i_reset_n,

    input i_we,                     // write enable
    input [3:0] i_ws,               // write register selector
    input [31:0] i_wd,              // write data

    input [3:0] i_rs1,              // read register selector (1) 
    input [3:0] i_rs2,              // read register selector (2)

    output [31:0] o_rd1,            // read data (1)
    output [31:0] o_rd2             // read data (2)
);

reg [31:0] r_registers [15:0];

reg [31:0] r_rd1;
reg [31:0] r_rd2;

always @(*)
begin
    r_rd1 = r_registers[i_rs1];
    r_rd2 = r_registers[i_rs2]; 

    // bypass from write to read
    if (i_we)
    begin
        if (i_ws == i_rs1)
            r_rd1 = i_wd;

        if (i_ws == i_rs2)
            r_rd2 = i_wd;
    end
end

integer i;

always @(posedge i_clk or negedge i_reset_n)
begin
    if (!i_reset_n)
    begin
        begin
            for (i=0; i<16; i=i+1)
                r_registers[i] <= 0;
        end
    end
    else if (i_we)
    begin
        r_registers[i_ws] <= i_wd;
    end
end

assign o_rd1 = r_rd1;
assign o_rd2 = r_rd2;

endmodule
