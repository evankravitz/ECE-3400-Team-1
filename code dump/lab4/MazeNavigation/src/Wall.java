import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.geom.Rectangle2D;

import javax.swing.JPanel;

public class Wall {
	
	int x;
	int y;
	int width;
	int height;
	
	public Wall(int x, int y, int width, int height){
		this.x = x;
		this.y = y;
		this.width = width;
		this.height = height;
	}
	
	

	
	public void draw(Graphics g){
		Graphics2D g2d = (Graphics2D) g;
		Rectangle2D.Double wall = new Rectangle2D.Double(x,y, width, height);
		g2d.setColor(Color.GRAY);
		g2d.fill(wall);
	}
}
