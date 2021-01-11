/**
 * PURPOSE
      This class is wang tile texture synthesis using graph cut
 * @author peter
 */
package WangTile;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.awt.image.RasterFormatException;
import java.io.File;
import java.io.IOException;
import java.lang.annotation.ElementType;
import java.util.Date;
import java.util.Iterator;
import java.util.Random;
import java.util.TreeSet;
import java.util.Vector;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.LineBorder;

import UI.Application;
import UI.CPanel;
import WangTile.ElementInfo.DIRECTION;

public class WTGraphCut implements WangTiler {

	private BufferedImage img;
	
	/*Constructors*/
	public WTGraphCut ( String fname) { 
		setSourceImage( fname );
		init();
	}
	public WTGraphCut( File f) {
		try {
			img = ImageIO.read(f);
			init();
		}catch (IOException e) {
			System.out.println( e.toString()); 
		}
	}
	
	public BufferedImage getSourceImage() {
		return img;
	}
	public boolean setSourceImage(String fname) {
		try {
			img = ImageIO.read( new File(fname));
			return true;
		}catch (IOException e) {
			System.out.println( e.toString()); 
			return false;
		}
	}

	private BufferedImage[] samples;
	@Override
	public boolean hasSamples() {
		if ( samples != null ) return true;
		else return false;
	}
	@Override
	public boolean pickSamples(int num, int width, int height) {
		// TODO ��num ��random�I �åB�H�����_�I��width ~height���v����samples
		
		try {
			samples = new BufferedImage[num];
		
			Date d = new Date();
			Random r = new Random( d.getTime() );
			for ( int i = 0; i < samples.length; i++ ) {
				int x = r.nextInt( img.getWidth() );
				int y = r.nextInt( img.getHeight() );
				//�v���n�d�Ŷ� �n���M�|�X���ݬ�x y�O�_���W�L
				while( x + width >= img.getWidth() ) x = r.nextInt( img.getWidth() );
				while( y + height >= img.getHeight() ) y = r.nextInt( img.getHeight() );
				
				samples[i] = img.getSubimage(x, y, width, height);
			}
			return true;
		}catch ( RasterFormatException re ) {
			System.out.println("��sample����");
			samples = null;
			
			return false;
		}
	}
	
	@Override
	public BufferedImage[] getSampleImage() {
		return samples;
	}
	Tile[][][] tiles;
	@Override
	public boolean hasTiles() {
		if ( tiles != null)
			return true;
		else return false;
	}
	@Override
	public Tile[][][] getTiles() {
		if ( hasTiles() )
			return tiles;
		else {
			produceTiles();
			return tiles;
		}
	}
	@Override
	public void produceTiles() {
		//��l��Tile
		tiles = new GCTile[samples.length][samples.length][2];
		Date d = new Date();
		Random r = new Random( d.getTime() );
		
		
		//���ͤG��dimension��tiles
		/*��C��tiles��줧img���o�˪� x y��}�l��random��X ��Ө̧ǲ���
		 *          j  
	     *       -------
	     *    i |      |
	     *      |      |   x
	     *      ---------
	     *          y
		 */
		for ( int k = 0; k < 2; k++ ) {
			//���ͤG��dimension��tiles
			int x = r.nextInt( samples.length );
			int y = r.nextInt( samples.length );
			for ( int i = 0; i < samples.length; i++ ) {
				for ( int j = 0; j < samples.length; j++) {
					int [] nesw = {j ,y ,x ,i};
					tiles[i][j][k] = new GCTile(samples ,nesw);
				}
			}
		}
		
	}
	@Override
	public BufferedImage[] getTileImgs() {
		if ( tiles == null ) produceTiles();
		BufferedImage[] timg = new BufferedImage[samples.length * samples.length * 2];
		int i = 0;
		for ( int x = 0; x < samples.length; x++ ) 
			for ( int y = 0; y < samples.length; y++ )
				for ( int z = 0; z < 2; z++ )
					timg[ i++ ] = tiles[x][y][z].getImage();
			
		return timg;
	}
	/**
	 * Initial some empty data
	 */
	private void init() {
		samples = null;
		tiles = null;
		seset = null;
		
	}
	/*
	 * �H�U�N�i��X��������
	 */
	private TreeSet <ElementInfo>[][] seset;//�X����element set ���i��@��element�O�Ѧh��Tile�M�w��
	private int sew;//�X����element set�e��
	private int seh;//�X����element set����
	
	@Override
	public boolean synthesis(int width ,int height) {
		try {
			if ( tiles == null) produceTiles();
			
//			//�M�w�u����tile set�j���n�h�j
//			double tmp = Math.sqrt( width * width + height * height );
//			sew = (int) Math.ceil( tmp / ( samples[0].getWidth() ) /2) * 2; //�H�@��Tile����
//			seh = (int) Math.ceil( tmp / ( samples[0].getWidth() ) /2) * 2;
			
			sew = (int) Math.ceil( width  / ( samples[0].getWidth() ) ) + 2; //�H�@��Tile����
			seh = (int) Math.ceil( height / ( samples[0].getWidth() ) ) + 2;
			if ( sew % 2 == 1 ) sew++;//��Element�O���ƪ�
			if ( seh % 2 == 1 ) seh++;
			seset = new TreeSet [seh][sew];
			/*��l��Element Matrix*/
			for ( int i = 0; i < seh; i++ ) 
				for ( int j = 0; j < sew; j++ ) {
					seset[i][j] = new TreeSet <ElementInfo>();
				}
			
			//�����1 ,2��ƪ�����椸
			Date d = new Date();
			Random r = new Random( d.getTime() );
			int row = 0;//��ܭ��@�C����椸
			for ( int col = 0; col < sew; col+= 2) {
				int x = r.nextInt(samples.length);
				int y = r.nextInt(samples.length);
				int z = r.nextInt(2);
				Tile t = tiles[x][y][z];
				t.stitch(img);
				seset[row][col].add( new ElementInfo(t , ElementInfo.DIRECTION.WEST) ); 
				seset[row+1][col].add( new ElementInfo(t , ElementInfo.DIRECTION.SOUTH) );
				seset[row][col+1].add( new ElementInfo(t , ElementInfo.DIRECTION.NORTH) );
				seset[row+1][col+1].add( new ElementInfo(t , ElementInfo.DIRECTION.EAST) );
			}
			/*��ƨ�Ƥ@�� ��X�ѤU������椸 ��M�覡�O�ͦ��覡�Ьݻ����άO��v��
			 * ���]�n�X�����I��
			 *  1  2  3  4   5   6
			 *  7  8  9  10  11  12
			 *  ���X��2 3 4 5
			 *  �A�X�� 1 7 8 ,6 11 12
			 *  �~�򩹫�X��   9 10
			 */
			int [] nesw = null;//���@�U�|�Ψ� tile��
			for ( row = 2; row < seh; row+=2 ) {
				//���X��2 3 4 5
				int x = 0;
				int y = 0;
				int z = 0;
				for ( int col = 1; col < sew - 1; col+=2 ) {
					x = seset[row-1][col].first().getDirection();
					nesw  = seset[row-1][col].last().tile.getNESW();
					x = nesw[x];
					y = seset[row-1][col+1].first().getDirection();
					nesw = seset[row-1][col+1].last().tile.getNESW();
					y = nesw[y];
					
					z = r.nextInt(2);
					Tile t = tiles[x][y][z];
					t.stitch(img);
					seset[row][col].add(  new ElementInfo(t ,ElementInfo.DIRECTION.SOUTH) );
					seset[row][col+1].add( new ElementInfo(t ,ElementInfo.DIRECTION.EAST) );
					seset[row-1][col].add( new ElementInfo(t , ElementInfo.DIRECTION.WEST) );
					seset[row-1][col+1].add( new ElementInfo(t , ElementInfo.DIRECTION.NORTH) );
					
				}
				
				//�A�X�� 1 7 8 ,6 11 12
				int col = 0;
				y = seset[row][col+1].last().getDirection();
				nesw = seset[row][col+1].last().tile.getNESW();
				y = nesw[y];
				x = r.nextInt(samples.length);
				z = r.nextInt(2);
				Tile t = tiles[x][y][z];
				t.stitch(img);
				seset[row][col].add( new ElementInfo(t ,ElementInfo.DIRECTION.WEST ) );
				seset[row+1][col].add( new ElementInfo(t ,ElementInfo.DIRECTION.SOUTH ) );
				seset[row+1][col+1].add( new ElementInfo(t ,ElementInfo.DIRECTION.EAST ) );
				seset[row][col+1].add( new ElementInfo(t ,ElementInfo.DIRECTION.NORTH ) );
				
				col = sew - 1;
				x = seset[row][col-1].last().getDirection();
				nesw = seset[row][col-1].last().tile.getNESW();
				x = nesw[x];
				y = r.nextInt(samples.length);
				z = r.nextInt(2);
				t = tiles[x][y][z];
				t.stitch(img);
				seset[row][col].add( new ElementInfo(t ,ElementInfo.DIRECTION.NORTH ) );
				seset[row+1][col].add( new ElementInfo(t ,ElementInfo.DIRECTION.EAST ) );
				seset[row+1][col-1].add( new ElementInfo(t ,ElementInfo.DIRECTION.SOUTH ) );
				seset[row][col-1].add( new ElementInfo(t ,ElementInfo.DIRECTION.WEST ) );
				
				//�~�򩹫�X��   9 10
				for ( col = 2 ; col < sew - 2; col+=2) {
					x = seset[row][col].last().getDirection();
					nesw = seset[row][col].last().tile.getNESW();
					x = nesw[x];
					y = seset[row][col+1].last().getDirection();
					nesw = seset[row][col+1].last().tile.getNESW();
					y = nesw[y];
					z = r.nextInt(2);
					t = tiles[x][y][z];
					t.stitch(img);
					seset[row+1][col].add( new ElementInfo(t ,ElementInfo.DIRECTION.SOUTH) );
					seset[row+1][col+1].add( new ElementInfo(t ,ElementInfo.DIRECTION.EAST) );
					seset[row][col].add( new ElementInfo(t ,ElementInfo.DIRECTION.WEST) );
					seset[row][col+1].add( new ElementInfo(t ,ElementInfo.DIRECTION.NORTH) );
				}
				
			}
			
//			//�H����@���I�b���W��
//			Date d = new Date();
//			Random r = new Random( d.getTime() );
//			int x = r.nextInt(samples.length);
//			int y = r.nextInt(samples.length);
//			int z = r.nextInt(2);
//			stileset[0][0] = tiles[x][y][z];
//		
//			//���ͦ���ɽu
//			for ( int i = 1; i < stw; i++ ) {
//				int[] nesw = stileset[0][i-1].getNESW();
//				//���e�@�Ӫ��F���@x ��L���Random
//				x = nesw[1];
//				y = r.nextInt(samples.length);
//				z = r.nextInt(2);
//				stileset[0][i] = tiles[x][y][z];
//			}
//			for ( int i = 1; i < sth; i++ ) {
//				int[] nesw = stileset[i-1][0].getNESW();
//				//���e�@�Ӫ��_���@y ��L���Random
//				y = nesw[2];
//				x = r.nextInt(samples.length);
//				z = r.nextInt(2);
//				stileset[i][0] = tiles[x][y][z];
//			}
//			
//			//���ͥ���
//			for ( int i = 1; i < sth; i++ ){
//				for ( int j = 1; j < stw; j++ ){
//					//���W�����n���@y �����䪺�F���@x
//					int[] nesw = stileset[i-1][j].getNESW();
//					y = nesw[2];
//					nesw = stileset[i][j-1].getNESW();
//					x = nesw[1];
//					z = r.nextInt(2);
//					stileset[i][j] = tiles[x][y][z];
//				}
//			}
			
			return true;
		}catch (Exception e) {
			e.printStackTrace();
			System.out.println( e.toString() );
			return false;
		}
	}
	@Override
	public Dimension getSynTileSetDim() {
		return new Dimension(sew ,seh);
	}
	@Override
	public BufferedImage[] getSynthesisTilesImgs() {
		
		BufferedImage[] stimgs = new BufferedImage[(sew) * (seh)];
		
		int index = 0;
		
		for (int i = 0; i < seh; i++ ) {
			for ( int j = 0; j < sew; j++ ) {
//				System.out.println(i+" "+ j);
				TreeSet <ElementInfo> element = seset[i][j];
				stimgs[index++] = produceElementImage(element);
				
			}
		}
		return stimgs;
	}
	private BufferedImage produceElementImage(TreeSet <ElementInfo> element ) {
		
		if ( element.size() == 1 ) {
			ElementInfo e = element.first();
			return e.getStitchImage();
//			return e.getImage();
		}
		else {
			
			BufferedImage eimg = new BufferedImage( samples[0].getWidth() ,samples[0].getHeight() ,img.getType() );
			
			// TODO �NElement �Ϥ��ͦ��X��
			Iterator<ElementInfo> eiter = element.iterator();
			while ( eiter.hasNext() ) {
				ElementInfo e = eiter.next();
				BufferedImage tmp = e.getStitchImage();
//				BufferedImage tmp = e.getImage();
				
				int inity = 0 ,iy = 1 ,finishy = tmp.getHeight();//y���d��
				switch ( e.d ){
					case NORTH://��tmp���U������W�h
						inity = 0; 
						iy = 1;
						finishy = tmp.getHeight();
						break;
					case EAST://��tmp���W������W�h
						inity = 0; finishy = tmp.getHeight(); 
						iy = -1;
						break;
					case SOUTH://��tmp�k�W������W�h
						inity = 0; 
						finishy = 1; 
						iy = 1;
						break;
					case WEST://��tmp�k�U������W�h
						inity = tmp.getHeight() - 1;
						finishy = tmp.getHeight(); 
						iy = -1;
						break;
				}
				
				for( int i = 0; i < tmp.getWidth(); i++) {
					for(int j = inity; j < finishy; j++) {
						int color = tmp.getRGB(i, j);
						eimg.setRGB(i, j, color);
					}
					
					if ( e.d == DIRECTION.NORTH || e.d == DIRECTION.WEST)
						inity += iy;
					else finishy += iy;
				}
				
//				try {
//					ImageIO.write(tmp, "jpeg", new File("debug.jpg"));
//				} catch (IOException ne) {
//					// TODO Auto-generated catch block
//					ne.printStackTrace();
//				}
			}
			
			
			return eimg;
		}
		
	}
	@Override
	public JPanel drawSynthesisTilesResult() {
		JPanel panel = new JPanel();
		panel.setLayout( new GridLayout(sew/2 ,seh/2) );
		
		for ( int i = 0; i < seh; i+=2 )
			for ( int j = 0; j < sew; j+=2 ) {
				JPanel p = getTileInfoPanel(seset[i][j].last().tile);
				panel.add(p);
			}
		
		return panel;
	}
	/**
	 * ���oTile��������T�noutput
	 * @param t
	 * @return
	 */
	private JPanel getTileInfoPanel(Tile t) {
		JPanel panel = new JPanel();
		
		int[] nesw = t.getNESW(); 
		panel.setLayout(new GridLayout(2,2) );
		JLabel[] neswlabel = new JLabel[ nesw.length ];
		//��icon
		String workingdir = System.getProperty("user.dir");
		ImageIcon slash = new ImageIcon( workingdir +"\\Icon\\slash.jpg" );
		ImageIcon backslash = new ImageIcon(workingdir +"\\Icon\\backslash.jpg");
		
		for ( int i = 0; i < neswlabel.length; i++ ) {
			neswlabel[i] = new JLabel( Integer.toString(nesw[i]) );
			neswlabel[i].setBorder( new LineBorder(Color.BLACK) );
			neswlabel[i].setVerticalTextPosition(JLabel.BOTTOM);
			neswlabel[i].setHorizontalTextPosition(JLabel.CENTER);

		}
		//�]�j�p
//		neswlabel[0].setPreferredSize( new Dimension(15, 12) );
//		neswlabel[2].setPreferredSize( new Dimension(5, 2) );
//		neswlabel[1].setPreferredSize( new Dimension(2 ,5) );
//		neswlabel[3].setPreferredSize( new Dimension(2 ,5) ) ;
		
		neswlabel[0].setIcon(backslash);
		neswlabel[1].setIcon(backslash);
		neswlabel[2].setIcon(slash);
		neswlabel[3].setIcon(slash);
		
		panel.add( neswlabel[3] );
		panel.add( neswlabel[0] );
		panel.add( neswlabel[1] );
		panel.add( neswlabel[2] );
		panel.setPreferredSize( new Dimension(100 ,130 ) );
		return panel;
	}
	@Override
	public BufferedImage getSynthesisResult(int w ,int h) {
		if ( seset == null ) synthesis( w ,h );//�p�G�٨S�}�l�ͦ��@�л��֥ͦ�����
		
		//�ھ�Tile info���ͤ@�i�j��
		int iw = samples[0].getWidth();
		int ih = samples[0].getHeight();
		BufferedImage rtileimg = new BufferedImage(iw * sew ,ih * seh ,img.getType() );
		for ( int i = 0; i < seh; i++ ) {
			for ( int j = 0; j < sew; j++ ) {
				BufferedImage tmp = produceElementImage(seset[i][j]);
				 
				int ry = i * tmp.getHeight();
				for ( int y = 0; y < tmp.getHeight(); y++ ) {
					int rx = j * tmp.getWidth();
					for ( int x = 0; x < tmp.getWidth(); x++ ) {
						int color = tmp.getRGB(x, y);
						rtileimg.setRGB(rx, ry, color);
						rx++;
					}
					ry++;
						
				}
			}
		}
		
		//���ͯu�������ɥX��
		
		//Output�٧Ϊ��ݬ�
//		BufferedImage result = new BufferedImage(rtileimg.getWidth() ,rtileimg.getHeight() ,img.getType());
//		
//		int yup = rtileimg.getHeight() / 2 - 1;
//		int ydown = rtileimg.getHeight() / 2;
//		for ( int x = 0; x < result.getWidth(); x++ ) {
//			//��C�@��i��B�z
//			if ( x < result.getWidth() / 2) {
//				//�N�Ϫ��W�u�H�ΤU�u�]�w�n��Pixel
//				for ( int y = yup; y <= ydown; y++ ) {
//					int color = rtileimg.getRGB(x, y);
//					result.setRGB(x, y, color);
//				}
//					
//				yup--;
//				ydown++;	
//			}
//			else {
//				yup++;
//				ydown--;
//				//�N�Ϫ��W�u�H�ΤU�u�]�w�n��Pixel
//				for ( int y = yup; y <= ydown; y++ ) {
//					int color = rtileimg.getRGB(x, y);
//					result.setRGB(x, y, color);
//				}
//			}
//			
//		}
		return rtileimg.getSubimage(iw, ih, w, h);
//		return rtileimg;
	}
	
	
}
/**
 * ��ܤ@�ӦX���ɳ���椸��������T �ΨӶi��X����
 * �����D�n����ӵ��c ��ܦX�����@��²�檺����L�O�q���@��Tile��nesw���@�ӳ�����
 * @author peter
 *
 */ 

class ElementInfo implements Comparable<ElementInfo>{
	static enum DIRECTION {NORTH ,EAST ,SOUTH ,WEST };//��ܤ�V��
	Tile tile;//�ѴX��Tile�զ� �o�@��element info����T
	DIRECTION d;//��Tile�����@�ӳ�����data
	
	/**
	 * Constructor
	 */
	public ElementInfo (Tile t , DIRECTION d) {
		tile = t;
		this.d = d;
	}
	/**
	 * �qDirection�M�w�n��nesw���@�ӭ�
	 * @return
	 */
	public int getDirection() {
		switch (d) {
			case NORTH:
				return 0;
			case EAST:
				return 1;
			case SOUTH:
				return 2;
			case WEST:
				return 3;
			default:
				return -1;
		}
		
	}
	/**
	 * get the tile stitch images of this element
	 * @return
	 */
	public BufferedImage getStitchImage() {
		return tile.getStitchImg( getDirection() );
	}
	/**
	 * get the tile orginal image of this element
	 * @return
	 */
	public BufferedImage getImage() {
		return tile.getImage( getDirection() );
	}
	@Override
	public int compareTo(ElementInfo o) {
		if (tile == o.tile && d == o.d )
			return 0;//�O�Ʊ椣�n���ƨ�
		return 1;//���u�n�۵���N�n �ںޥ��j�p�O���
	}
	
	@Override
	public boolean equals(Object obj) {
		try {
			ElementInfo o = ( ElementInfo )obj;
			return (tile == o.tile && d == o.d)? true :false;
		}catch (Exception e) {
			return super.equals(obj);
		}
		
	}
}

