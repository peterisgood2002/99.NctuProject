/**
 * PURPOSE
      This class is in order to show the application for output
 * @author peter
 */
package UI;


import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.KeyEvent;
import java.awt.image.BufferedImage;
import java.io.File;
import java.lang.reflect.Method;

import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.SpringLayout;
import javax.swing.filechooser.FileNameExtensionFilter;

import WangTile.Tile;
import WangTile.WTGraphCut;
import WangTile.WangTiler;

public class Application extends JFrame implements ActionListener ,ItemListener {
	public static final long  serialVersionUID = 1000;
	String defaultdir;//���ε{����directory�b����
	WangTiler wt;
	public Application( String title ,String ddir) {
		setTitle(title);
		defaultdir = ddir;
		wt = null;
		/*Debug�ΰO�o�R��*/
		wt = new WTGraphCut(defaultdir +"ropenet.gif");
		wt.pickSamples(4, 32, 32);
		/*Debug�����O�o�R��*/
		setComponent();
	}
	
	SPanel srinfo;
	JTabbedPane tp;
	JPanel tp1;
	JPanel tp2;
	JPanel tp3;//�Ψӹ�Graph cut�i�氻���� �O�o�R��
	public void setComponent() {
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);//JFrame�����Y���}
//		SpringLayout layout = new SpringLayout();
//		getContentPane().setLayout( layout );
		//�إ�menu bar
		JMenuBar menubar = constructMenuBar();
		setJMenuBar( menubar );
		
		//���ͤ@��Tab Panel �Ӧs�@�Ǹ�T�M���G
		tp = new JTabbedPane();
		tp1 = new JPanel();
		tp2 = new JPanel();
		tp.addTab("Wang Tile Setting", tp1);
		tp.addTab("Wang Tile Result", tp2);
		tp.setEnabledAt(1, false);
		/*�إ�Wang tile setting��Panel*/
		SpringLayout layout = new SpringLayout();
		tp1.setLayout( layout );
		//�إ�Source image�������]�w��panel
		
		srinfo = new SPanel();
		srinfo.setBorder(BorderFactory.createTitledBorder("source"));
		/*Debug�ΰO�o�R��*/
		srinfo.setImage( wt.getSourceImage() );
		/*Debug�����O�o�R��*/
		tp1.add( srinfo );
		
		/*Debug�ΰO�o�R��*/
//		setSamplesPanel(4 ,32 ,32);
//		setTilePanel();
		/*Debug�����O�o�R��*/
		
		/*Debug�ΰO�o�R��*/
//		tp3 = setDebugPanel();
//		tp.addTab("Tile Graph cut", tp3);
		/*Debug�����O�o�R��*/

		getContentPane().add(tp);
		setSize(1000 ,1000);
		setVisible( true);
		
	}

	/*
	 * Debug�� �S�b�ήɷ|�R��
	*/
	private CPanel setDebugPanel() {
		CPanel debug = new CPanel("Debug Tile image" ,3);
		BufferedImage[] timg = wt.getTileImgs();
		debug.addImage(timg[0], 0);
		Tile[][][] tile = wt.getTiles();
		Tile t = tile[0][0][0];
		debug.addImage( t.stitch( wt.getSourceImage()), 1 );
		try {
			debug.addImage(t.getStitchImg(-1), 2);
		}catch (NullPointerException e) {
			System.out.println( e.toString() );
		}
		debug.setCol(10);
		debug.repaint();
			
		return debug;
	}
	/*
	 * to build a Menu bar
	 */
	private JMenuBar constructMenuBar() {
		JMenuBar mb = new JMenuBar();
		
		/*�إ߲Ĥ@��menu*/
		JMenu menu = new JMenu("�ɮ�(F)");//title
		menu.setMnemonic(KeyEvent.VK_F);//��L���s
		
		JMenuItem mitem = new JMenuItem("read texture" ,KeyEvent.VK_R);//�ɮפ���Ū��
		mitem.setAccelerator( KeyStroke.getKeyStroke(
		        				KeyEvent.VK_R, ActionEvent.ALT_MASK));
		mitem.addActionListener(this);
		menu.add(mitem);
		
		mitem = new JMenuItem("write texture" ,KeyEvent.VK_S);//�ɮפ���Ū��
		mitem.setAccelerator( KeyStroke.getKeyStroke(
				KeyEvent.VK_S, ActionEvent.ALT_MASK));
		//�[�J�ƥ�
		mitem.addActionListener(this);
		menu.add(mitem);
		
		mb.add( menu );
		
		return mb;
	}

	/**
	 * set the samples panel to the frame
	 */
	CPanel simg;
	public void setSamplesPanel(int num ,int w ,int h) {
		try{
			wt.pickSamples(num, w, h);
			simg = new CPanel("sample images" ,wt.getSampleImage() );
//			simg.setSize(200, 200);
//			System.out.println(srinfo.getWidth());
//			simg.setLocation( srinfo.getWidth()+5, 0);
			/*Debug�ΰO�o�R��*/
//			simg.setLocation( 514, 0);
			/*Debug�����O�o�R��*/
//			getContentPane().add( simg );
			tp1.add(simg);
			//�]�wSample panel��layout
			SpringLayout layout = (SpringLayout) tp1.getLayout();
			layout.putConstraint(SpringLayout.WEST, simg, 5, SpringLayout.EAST, srinfo);
			
			validate();
//			simg.repaint();
		}catch (NullPointerException ne) {
			System.out.println("���V���~���O����Ŷ�");
		}
	}
	/**
	 * set the tile panel in order to check error
	 */
	CPanel tilec;
	JPanel resultset;
	public void setTilePanel() {
		if ( wt.hasSamples()) {
			wt.produceTiles();
			tilec = new CPanel ( "Tiles" , wt.getTileImgs());

			tp1.add(tilec);
			//�]�wTile panel��layout
			SpringLayout layout = (SpringLayout) tp1.getLayout();
			layout.putConstraint(SpringLayout.NORTH, tilec, 5, SpringLayout.SOUTH, srinfo);
//			tilec.repaint();
			
			
			/*�[�J����Wang tile synthesis�������]�w��Panel*/
			resultset = setResultSetPanel();
			tp1.add(resultset);
//			resultset.setPreferredSize(new Dimension(200, 200));
			layout.putConstraint(SpringLayout.WEST, resultset, 5, SpringLayout.EAST, srinfo);
			layout.putConstraint(SpringLayout.NORTH, resultset, 5, SpringLayout.SOUTH, simg);
			
			validate();//��layout���D������ܤF
		}
	}
	
	JTextField width;//�X�����e��
	JTextField height;//�X��������
	public JPanel setResultSetPanel() {
		JPanel panel = new JPanel(); 
		panel.setBorder(BorderFactory.createTitledBorder("Synthesis setting"));
		
		/*�������*/
		JLabel swidth = new JLabel("Width�G");
		width = new JTextField("256" ,5);
		JLabel sheight = new JLabel("Height�G");
		height = new JTextField("256" ,5);
		JButton b = new JButton("Synthesis");
		b.addActionListener(this);
		/*�[�Jpanel*/
		panel.add(swidth);
		panel.add(width);
		panel.add(sheight);
		panel.add(height);
		panel.add(b);
		
		/*�]�wLayout*/
		GridLayout layout = new GridLayout(3 ,2);
		panel.setLayout(layout);
		return panel;
	}
	public void reSet() {
		if ( simg != null) {
		
			tp1.remove(simg);
			tp1.remove(tilec);
			tp1.remove(resultset);
			tp1.repaint();
			tp.validate();
		}
	}
	@Override
	public void actionPerformed(ActionEvent e) {
		Class c = e.getSource().getClass();
		String cname = c.getName();
		if ( cname == "javax.swing.JMenuItem" ) {
			try {
				Method m = c.getMethod("getText", null );
				String text = m.invoke(e.getSource(), null ).toString();
				System.out.println(text+"\n");
				//�uŪ����
				FileNameExtensionFilter filter = 
					new FileNameExtensionFilter("JPG & GIF Images", "jpg", "gif");
				
				JFileChooser fc = new JFileChooser( defaultdir );
				fc.setFileFilter( filter);
				
				if ( text == "read texture") {
					fc.showOpenDialog( this );
					File f = fc.getSelectedFile();
					wt = new WTGraphCut(f);
					
					srinfo.setImage( wt.getSourceImage() );
					reSet();
					validate();//layout������
				}
				else if ( text == "write texture") {
					
					fc.showSaveDialog( this );
					File f = fc.getSelectedFile();
					String [] sep = f.getName().split("[.]");
					
					int w = Integer.parseInt( width.getText() );
					int h = Integer.parseInt( height.getText() );
					BufferedImage result = wt.getSynthesisResult(w ,h);
					if ( sep.length == 1 ) 
						ImageIO.write(result, "jpeg", f);
					else ImageIO.write(result, sep[1], f);
				}
				
				repaint();
				
			}catch (NoSuchMethodException ne) {
				System.out.println("�S�����禡");
			}
			catch (NullPointerException pe) {
				System.out.println("���V�Ū���");
			}catch (Exception ex) {
				
			}
			
		}
		else if ( cname == "javax.swing.JButton" ) {//Synthesis Button click
			int w = Integer.parseInt( width.getText() );
			int h = Integer.parseInt( height.getText() );
			if ( wt.synthesis(w ,h ) ) {
				tp.setEnabledAt(1, true);
				JCheckBox cxb = new JCheckBox("�O�_��ܹ�");
				cxb.addItemListener(this);
				tp2.removeAll();
				tp2.add( cxb );
				tp2.add( wt.drawSynthesisTilesResult() ,1 );
				
//				wt.getSynthesisResult(w ,h);
			}
			else {
				JOptionPane.showMessageDialog(this, "�L�k�ͦ��v�� �·н��p���t�ζ}�o�̩Φۦ�ק�{�� ����" 
						,"�t�ο��~" ,JOptionPane.WARNING_MESSAGE);
			}
		}
		
		
	}

	@Override
	public void itemStateChanged(ItemEvent e) {
		tp2.remove(1);
		if ( e.getStateChange() == ItemEvent.SELECTED ) {
			CPanel canvas = new CPanel("Synthesis tile imgs" ,wt.getSynthesisTilesImgs());
			canvas.setCol( wt.getSynTileSetDim().width );
			tp2.add( canvas ,1);
			validate();	
			tp2.repaint();
		}
		else if ( e.getStateChange() == ItemEvent.DESELECTED ) {
			tp2.add( wt.drawSynthesisTilesResult() ,1 );
			validate();
			tp2.repaint();
		}
	}

	

}
