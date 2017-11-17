module inputReader(
valid,
arduinoInput,
robotX,
robotY,
preX,
preY),
walls,
treasure;

input valid;
input [7:0] arduinoInput;
output reg [1:0] robotX;
output reg [2:0] robotY;
output reg [3:0] walls;
output reg [1:0] treasure;

output reg [1:0] preX;
output reg [2:0] preY;

always @ (posedge valid) begin
	preX = robotX;
	preY = robotY;
	if (arduinoInput[8] == 1) begin
		robotX = arduinoInput[6:5];
		robotY = arduinoInput[4:2];
	end
	if arduinoInput[8] == 0) begin
		walls = arduinoInput[6:3];
		treasure = arduinoInput[2:1];
	end
end

endmodule