//=======================================================
// ECE3400 Fall 2017
// Lab 3: Template top-level module
//
// Top-level skeleton from Terasic
// Modified by Claire Chen for ECE3400 Fall 2017
//=======================================================

`define ONE_SEC 25000000


module DE0_NANO(

	//////////// CLOCK //////////
	CLOCK_50,

	//////////// LED //////////
	LED,

	//////////// KEY //////////
	KEY,

	//////////// SW //////////
	SW,

	//////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	GPIO_0_D,
	GPIO_0_IN,

	//////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
	GPIO_1_D,
	GPIO_1_IN,
);

	 //=======================================================
	 //  PARAMETER declarations
	 //=======================================================

	 localparam ONE_SEC = 25000000; // one second in 25MHz clock cycles
	 localparam white = 8'b11111111; //unexplored
	 localparam black = 8'b0; //walls
	 localparam pink = 8'b11110011; //explored
	 localparam magenta = 8'b11100011; //botbotbot
	 localparam red = 8'b11100000;  // 7kHz treasure Freq (2'b01)
	 localparam blue = 8'b00110011; //12kHz treasure Freq (2'b10)
	 localparam green = 8'b01010001; // 17 kHZ treasure Freq (2'b11)
	 localparam CLKDIVIDER_B_SIN = 25000000 / 800 / 256;
	 
	 //=======================================================
	 //  PORT declarations
	 //=======================================================

	 //////////// CLOCK //////////
	 input 		          		CLOCK_50;

	 //////////// LED //////////
	 output		     [7:0]		LED;

	 /////////// KEY //////////
	 input 		     [1:0]		KEY; 

	 //////////// SW //////////
	 input 		     [3:0]		SW;

	 //////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	 inout 		    [33:0]		GPIO_0_D;
	 input 		     [1:0]		GPIO_0_IN;

	 //////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
	 inout 		    [33:0]		GPIO_1_D;
	 input 		     [1:0]		GPIO_1_IN;

    //=======================================================
    //  REG/WIRE declarations
    //=======================================================
    reg         CLOCK_25;
    wire        reset; // active high reset signal 

    wire [9:0]  PIXEL_COORD_X; // current x-coord from VGA driver
    wire [9:0]  PIXEL_COORD_Y; // current y-coord from VGA driver
    reg [7:0]  PIXEL_COLOR;   // input 8-bit pixel color for current coords
	 wire [2:0] GRID_X;
	 wire [2:0] GRID_Y;
	 reg grid [19:0][7:0];
	 reg visited [19:0]; 
	reg [15:0] soundcounter;
	 
	 
	 GRID_SELECTOR gridSelector(
		.CLOCK_50(CLOCK_50),
		.PIXEL_COORD_X(PIXEL_COORD_X),
		.PIXEL_COORD_Y(PIXEL_COORD_Y),
		.GRID_X(GRID_X),
		.GRID_Y(GRID_Y)
	);
	

	reg[7:0] grid1[3:0] [4:0];
	reg[7:0] currentGrid;
	reg[24:0] counter;
		 reg [24:0] led_counter; // timer to keep track of when to toggle LED
	 reg [24:0] freq_counter;
	 reg 			led_state;   // 1 is on, 0 is off
	 
	 reg [2:0]	freq_state;
	 
	 reg [7:0] DAC; //DAC
	 
	//state machine 
	always @(posedge CLOCK_25) begin
		if (GRID_X > 3) begin
			PIXEL_COLOR <= black;
		end
		else begin
		currentGrid <= grid1[GRID_X][GRID_Y];
 
			if (PIXEL_COORD_X<=(10'd4+(GRID_X*10'd96)) && PIXEL_COORD_X>=(GRID_X*10'd96) && currentGrid[6] == 1'b1) begin  //right
				PIXEL_COLOR <= black; 
			end else 
			if ((PIXEL_COORD_X >=((GRID_X+1)*10'd96)-10'd4) && PIXEL_COORD_X <= ((GRID_X+1)*10'd96) && currentGrid[4] == 1'b1) begin //left
				PIXEL_COLOR <= black; 
			end else if (currentGrid[1:0] == 2'b01) begin 
				PIXEL_COLOR <= pink;
			end else if (currentGrid[1:0] == currPos && (PIXEL_COORD_X<=((GRID_X+1)*10'd96)-10'd30) && PIXEL_COORD_X >= ((GRID_X*10'd96)+10'd30) && (PIXEL_COORD_Y<=((GRID_Y+1)*10'd96)-10'd30) && PIXEL_COORD_Y >= ((GRID_Y*10'd96)+10'd30))  begin
				PIXEL_COLOR <= magenta;
//			end else if (currentGrid[1:0] == currPos) begin 
//				if (right == 1'b1 && (PIXEL_COORD_X<=(GRID_X*10'd96) + 10'd48) && (PIXEL_COORD_X>=(GRID_X*10'd96))) begin 
//					PIXEL_COLOR <= pink; 
//				end 
//				else if (left == 1'b1 && (PIXEL_COORD_X>=((GRID_X+1)*10'd96) - 10'd48) && (PIXEL_COORD_X<=(GRID_X+1)*10'd96)) begin 
//					PIXEL_COLOR <= pink; 
//				end 
//				else if (up == 1'b1 && (PIXEL_COORD_Y>=(GRID_Y*10'd96) + 10'd48)  && (PIXEL_COORD_Y<=(GRID_Y+1)*10'd96)) begin 
//					PIXEL_COLOR <= pink; 
//				end 
//				else if (down == 1'b1 && (PIXEL_COORD_Y<=((GRID_Y+1)*10'd96) - 10'd48) && (PIXEL_COORD_X>=(GRID_X)*10'd96)) begin 
//					PIXEL_COLOR <= pink; 
//				end else begin 
//					PIXEL_COLOR <= white;
//				end
				
			end else begin	
				PIXEL_COLOR <= white; 
			end

		   if (PIXEL_COORD_Y <=(10'd4+(GRID_Y*10'd96)) && PIXEL_COORD_Y >= (GRID_Y*10'd96) && currentGrid[7] == 1'b1) begin //top
				PIXEL_COLOR <= black; 
		   end

			if ((PIXEL_COORD_Y >=((GRID_Y+1)*10'd96)-10'd4) && currentGrid[5] == 1'b1) begin //bottom
				PIXEL_COLOR <= black; 
		   end
			
			if ((currentGrid[3:2] == 2'b01) && (PIXEL_COORD_X <= ((GRID_X+1)*10'd96) - 10'd7) && (PIXEL_COORD_X >= ((GRID_X+1)*10'd96) - 10'd15) && (PIXEL_COORD_Y <= ((GRID_Y+1)*10'd96) - 10'd7) && (PIXEL_COORD_Y >= ((GRID_Y+1)*10'd96) - 10'd15)) begin
				PIXEL_COLOR <= red; 
			end
			if ((currentGrid[3:2] == 2'b10) && (PIXEL_COORD_X <= ((GRID_X+1)*10'd96) - 10'd7) && (PIXEL_COORD_X >= ((GRID_X+1)*10'd96) - 10'd15) && (PIXEL_COORD_Y <= ((GRID_Y+1)*10'd96) - 10'd7) && (PIXEL_COORD_Y >= ((GRID_Y+1)*10'd96) - 10'd15)) begin
				PIXEL_COLOR <= blue; 
			end
			if ((currentGrid[3:2] == 2'b11) && (PIXEL_COORD_X <= ((GRID_X+1)*10'd96) - 10'd7) && (PIXEL_COORD_X >= ((GRID_X+1)*10'd96) - 10'd15) && (PIXEL_COORD_Y <= ((GRID_Y+1)*10'd96) - 10'd7) && (PIXEL_COORD_Y >= ((GRID_Y+1)*10'd96) - 10'd15)) begin
				PIXEL_COLOR <= green; 
			end
			if (done) begin 
				if (soundcounter == 0) begin
					soundcounter <= CLKDIVIDER_B_SIN - 1;
				end 
				if (DAC >= 255) begin
				DAC <= 0;
				end
				else begin
				DAC <=  DAC + 1;
				end 
			end 
			else begin 
				soundcounter <= soundcounter - 1;
			end
		end
		
	end

	 reg [24:0] led_counter; // timer to keep track of when to toggle LED
	 reg 			led_state;   // 1 is on, 0 is off
	 wire [1:0]	botX;
	 wire [2:0]	botY;
	 wire [1:0]	preX;
	 wire [2:0]	preY;
	 wire [3:0] wall;
	 wire [1:0] tres;
	 wire left;
	 wire right; 
	 wire up;
	 wire updateType;
	 wire down;
	 wire done;

	 
    // Module outputs coordinates of next pixel to be written onto screen
    VGA_DRIVER driver(
		  .RESET(reset),
        .CLOCK(CLOCK_25),
        .PIXEL_COLOR_IN(PIXEL_COLOR),
        .PIXEL_X(PIXEL_COORD_X),
        .PIXEL_Y(PIXEL_COORD_Y),
        .PIXEL_COLOR_OUT({GPIO_0_D[9],GPIO_0_D[11],GPIO_0_D[13],GPIO_0_D[15],GPIO_0_D[17],GPIO_0_D[19],GPIO_0_D[21],GPIO_0_D[23]}),
        .H_SYNC_NEG(GPIO_0_D[7]),
        .V_SYNC_NEG(GPIO_0_D[5])
    );
	 
	 inputReader reader(
		.valid(GPIO_1_D[24]),
		.arduinoInput({GPIO_1_D[10],GPIO_1_D[12],GPIO_1_D[14],GPIO_1_D[16],GPIO_1_D[18], GPIO_1_D[20], GPIO_1_D[22]}),
		.robotX(botX),
		.robotY(botY),
		.preX(preX),
		.preY(preY),
		.walls(wall),
		.treasure(tres), 
		.left(left),
		.right(right),
		.down(down),
		.up(up), 
		.updateType(updateType), 
		.done(done)
	);
	
	sin_rom sin(
		.addr(DAC),
		.clk(CLOCK_25),
		.q({GPIO_1_D[9],GPIO_1_D[11],GPIO_1_D[13],GPIO_1_D[15],GPIO_1_D[17],GPIO_1_D[19],GPIO_1_D[21],GPIO_1_D[23]})
	 );
	 
//	assign botX = 2'b10;
//	assign botY = 3'b010;
//	 
	 localparam explored = 8'b00000001;
	 localparam unexplored = 8'b00000000;
	 localparam currPos = 2'b11;

	 
	 assign reset = ~KEY[0]; // reset when KEY0 is pressed
	 
	// assign PIXEL_COLOR = 8'b000_111_00; // Green
	// assign LED[0] = led_state;
	 
    //=======================================================
    //  Structural coding
    //=======================================================
 
	 // Generate 25MHz clock for VGA, FPGA has 50 MHz clock
    always @ (posedge CLOCK_50) begin
        CLOCK_25 <= ~CLOCK_25; 
    end // always @ (posedge CLOCK_50)
	
	 // Simple state machine to toggle LED0 every one second
	 always @ (posedge CLOCK_25) begin
		  if (reset) begin
				led_state   <= 1'b0;
				led_counter <= 25'b0;

				grid1[0][0] = 8'b00000000;
				grid1[0][1] = 8'b00000000;
				grid1[0][2] = 8'b00000000;
				grid1[0][3] = 8'b00000000;
				grid1[0][4] = 8'b00000000;
				grid1[1][0] = 8'b00000000;
				grid1[1][1] = 8'b00000000;
				grid1[1][2] = 8'b00000000;
				grid1[1][3] = 8'b00000000;
				grid1[1][4] = 8'b00000000;
				grid1[2][0] = 8'b00000000;
				grid1[2][1] = 8'b00000000;
				grid1[2][2] = 8'b00000000;
				grid1[2][3] = 8'b00000000;
				grid1[2][4] = 8'b00000000;
				grid1[3][0] = 8'b00000000;
				grid1[3][1] = 8'b00000000;
				grid1[3][2] = 8'b00000000;
				grid1[3][3] = 8'b00000000;
				grid1[3][4] = 8'b00000000;
		  end
		  
		  else begin 
			grid1[preX][preY] = grid1[preX][preY] & 8'b11111101;
			if (updateType == 1'b0) begin
				grid1[botX][botY] = grid1[botX][botY] & 8'b11111100;
				grid1[botX][botY] = grid1[botX][botY] | currPos;
			end else begin
				grid1[botX][botY] = {wall,tres,currPos};
				end 
		  end
	 end
	 

endmodule
