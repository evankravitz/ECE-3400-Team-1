import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Deque;
import java.util.HashSet;

import javax.swing.JFrame;

public class MazeNavigator {

	
	String[][] actualMaze = new String[7][9];
	String[][] maze = new String[7][9];
	int[] currPos = new int[2];
	HashSet<String> frontierSet= new HashSet<String>();
	Deque<int[]> visitedStack = new ArrayDeque<int[]>();
	JFrame frame = new JFrame();
	MazeDrawer oldMaze;

	
	
	/**
	 * 	Note that elements with odd x and odd y
	 * indices don't actually represent maze data.
	 * These are extraneous elements. 
	 * 
	 * Also note that to convert from the 4x5 coordinates
	 * To this coordinate system, multiply the x index and y index by 2.
	 * @throws InterruptedException 
	 */
	public MazeNavigator() throws InterruptedException {
		resetMaze();
		initializeCurrentPosition();
		frontierSet.add(Arrays.toString(currPos));
		initializeActualMaze();
		visitedStack.push(currPos);
		createJFrame();
		displayActualMaze();
		while (!frontierSet.isEmpty()){
			updateJFrame();
			Thread.sleep(100);
			maze[currPos[0]][currPos[1]] = "Explored";
			//System.out.println(Arrays.toString(currPos));
			removeCurrentPosFromFrontier();
			ArrayList<int[]> reachableCells = getReachableCellsAndAddWalls();
			addUnvisitedSurroundingNodesToFrontier(reachableCells);
			updateCurrPosAndVisitedSet(reachableCells);
			if (!frontierSet.isEmpty()){
				updateJFrame();
			}
		}
		printDone();
		
		
		
		

	}
	
	
	private void printDone() {
		frame.remove(oldMaze);
		EndScreen endScreen = new EndScreen(500, 500);
		frame.add(endScreen);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	    frame.pack();
	    frame.setVisible(true);
		
	}


	private void displayActualMaze() {
		JFrame otherFrame = new JFrame();
		otherFrame.add(new MazeDrawer(actualMaze, new int[]{30, 30}));
		otherFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		otherFrame.pack();
		otherFrame.setLocationRelativeTo(null);
		otherFrame.setVisible(true);
	}


	private void updateJFrame(){
		 frame.remove(oldMaze);
		 MazeDrawer newMaze = new MazeDrawer(maze, currPos);
		 frame.add(newMaze);
		 oldMaze = newMaze;
		 frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	     frame.pack();
	    // frame.setLocationRelativeTo(null);
	     frame.setVisible(true);
	}
	
	private void createJFrame(){
		frame = new JFrame();
		oldMaze = new MazeDrawer(maze, currPos);
		frame.add(oldMaze);
		
	}
	
	private void printMaze() {
		for (int y = 0; y<9; y++){
			System.out.println("");
			System.out.println("_____________________");
			for (int x = 0; x<7; x++){
				System.out.print(" || ");
				System.out.print(maze[x][y]);
			}
		}		
	}

	//Load test data here
	private void initializeActualMaze() {
		for (int i = 0; i<7; i++){
			for (int j = 0; j<9; j++){
				actualMaze[i][j] = "No Wall";
			}
		}
		
		actualMaze[0][1] = "Wall";

		actualMaze[1][0] = "Wall";
		actualMaze[1][1] = "Wall";
		actualMaze[1][2] = "Wall";
		actualMaze[1][3] = "Wall";
		actualMaze[1][4] = "Wall";
		actualMaze[1][5] = "Wall";
		
		actualMaze[3][3] = "Wall";
		actualMaze[4][3] = "Wall";
		actualMaze[5][3] = "Wall";
		actualMaze[6][3] = "Wall";
		actualMaze[3][6] = "Wall";
		actualMaze[5][2] = "Wall";
		actualMaze[0][2] = "Wall";
		actualMaze[3][2] = "Wall";

		actualMaze[2][5] = "Wall";
		actualMaze[4][5] = "Wall";
		//actualMaze[6][5] = "Wall";





		
	}



	private void addUnvisitedSurroundingNodesToFrontier(ArrayList<int[]> reachableCells) {
		for (int[] cell : reachableCells){
			if (maze[cell[0]][cell[1]].equals("Unexplored")){
				frontierSet.add(Arrays.toString(cell));
			}
		}
		
	}

	private void updateCurrPosAndVisitedSet(ArrayList<int[]> reachableCells) {
		boolean foundUnexplored = false;
		int[] newCurrPos = new int[2];
		for (int[] cell : reachableCells){
			if (maze[cell[0]][cell[1]].equals("Unexplored")){
				foundUnexplored = true;
				newCurrPos = cell;
				break;
			}
		}
		if (!foundUnexplored){
			newCurrPos = visitedStack.pop();
		}
		else{
			visitedStack.push(currPos);
		}
		this.currPos = newCurrPos;
		
		
	}

	private void removeCurrentPosFromFrontier() {
		if (frontierSet.contains(Arrays.toString(currPos))){
			frontierSet.remove(Arrays.toString(currPos));
		}
		
	}

	private ArrayList<int[]> getReachableCellsAndAddWalls() {
		ArrayList<int[]> reachableCells = new ArrayList<>();
		ArrayList<int[]> validCoordinates = getValidMazeCoordinates();
		for (int [] validCoord : validCoordinates){
			maze[validCoord[0]][validCoord[1]] = actualMaze[validCoord[0]][validCoord[1]];
			if (actualMaze[validCoord[0]][validCoord[1]].equals("No Wall")){
				int[] reachableCell = findReachableCell(validCoord);
				reachableCells.add(reachableCell);
			}
		}
		return reachableCells;
	}


	private int[] findReachableCell(int[] validCoord) {
		int dx = validCoord[0]- currPos[0];
		int dy = validCoord[1] - currPos[1];
		int[] reachableCell;
		if (dx == 1){
			reachableCell =  new int[]{currPos[0]+dx+1, currPos[1]};
		}
		else if (dx == -1){
			reachableCell =  new int[]{currPos[0]+dx-1, currPos[1]};
		}
		else if (dy == 1){
			reachableCell =  new int[]{currPos[0], currPos[1] + dy +1};
		}
		else{
			reachableCell =  new int[]{currPos[0], currPos[1] + dy -1};
		}
		return reachableCell;
	}
	
	private ArrayList<int[]> getValidMazeCoordinates() {
		ArrayList<int[]> validMazeCoordinates = new ArrayList<int[]>();

		if (currPos[0]+1 <=6){
			validMazeCoordinates.add(new int[]{currPos[0]+1, currPos[1]});
		}
		if (currPos[0]-1 >=0){
			validMazeCoordinates.add(new int[]{currPos[0]-1, currPos[1]});
		}
		if (currPos[1] +1 <=8){
			validMazeCoordinates.add(new int[]{currPos[0], currPos[1]+1});
		}
		if (currPos[1] -1 >=0){
			validMazeCoordinates.add(new int[]{currPos[0], currPos[1]-1});
		}
		return validMazeCoordinates;
		
	}

	private void initializeCurrentPosition() {
		currPos[0] = 6;
		currPos[1] = 8;
	}

	public void resetMaze(){
		for (int x = 0; x<7; x++){
			for (int y = 0; y<9; y++){
				if (x%2 ==0 || y%2 ==0){
					maze[x][y]="Unexplored";
				}
				else{
					maze[x][y]="Ignore";
				}
			}
		}
		
		
	}
	
	

}
