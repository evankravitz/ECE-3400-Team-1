import java.awt.Graphics;
import java.awt.Graphics2D;

public class EndScreen2 {
	
	public int WIDTH;
	public int HEIGHT;
	
	public EndScreen2(int width, int height){
		this.WIDTH =  width;
		this.HEIGHT = height;
		
	}
	
	public void draw(Graphics g){
		Graphics2D g2d = (Graphics2D) g;
		g2d.drawString("DONE",WIDTH/2,HEIGHT/2);
		

	}
}
