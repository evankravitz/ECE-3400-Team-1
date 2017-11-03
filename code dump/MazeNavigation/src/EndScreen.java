import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;

import javax.swing.JPanel;

public class EndScreen extends JPanel {
	private EndScreen2 endScreen;
	
	public EndScreen(int width, int height){
        setPreferredSize(new Dimension(width, height));
		this.endScreen = new EndScreen2(width, height);
		repaint();
	}
	
	@Override
	protected void paintComponent(Graphics g){
		super.paintComponent(g);
		endScreen.draw(g);
	}
}
