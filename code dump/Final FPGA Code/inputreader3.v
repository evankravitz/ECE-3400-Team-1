module inputReader(
valid,
arduinoInput,
robotX,
robotY,
preX,
preY,
walls,
treasure, 
left, right, down, up);

input valid;
input [7:0] arduinoInput;
output reg [1:0] robotX;
output reg [2:0] robotY;
output reg [3:0] walls;
output reg [1:0] treasure;
output reg left; 
output reg right; 
output reg up; 
output reg down; 

output reg [1:0] preX;
output reg [2:0] preY;

always @ (posedge valid) begin
	preX = robotX;
	preY = robotY;

	if (arduinoInput[8] == 1'b1) begin
		robotX = arduinoInput[6:5];
		robotY = arduinoInput[4:2];
	end
	if (arduinoInput[8] == 1'b0) begin
		walls = arduinoInput[6:3];
		treasure = arduinoInput[2:1];
	end
	
	if (preX == robotX) begin 
		if (robotY > preY) begin 
			right = 1'b0; 
			left = 1'b0;
			up = 1'b1;
			down = 1'b0; 
		end else begin 
			right = 1'b0; 
			left = 1'b0; 
			up = 1'b0;
			down = 1'b1;
		end
	end 

	if (preY == robotY) begin 
		if (robotX > preX) begin 
			right = 1'b1; 
			left = 1'b0;
			up = 1'b0;
			down = 1'b0; 
		end else begin 
			right = 1'b0; 
			left = 1'b1; 
			up = 1'b0;
			down = 1'b0;
		end
	end 

end

endmodule
