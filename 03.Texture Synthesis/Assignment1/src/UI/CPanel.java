/**
 * PURPOSE
      This class is in order to output set of images
 * @author peter
 */
package UI;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.image.BufferedImage;

import javax.swing.BorderFactory;
import javax.swing.JPanel;

public class CPanel extends JPanel{
	private static final long serialVersionUID = 1002;
	private BufferedImage[] imgs;
	private int col = 1;
//	private JPanel info;
	/**
	 * Constructor 
	 * @param title 此panel的名字
	 * @param imgs 要產生的影像
	 * @param info 要相對於哪一個panel去進行output
	 */
	public CPanel(String title ,BufferedImage[] imgs) {
		setImages(imgs);
		setBorder( BorderFactory.createTitledBorder(title) );
		
	}
	/**
	 * 
	 * @param title 此panel的名字
	 * @param num 大概有多少個img以方便一直加下去
	 * @param panel 要相對於哪一個panel去進行output
	 */
	public CPanel(String title ,int num ) {
		setBorder( BorderFactory.createTitledBorder(title) );
		imgs = new BufferedImage[num];
	}
	/**
	 * 設定顯示圖檔的時候col要幾個
	 * @param c
	 */
	public void setCol(int c) {
		col = c;
		setSize();
	}
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		if ( imgs != null ) {
			int x = 10 ,y = 15;//畫圖的位置
			int width = 0 ,height = 0;
			int i = 0;
			for ( BufferedImage img : imgs ) {
				width = img.getWidth();
				height = img.getHeight();
				g.drawImage(img, x, y, width, height, this );
				x +=img.getWidth() + 5;
				if ( ++i % col == 0 ) {
					y+= img.getHeight() + 5;
					x = 10;
				}

			}
			
		}
		
	}
	
	public void setImages(BufferedImage[] imgs) {
		this.imgs = imgs;
		setCol(10);
	}
	/**
	 * add image to <code>images[i]</code>
	 * @param img image to add
	 * @param i the index of the image array
	 */
	public void addImage(BufferedImage img ,int i) {
		imgs[i] = img;
	}
	private void setSize() {
		int x = 10 ,y = 15;//畫圖的位置
		int width = 0 ,height = 0;
		int i = 0;
		int tx = x;
		for ( BufferedImage img : imgs ) {
			width = img.getWidth();
			height = img.getHeight();
			x +=img.getWidth() + 5;
			if ( ++i % col == 0 ) {
				y+= img.getHeight() + 5;
				x = 10;
			}
			if ( tx < x ) tx = x;

		}
		
		setPreferredSize(new Dimension(tx + width, y + height+10) );
	}
	
	

}
