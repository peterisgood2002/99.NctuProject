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
import java.util.Date;
import java.util.Random;
import java.util.Vector;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.LineBorder;

import UI.Application;
import UI.CPanel;

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
		// TODO 取num 個random點 並且以此為起點找width ~height的影像為samples
		
		try {
			samples = new BufferedImage[num];
		
			Date d = new Date();
			Random r = new Random( d.getTime() );
			for ( int i = 0; i < samples.length; i++ ) {
				int x = r.nextInt( img.getWidth() );
				int y = r.nextInt( img.getHeight() );
				//影像要留空間 要不然會出錯看看x y是否有超過
				while( x + width >= img.getWidth() ) x = r.nextInt( img.getWidth() );
				while( y + height >= img.getHeight() ) y = r.nextInt( img.getHeight() );
				
				samples[i] = img.getSubimage(x, y, width, height);
			}
			return true;
		}catch ( RasterFormatException re ) {
			System.out.println("取sample失敗");
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
		//初始化Tile
		tiles = new GCTile[samples.length][samples.length][2];
		Date d = new Date();
		Random r = new Random( d.getTime() );
		
		
		//產生二個dimension的tiles
		/*其每個tiles對到之img為這樣的 x y剛開始為random選出 後來依序產生
		 *          j  
	     *       -------
	     *    i |      |
	     *      |      |   x
	     *      ---------
	     *          y
		 */
		for ( int k = 0; k < 2; k++ ) {
			//產生二個dimension的tiles
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
	 * 以下將進行合成的部分
	 */
	private ElementInfo[][] seset;//合成的element set 
	private int sew;//合成的element set寬度
	private int seh;//合成的element set高度
	
	@Override
	public boolean synthesis(int width ,int height) {
		try {
			if ( tiles == null) produceTiles();
			
//			//決定真正的tile set大概要多大
//			double tmp = Math.sqrt( width * width + height * height );
//			sew = (int) Math.ceil( tmp / ( samples[0].getWidth() ) /2) * 2; //以一個Tile為準
//			seh = (int) Math.ceil( tmp / ( samples[0].getWidth() ) /2) * 2;
			
			sew = (int) Math.ceil( width  / ( samples[0].getWidth() ) ) + 2; //以一個Tile為準
			seh = (int) Math.ceil( height / ( samples[0].getWidth() ) ) + 2;
			seset = new ElementInfo[seh][sew];
			//先找到1 ,2兩排的單體單元
			Date d = new Date();
			Random r = new Random( d.getTime() );
			int row = 0;//表示哪一列單體單元
			for ( int col = 0; col < sew; col+= 2) {
				int x = r.nextInt(samples.length);
				int y = r.nextInt(samples.length);
				int z = r.nextInt(2);
				Tile t = tiles[x][y][z];
				t.stitch(img);
				seset[row][col] = new ElementInfo(t , ElementInfo.DIRECTION.WEST);
				seset[row+1][col] = new ElementInfo(t , ElementInfo.DIRECTION.SOUTH);
				seset[row][col+1] = new ElementInfo(t , ElementInfo.DIRECTION.NORTH);
				seset[row+1][col+1] = new ElementInfo(t , ElementInfo.DIRECTION.EAST);
			}
			/*兩排兩排一組 找出剩下的單體單元 找尋方式是生成方式請看說明或是投影片
			 * 假設要合成的點有
			 *  1  2  3  4   5   6
			 *  7  8  9  10  11  12
			 *  先合成2 3 4 5
			 *  再合成 1 7 8 ,6 11 12
			 *  繼續往後合成   9 10
			 */
			for ( row = 2; row < seh; row+=2 ) {
				//先合成2 3 4 5
				int x = 0;
				int y = 0;
				int z = 0;
				for ( int col = 1; col < sew - 1; col+=2 ) {
					x = seset[row-1][col].getDirection();
					y = seset[row-1][col+1].getDirection();
					z = r.nextInt(2);
					Tile t = tiles[x][y][z];
					t.stitch(img);
					seset[row][col] = new ElementInfo(t ,ElementInfo.DIRECTION.SOUTH);
					seset[row][col+1] = new ElementInfo(t ,ElementInfo.DIRECTION.EAST);
				}
				
				//再合成 1 7 8 ,6 11 12
				int col = 0;
				y = seset[row][col+1].getDirection();
				x = r.nextInt(samples.length);
				z = r.nextInt(2);
				Tile t = tiles[x][y][z];
				t.stitch(img);
				seset[row][col] = new ElementInfo(t ,ElementInfo.DIRECTION.WEST );
				seset[row+1][col] = new ElementInfo(t ,ElementInfo.DIRECTION.SOUTH );
				seset[row+1][col+1] = new ElementInfo(t ,ElementInfo.DIRECTION.EAST );
				
				col = sew - 1;
				x = seset[row][col-1].getDirection();
				y = r.nextInt(samples.length);
				z = r.nextInt(2);
				t = tiles[x][y][z];
				t.stitch(img);
				seset[row][col] = new ElementInfo(t ,ElementInfo.DIRECTION.NORTH );
				seset[row+1][col] = new ElementInfo(t ,ElementInfo.DIRECTION.EAST );
				seset[row+1][col-1] = new ElementInfo(t ,ElementInfo.DIRECTION.SOUTH );
				
				//繼續往後合成   9 10
				for ( col = 2 ; col < sew - 2; col+=2) {
					x = seset[row][col].getDirection();
					y = seset[row][col+1].getDirection();
					z = r.nextInt(2);
					t = tiles[x][y][z];
					t.stitch(img);
					seset[row+1][col] = new ElementInfo(t ,ElementInfo.DIRECTION.SOUTH);
					seset[row+1][col+1] = new ElementInfo(t ,ElementInfo.DIRECTION.EAST);
				}
				
			}
			
//			//隨機找一個點在左上角
//			Date d = new Date();
//			Random r = new Random( d.getTime() );
//			int x = r.nextInt(samples.length);
//			int y = r.nextInt(samples.length);
//			int z = r.nextInt(2);
//			stileset[0][0] = tiles[x][y][z];
//		
//			//先生成邊界線
//			for ( int i = 1; i < stw; i++ ) {
//				int[] nesw = stileset[0][i-1].getNESW();
//				//取前一個的東邊當作x 其他兩個Random
//				x = nesw[1];
//				y = r.nextInt(samples.length);
//				z = r.nextInt(2);
//				stileset[0][i] = tiles[x][y][z];
//			}
//			for ( int i = 1; i < sth; i++ ) {
//				int[] nesw = stileset[i-1][0].getNESW();
//				//取前一個的北邊當作y 其他兩個Random
//				y = nesw[2];
//				x = r.nextInt(samples.length);
//				z = r.nextInt(2);
//				stileset[i][0] = tiles[x][y][z];
//			}
//			
//			//產生全部
//			for ( int i = 1; i < sth; i++ ){
//				for ( int j = 1; j < stw; j++ ){
//					//取上面的南邊當作y 取左邊的東邊當作x
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
				Tile t = seset[i][j].tile;
				stimgs[index++] = t.getStitchImg( seset[i][j].getDirection() ); 
			}
		}
		return stimgs;
	}
	@Override
	public JPanel drawSynthesisTilesResult() {
		JPanel panel = new JPanel();
		panel.setLayout( new GridLayout(sew/2 ,seh/2) );
		
		for ( int i = 0; i < seh; i+=2 )
			for ( int j = 0; j < sew; j+=2 ) {
				JPanel p = getTileInfoPanel(seset[i][j].tile);
				panel.add(p);
			}
		
		return panel;
	}
	/**
	 * 取得Tile的相關資訊好output
	 * @param t
	 * @return
	 */
	private JPanel getTileInfoPanel(Tile t) {
		JPanel panel = new JPanel();
		
		int[] nesw = t.getNESW(); 
		panel.setLayout(new GridLayout(2,2) );
		JLabel[] neswlabel = new JLabel[ nesw.length ];
		//找icon
		String workingdir = System.getProperty("user.dir");
		ImageIcon slash = new ImageIcon( workingdir +"\\Icon\\slash.jpg" );
		ImageIcon backslash = new ImageIcon(workingdir +"\\Icon\\backslash.jpg");
		
		for ( int i = 0; i < neswlabel.length; i++ ) {
			neswlabel[i] = new JLabel( Integer.toString(nesw[i]) );
			neswlabel[i].setBorder( new LineBorder(Color.BLACK) );
			neswlabel[i].setVerticalTextPosition(JLabel.BOTTOM);
			neswlabel[i].setHorizontalTextPosition(JLabel.CENTER);

		}
		//設大小
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
		if ( seset == null ) synthesis( w ,h );//如果還沒開始生成　請趕快生成謝謝
		
		//根據Tile info產生一張大圖
		int iw = seset[0][0].tile.getImageWidth();
		int ih = seset[0][0].tile.getImageHeight();
		BufferedImage rtileimg = new BufferedImage(iw * sew ,ih * seh ,img.getType() );
		for ( int i = 0; i < seh; i++ ) {
			for ( int j = 0; j < sew; j++ ) {
				Tile t = seset[i][j].tile;
				int d = seset[i][j].getDirection();
				BufferedImage tmp = null;
				try {
					tmp = t.getStitchImg(d);//取得 Stitch 過後的image
				}catch ( NullPointerException ne ) {
					ne.printStackTrace();
					System.out.println("請先Stitch tile");
				}
				 
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
		
		//產生真正的圖檔出來
		
		//Output菱形的看看
//		BufferedImage result = new BufferedImage(rtileimg.getWidth() ,rtileimg.getHeight() ,img.getType());
//		
//		int yup = rtileimg.getHeight() / 2 - 1;
//		int ydown = rtileimg.getHeight() / 2;
//		for ( int x = 0; x < result.getWidth(); x++ ) {
//			//對每一行進行處理
//			if ( x < result.getWidth() / 2) {
//				//將圖的上線以及下線設定好給Pixel
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
//				//將圖的上線以及下線設定好給Pixel
//				for ( int y = yup; y <= ydown; y++ ) {
//					int color = rtileimg.getRGB(x, y);
//					result.setRGB(x, y, color);
//				}
//			}
//			
//		}
		return rtileimg.getSubimage(iw, ih, w, h);
	}
	
	
}
/**
 * 表示一個合成時單體單元的內部資訊 用來進行合成用
 * 內部主要有兩個結構 表示合成中一個簡單的單體他是從哪一個Tile的nesw哪一個部分來
 * @author peter
 *
 */ 
class ElementInfo {
	static enum DIRECTION {NORTH ,EAST ,SOUTH ,WEST };//表示方向性
	Vector<Tile> tile;//由幾塊Tile組成 這一塊element info的資訊
	DIRECTION d;
	
	/**
	 * Constructor
	 */
	public ElementInfo (Tile t , DIRECTION d) {
		if ( !tile.contains(t) ) 
			tile.add(t);
		this.d = d;
	}
	/**
	 * 從Direction決定要取nesw哪一個值
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
	 * get the element info's images
	 * @return
	 */
	public BufferedImage getImage() {
		return tile.getImage( getDirection() );
	}
}
