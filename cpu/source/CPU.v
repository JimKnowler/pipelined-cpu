module CPU(
    input i_clk,
    input i_reset_n,

    output o_data
);

reg r_data;

always @(posedge i_clk or negedge i_reset_n) begin
    if (!i_reset_n)
        r_data <= 0;
    else
        r_data <= 1;
end

assign o_data = r_data;

endmodule
