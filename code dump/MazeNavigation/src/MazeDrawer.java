import javax.swing.JPanel;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import javax.swing.JFrame;
import javax.swing.JOptionPane;


public class MazeDrawer extends JPanel {
	
	 private List<Object> shapes = new ArrayList<>();
	 public static final int WIDTH = 500;
	 public static final int HEIGHT = 500;



	 public MazeDrawer(String[][] maze, int[] currPos){
	        setPreferredSize(new Dimension(WIDTH, HEIGHT));
	        drawMaze(maze, currPos);
	     
	 }
	 
	 

	 public static void main(String[] args){
		 JFrame frame = new JFrame();
		 String[][] maze = generateMaze();
		 int[] currPos = new int[2];
		 currPos[0] = 0;
		 currPos[1] = 0;
		 frame.add(new MazeDrawer(maze, currPos));
		 
		
		 
		 
		 frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	     frame.pack();
	     frame.setLocationRelativeTo(null);
	     frame.setVisible(true);
		 
		 
	 }
	 
	 /**Creates a blank maze, returns maze
	  */
	 private static String[][] generateMaze() {
		 String[][] maze = new String[7][9];
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
		return maze;
		
				
			
		
	}


		
	
	 
	public void drawMaze(String[][] maze, int[] currPosArray){
		 for (int i = 0; i<7; i++){
				for (int j = 0; j<9; j++){
					if ((i%2 == 1 && j%2 == 0) ||(i%2 == 0 && j%2 == 1) ){ //possible wall locaation
						if(maze[i][j]=="Wall"){
							addWall(i,j);
						}
					}
					else if (i%2 ==0 && j%2 ==0){ //square location
						if (i==currPosArray[0] && j==currPosArray[1]){
							addCurrPos(i/2, j/2);
						}
						else{
							if (maze[i][j].equals("Unexplored")){
								addUnvisited(i/2, j/2);			
							}
							else{
								addVisited(i/2, j/2);
							}
						}	
					}
				}
			}
	 }
	 
	/** adds a Visited square at i,j
	 */	
	 private void addVisited(int i, int j) {
		 int x = (int) Math.floor((i/4.0)*WIDTH);
			int y =  (int) Math.floor((j/5.0)*HEIGHT);
			int width = (int) Math.floor(WIDTH/4.0);
			int height = 	(int) Math.floor(HEIGHT/5.0);
			shapes.add(new Visited(x,y,width,height));
			repaint();
	}
	 /** adds an Unvisited square at i,j
	  */
	private void addUnvisited(int i, int j) {
		 int x = (int) Math.floor((i/4.0)*WIDTH);
			int y =  (int) Math.floor((j/5.0)*HEIGHT);
			int width = (int) Math.floor(WIDTH/4.0);
			int height = 	(int) Math.floor(HEIGHT/5.0);
			shapes.add(new Unvisited(x,y,width,height));
			repaint();
	}

	/** adds a CurrPos square at i,j
	 */
	private void addCurrPos(int i, int j) {
		 int x = (int) Math.floor((i/4.0)*WIDTH);
			int y =  (int) Math.floor((j/5.0)*HEIGHT);
			int width = (int) Math.floor(WIDTH/4.0);
			int height = 	(int) Math.floor(HEIGHT/5.0);
			shapes.add(new CurrPos(x,y,width,height));
			repaint();
	}

	/** adds a wall at position i,j
	 */
	private void addWall(int i, int j) {
		int x,y,width,height;
		//vertical wall
		if (i%2 ==1){
			
			int relativeX = (i+1)/2;
			int relativeY = j/2;
			 width = (int) Math.floor((1/4.0)*(float)WIDTH*(0.1));
			 height = (int) Math.floor((1/5.0)*(float)HEIGHT);
			 x = (int) Math.floor(((relativeX/4.0)*WIDTH - width*0.5));
			 y = (int) (Math.floor((relativeY/5.0)*HEIGHT));
			
		}
		//horizontal wall
		else{
		
			int relativeX = i/2;
			int relativeY = (j+1)/2;
			 width = (int) Math.floor((1/4.0)*WIDTH);
			 height = (int) Math.floor((1/5.0)*HEIGHT*(0.1));
			 x = (int) (Math.floor((relativeX/4.0)*WIDTH));
			 y = (int) Math.floor(((relativeY/5.0)*HEIGHT - height*.5));

		}		
		shapes.add(new Wall(x,y, width, height));
		repaint();
	}

	@Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        for (Object s : shapes) {
            if (s instanceof Visited) {
                ((Visited) s).draw(g);
            } else if (s instanceof Unvisited) {
                ((Unvisited) s).draw(g);
            }
            else if (s instanceof CurrPos) {
                ((CurrPos) s).draw(g);
            }
            else if (s instanceof Wall) {
                ((Wall) s).draw(g);
            }
        }
    }
	 

}
