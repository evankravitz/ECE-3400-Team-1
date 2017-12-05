/** Returns a 9 by 11 array of bytes encoding the maze
 *  Walls:  Refers to wall if one index is even and the other is odd. 
 *          0 is no wall, 1 is wall (in binary)
 *  Tile:   Refers to tile if both indices are odd.
 *          0 is no treasure, 1 is 7kHz, 2 is 12kHz, 3 is 17kHz (in binary)
 *  Ignore: If both indices are even, there's no information, will be 0
 */
#define noWall 0
#define wall 1
#define unvisited 2
#define noTreasure 3
#define seven 4
#define twelve 5
#define seventeen 6

//void instantiateMaze() {
//
////  for (int i = 0; i <= 8; i++) {
////    for (int j = 0; j<= 10; j++) {
////      if (i%2 == 1 && j%2 == 1) {
////          maze[i][j]= (uint8_t) unvisited;
////       
////        }
////        if (i%2 == 0 && j%2 == 0) {
////          maze[i][j]= (uint8_t) B0;
////        }
////        if ((i%2 == 0 && j%2 == 1) || (i%2 == 1 && j%2 ==0)) {
////          maze[i][j] = (uint8_t) noWall;
////        }
////    }
////  }
////  for (int i = 0; i<= 8; i++){
////      for (int j = 0; j<= 10; j++){
////        if((i==0 || i== 8) && j%2 == 1) {
////          maze[i][j] = (uint8_t) wall;
////        }
////        if ((j==0 || j== 10) && i%2 == 1) {
////          maze[i][j] = (uint8_t) wall;
////        }
////      }
////    }
////
////  for (int i = 0; i<= 8; i++){
////      for (int j = 0; j<= 10; j++){
////        (uint8_t) out = maze[i][j];
////      }
////  }
//}

