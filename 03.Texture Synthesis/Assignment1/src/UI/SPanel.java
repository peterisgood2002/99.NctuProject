/**
 * PURPOSE
      to produce the control panel of source image in this application
 * @author peter
 */
package UI;

import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SpringLayout;


public class SPanel extends JPanel implements ActionListener{
	public static final long  serialVersionUID = 1001;
	private BufferedImage img;
	private JTextField numofs;//指定要多少samples
	//每一個sample大小多大
	private JTextField height;
	private JTextField width;
	//指定output image大小多大
	//private JTextField imgwidth;
	//private JTextField imgheight;
	public SPanel() {
		img = null;
		setComponent();
		
		//設定SPanel整體Layout向左對齊
		FlowLayout l = (FlowLayout) getLayout();
		l.setAlignment( FlowLayout.LEFT);
		
	}
	private JPanel p;
	private JButton b;
	public void setComponent() {
		
		//基本相關設定的Panel
		p = new JPanel();
		p.setBorder(BorderFactory.createTitledBorder("基本設定"));
		//設定layout
		SpringLayout layout = new SpringLayout();
		p.setLayout( layout );
		
		//設定要多少samples之Text file
		JLabel label = new JLabel("number of samples：");
		numofs = new JTextField("4" ,5);
		p.add(label );
		p.add(numofs );
		
		layout.putConstraint(SpringLayout.WEST, numofs, 5, SpringLayout.EAST,label );//將numofs = label + 5
		//設定要sample要多大的panel
		JPanel panel = new JPanel();
		panel.setBorder(BorderFactory.createTitledBorder("the size of sample"));
		panel.setLayout( new GridLayout(2,2) );//第二個panel用Grid來安排
		JLabel lh = new JLabel("Height：");
		height = new JTextField("32" ,5);
		JLabel lw = new JLabel("width：");
		width = new JTextField("32" ,5);
		panel.add( lh);
		panel.add( height );
		panel.add( lw);
		panel.add( width );
		p.add( panel );
		layout.putConstraint(SpringLayout.NORTH, panel, 5, SpringLayout.SOUTH, label);//此panel在nos下面
		b = new JButton("produce Tile set");
		b.addActionListener( this );
		p.add(b);
		
		//讓整個layout大小能夠符合整個panel大小
		layout.putConstraint(SpringLayout.EAST, p, 5, SpringLayout.EAST, numofs);
		layout.putConstraint(SpringLayout.SOUTH, p, 5, SpringLayout.SOUTH, b);
		add(p);
		
		layout.putConstraint(SpringLayout.NORTH, b, 5, SpringLayout.SOUTH, panel);
		
		
		
	}
	/*如果有 則畫圖*/
	public void update(Graphics g) {
		repaint();
	}
	
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2 = (Graphics2D) g;
		int w = p.getX() + p.getPreferredSize().width + 5;
		int h = p.getY() + p.getPreferredSize().height + 5;
		//如果有讀圖就畫圖的panel
//		Application a = (Application) getTopLevelAncestor();//得到上一層的資訊
		if ( img != null ) {
			g2.drawImage(img, w+30, 10, img.getWidth(), img.getHeight(), null);//從此Panel中的00開始畫起
			w += img.getWidth() +35;
			if ( img.getHeight()+10 >= h) 
				h = img.getHeight() + 10;
		}
		this.w = w;
		this.h = h+5;
	}
	@Override
	public void actionPerformed(ActionEvent e) {
		if ( e.getSource() == b){
			/*取得Wang tile並產生Tiles*/
			Application a = (Application) getTopLevelAncestor();//得到上一層的資訊
			int num = Integer.parseInt( numofs.getText());
			int w = Integer.parseInt( width.getText() );
			int h = Integer.parseInt( height.getText() );
			//System.out.printf("%d %d %d", num ,w ,h);
			a.reSet();
			a.setSamplesPanel(num ,w , h);
			a.setTilePanel();
		}
	}
	/**
	 * 設定source圖檔
	 * @param img
	 * @return 如果設定成功回傳true 設定失敗回傳false
	 */
	int w , h;
	public boolean setImage( BufferedImage img ) {
		try {
			this.img = img;
			w = p.getX() + p.getPreferredSize().width + 5;
			h = p.getY() + p.getPreferredSize().height + 5;
			
			w += img.getWidth() +45;
			if ( img.getHeight()+10 >= h) 
				h = img.getHeight() + 10;
			setPreferredSize(new Dimension(w , h+5) );//讓Layout好好Layout
			invalidate();//讓JFrame的Layout調整其大小 
			
			return true;
		}catch ( NullPointerException ne) {
			return false;
		}
	}
	/*
	 * in order to let layout manager to layout this component
	 */
	public Dimension getMinimumSize() { 
		
		return new Dimension(w, h+5); 
	}

	public Dimension getPreferredSize() { 
		return getMinimumSize(); 
	}


}
