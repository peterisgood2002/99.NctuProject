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
	private JTextField numofs;//���w�n�h��samples
	//�C�@��sample�j�p�h�j
	private JTextField height;
	private JTextField width;
	//���woutput image�j�p�h�j
	//private JTextField imgwidth;
	//private JTextField imgheight;
	public SPanel() {
		img = null;
		setComponent();
		
		//�]�wSPanel����Layout�V�����
		FlowLayout l = (FlowLayout) getLayout();
		l.setAlignment( FlowLayout.LEFT);
		
	}
	private JPanel p;
	private JButton b;
	public void setComponent() {
		
		//�򥻬����]�w��Panel
		p = new JPanel();
		p.setBorder(BorderFactory.createTitledBorder("�򥻳]�w"));
		//�]�wlayout
		SpringLayout layout = new SpringLayout();
		p.setLayout( layout );
		
		//�]�w�n�h��samples��Text file
		JLabel label = new JLabel("number of samples�G");
		numofs = new JTextField("4" ,5);
		p.add(label );
		p.add(numofs );
		
		layout.putConstraint(SpringLayout.WEST, numofs, 5, SpringLayout.EAST,label );//�Nnumofs = label + 5
		//�]�w�nsample�n�h�j��panel
		JPanel panel = new JPanel();
		panel.setBorder(BorderFactory.createTitledBorder("the size of sample"));
		panel.setLayout( new GridLayout(2,2) );//�ĤG��panel��Grid�Ӧw��
		JLabel lh = new JLabel("Height�G");
		height = new JTextField("32" ,5);
		JLabel lw = new JLabel("width�G");
		width = new JTextField("32" ,5);
		panel.add( lh);
		panel.add( height );
		panel.add( lw);
		panel.add( width );
		p.add( panel );
		layout.putConstraint(SpringLayout.NORTH, panel, 5, SpringLayout.SOUTH, label);//��panel�bnos�U��
		b = new JButton("produce Tile set");
		b.addActionListener( this );
		p.add(b);
		
		//�����layout�j�p����ŦX���panel�j�p
		layout.putConstraint(SpringLayout.EAST, p, 5, SpringLayout.EAST, numofs);
		layout.putConstraint(SpringLayout.SOUTH, p, 5, SpringLayout.SOUTH, b);
		add(p);
		
		layout.putConstraint(SpringLayout.NORTH, b, 5, SpringLayout.SOUTH, panel);
		
		
		
	}
	/*�p�G�� �h�e��*/
	public void update(Graphics g) {
		repaint();
	}
	
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2 = (Graphics2D) g;
		int w = p.getX() + p.getPreferredSize().width + 5;
		int h = p.getY() + p.getPreferredSize().height + 5;
		//�p�G��Ū�ϴN�e�Ϫ�panel
//		Application a = (Application) getTopLevelAncestor();//�o��W�@�h����T
		if ( img != null ) {
			g2.drawImage(img, w+30, 10, img.getWidth(), img.getHeight(), null);//�q��Panel����00�}�l�e�_
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
			/*���oWang tile�ò���Tiles*/
			Application a = (Application) getTopLevelAncestor();//�o��W�@�h����T
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
	 * �]�wsource����
	 * @param img
	 * @return �p�G�]�w���\�^��true �]�w���Ѧ^��false
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
			setPreferredSize(new Dimension(w , h+5) );//��Layout�n�nLayout
			invalidate();//��JFrame��Layout�վ��j�p 
			
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
