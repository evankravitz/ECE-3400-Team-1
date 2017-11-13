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
	 localparam white = 8'b11111111;
	 localparam black = 8'b0;
	 localparam pink = 8'b11110011;
	 localparam cyan = 8'b10011011;
	 localparam magenta = 8'b11100011; 
	 
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
	 
	//state machine 
	always @(posedge CLOCK_25) begin
		if (GRID_X > 3) begin
			PIXEL_COLOR <= black;
		end
		else begin
		currentGrid <= grid1[GRID_X][GRID_Y];
			if (currentGrid == unexplored) begin
				PIXEL_COLOR <= white;
			end
			if (currentGrid == explored) begin
				PIXEL_COLOR <= white;
			end
			if (currentGrid[1:0] == currPos) begin
				PIXEL_COLOR <= black;
			end
		   if (PIXEL_COORD_X < 10'd96*(GRID_X+1) && PIXEL_COORD_Y <(10'd12+(GRID_Y*10'd96)) && currentGrid[7] == 1'b1) begin
				PIXEL_COLOR <= magenta; 
		   end
//			if (PIXEL_COORD_Y < 10'd96*(GRID_Y+1) && PIXEL_COORD_X<(10'd12+(GRID_X*10'd96)) && currentGrid[6] == 1'b1) begin 
//				PIXEL_COLOR <= pink; 
//			end else begin 
//				PIXEL_COLOR <= white; 
//			end
			if (PIXEL_COORD_X < 10'd96*(GRID_X+1) && (PIXEL_COORD_Y >((GRID_Y+1)*10'd96)-10'd12) && (PIXEL_COORD_Y <((GRID_Y+1)*10'd96)) && currentGrid[5] == 1'b1) begin
				PIXEL_COLOR <= magenta; 
		   end
			
			
		end
		//if (PIXEL_COORD_X < (10'd96*(GRIX_X+1)) && PIXEL_COORD_Y <
	end

	 reg [24:0] led_counter; // timer to keep track of when to toggle LED
	 reg 			led_state;   // 1 is on, 0 is off
	 wire [1:0]	botX;
	 wire [2:0]	botY;
	 wire [1:0]	preX;
	 wire [2:0]	preY;
	 wire [3:0] wall;
	 wire [1:0] tres;

	 
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
	 
//	 inputReader reader(
//		.valid(GPIO_1_D[8]),
//		.arduinoInput({GPIO_1_D[10],GPIO_1_D[12],GPIO_1_D[14],GPIO_1_D[16],GPIO_1_D[18], GPIO_1_D[20], GPIO_1_D[22], GPIO_1_D[24]}),
//		.robotX(botX),
//		.robotY(botY),
//		.preX(preX),
//		.preY(preY),
//		.walls(wall),
//		.treasure(tres)
//	);
	 
	 assign botX = 2'b10;
	 assign botY = 3'b010;
	 
	 localparam explored = 8'b00000001;
	 localparam unexplored = 8'b00000000;
	 localparam currPos = 2'b11;
	 
	 assign reset = ~KEY[0]; // reset when KEY0 is pressed
	 
	// assign PIXEL_COLOR = 8'b000_111_00; // Green
	 assign LED[0] = led_state;
	 
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

				grid1[0][0] = unexplored;
				grid1[0][1] = 8'b10100000;
				grid1[0][2] = unexplored;
				grid1[0][3] = unexplored;
				grid1[0][4] = unexplored;
				grid1[1][0] = 8'b10100000;
				grid1[1][1] = unexplored;
				grid1[1][2] = unexplored;
				grid1[1][3] = unexplored;
				grid1[1][4] = unexplored;
				grid1[2][0] = unexplored;
				grid1[2][1] = 8'b10100000;
				grid1[2][2] = unexplored;
				grid1[2][3] = unexplored;
				grid1[2][4] = unexplored;
				grid1[3][0] = unexplored;
				grid1[3][1] = unexplored;
				grid1[3][2] = unexplored;
				grid1[3][3] = unexplored;
				grid1[3][4] = unexplored;
		  end
		  
//		  else begin 
//			grid1[preX][preY] = grid1[preX][preY] | explored;
//			grid1[preX][preY] = grid1[preX][preY] & 8'b11111101;
//			grid1[botX][botY] = {wall,tres,currPos};
//		  end
	 end
	 

endmodule 
