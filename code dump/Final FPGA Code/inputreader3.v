module inputReader(
valid,
arduinoInput,
robotX,
robotY,
preX,
preY,
walls,
treasure, 
left, right, down, up, updateType);

input valid;
input [6:0] arduinoInput;
output reg [1:0] robotX;
output reg [2:0] robotY;
output reg [3:0] walls;
output reg [1:0] treasure;
output reg updateType;
output reg left; 
output reg right; 
output reg up; 
output reg down; 

output reg [1:0] preX;
output reg [2:0] preY;

always @ (posedge valid) begin
	preX = robotX;
	preY = robotY;

	if (arduinoInput[6] == 1'b1) begin
		updateType = 1'b0;
		robotX = arduinoInput[5:4];
		robotY = arduinoInput[3:1];
	end
	if (arduinoInput[6] == 1'b0) begin
		updateType = 1'b1;
		walls = arduinoInput[5:2];
		treasure = arduinoInput[1:0];
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
