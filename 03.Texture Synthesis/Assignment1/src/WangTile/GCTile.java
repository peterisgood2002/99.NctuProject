/**
 * PURPOSE
      This class is a tile for wang tile texture synthesis using graph cut 
 * @author peter
 */

package WangTile;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.util.Date;
import java.util.Random;
import java.util.Vector;

import Algorithm.Graphic;
import Algorithm.MCutMFlowFVersion;
import Algorithm.MCutMFlowGraphic;
import Algorithm.Vertex;
import Algorithm.Vertex.PART;


public class GCTile implements Tile{
	private BufferedImage tileimg;
	private BufferedImage stitchimg = null;
	private int[] nesw;//北東南西
	/**
	 * Constructor 給定image和北東南西的顏色值 產生一個小小的tile
	 * @param img 圖檔
	 * @param nesw 分別儲存北東西南的color資訊
	 */
	public GCTile(BufferedImage img , int[] nesw ) {
		tileimg = img;
		this.nesw = nesw;
	}
	/**
	 * 根據<code>nesw</code>的順序將samples資料產生Tiles
	 * @param samples
	 * @param nesw
	 */
	public GCTile(BufferedImage[] samples ,int[] nesw ) {
		produceTile(samples ,nesw);
		this.nesw = nesw;
	}
	/**
	 * 根據<code>nesw</code>產生一個tile img
	 * 而此img大小為samples當中代表n + w的大小 而排列方式為下面
	 *        W     N
	 *        S     E
	 * 將他逆時針轉45就是Cohen的基本tile格式
	 * @param samples 要拿幾個sample's image
	 * @param nesw順序
	 */
	public void produceTile(BufferedImage[] samples ,int[] nesw) {
		//初始化tileimg
		int w = samples[0].getWidth() + samples[3].getWidth();
		int h = samples[0].getHeight() + samples[3].getHeight();
		tileimg = new BufferedImage(w ,h ,samples[0].getType() );
		
		/*將相關資料填入wt中*/
		int cx = tileimg.getWidth() / 2;
		int cy = tileimg.getHeight() / 2;
		setTileImg( samples[ nesw[0] ] ,cx ,0);
		setTileImg( samples[ nesw[1] ] ,cx ,cy );
		setTileImg( samples[ nesw[2] ] ,0 ,cy );
		setTileImg( samples[ nesw[3] ] ,0 ,0 );
	}
	/*
	 * 相對於位置(sx ,sy)來設定Tile
	 */
	private void setTileImg(BufferedImage sample ,int sx ,int sy) {
		if ( tileimg != null ) {
			for ( int i = 0; i < sample.getWidth(); i++ ) {
				for ( int j = 0; j < sample.getHeight(); j++ ) {
					//取出sample的color來
					int color = sample.getRGB(i, j);
					tileimg.setRGB(sx + i, sy + j, color);
				}
			}
		}
		else throw new NullPointerException();
	}
	
	public boolean isMatch( Tile tn ,Tile tw) {
		GCTile ttn = (GCTile)(tn);
		GCTile ttw = (GCTile)(tw);
		if ( nesw[1] == ttn.nesw[1] && nesw[2] == ttw.nesw[2])
			return true;
		return false;
	}
	
	public BufferedImage getImage() {
		return tileimg;
	}
	@Override
	public BufferedImage getImage(int i) {
		int x = 0 , y = 0;
		int h = tileimg.getHeight() / 2 , w = tileimg.getWidth() / 2;
		switch (i) {
			case 0:
				x = w;
				break;
			case 1:
				x = w;
				y = h;
				break;
			case 2:
				y = h;
				break;
			default:
				break;
		}
		 
		return tileimg.getSubimage(x, y, w, h);
		
	}
	@Override
	public BufferedImage getStitchImg( int d ) throws NullPointerException{
		if (stitchimg == null )
			throw new NullPointerException("Stitch img is null. plz execute stitch() first");
		int halfw = stitchimg.getWidth() / 2;
		int halfh = stitchimg.getHeight() / 2;
		
		switch (d) {
			case 0: 
				return stitchimg.getSubimage(halfw, 0, halfw, halfh);
			case 1:
				return stitchimg.getSubimage(halfw, halfh, halfw, halfh);
			case 2:
				return stitchimg.getSubimage(0 ,halfh, halfw, halfh);
			case 3:
				return stitchimg.getSubimage(0, 0, halfw, halfh);
			default:
				return stitchimg;
		}
		
		
	}
	@Override
	public int getImageHeight() {
		return tileimg.getHeight();
	}
	@Override
	public int getImageWidth() {
		return tileimg.getWidth();
	}
	@Override
	public int[] getNESW() {
		return nesw;
	}
	@Override
	public BufferedImage stitch(BufferedImage sample) {
		if (stitchimg != null ) return stitchimg;
		// 將會用Graph cut切東西
		try {
			int w = tileimg.getWidth();
			int h = tileimg.getHeight();
			//先隨機選幾塊 patch中error最小的和Tile image大小一般的sample image
			BufferedImage img = errorMinG(sample ,w ,h);
			
			/*
			 * 只取Tile以及img的中間進行Graph cut即可
			 * 並且因為Vertex會紀錄cut的位置並對每一個Pixel做好標記
			 * 因此我們只要將Min cut的Vertex前n個拿來把在新的圖片上即可
			 */
			Vector <Vertex> v = produceVertexNode();
			
			/*根據img產生Graphic以方便做min cut*/
			MCutMFlowGraphic min = produceGraphic(v, img);
			min.run();
			min.calCut();
			
			/*畫出結果*/
			stitchimg = new BufferedImage(w ,h ,img.getType() );
			BufferedImage tmp = new BufferedImage(w ,h ,img.getType() );//Debug用
			for ( int i = 0; i < w; i++ )
				for ( int j = 0; j < h; j++ ) {
					int color = tileimg.getRGB(i, j);
					stitchimg.setRGB(i, j, color);
					tmp.setRGB(i, j, 0);
				}
			for ( Vertex node : v ) {
				Pixel p = (Pixel) node;
				if (node.getWhatPart() == PART.SINK ) {
					int color = img.getRGB(p.x, p.y);
					stitchimg.setRGB(p.x, p.y, color);
					tmp.setRGB(p.x, p.y, color);
				}else {
					int color = tileimg.getRGB(p.x, p.y);
					stitchimg.setRGB(p.x, p.y, color);
				}
			}
			return tmp;
			
			
		}catch (NullPointerException e){
			e.printStackTrace();
			System.out.println("Tile set使用Graph cut Algorithm 但是並沒有sample image好讓他cut 請注意\n");
			return null;
		}
		
	}
	
	/*
	 * random choose an image with minmum error
	 * @param sample
	 * @param w 要取的sample寬度
	 * @param h 要取的sample高度
	 * @return
	 */
	private BufferedImage errorMinG(BufferedImage sample ,int w ,int h) {
		if ( w == sample.getWidth() && h == sample.getHeight() )
			return sample;
		
		double errorcost = Double.MAX_VALUE;
//		int rand =( sample.getWidth() / w )* ( sample.getHeight() / h ) * 2;
		
		int x = 0;
		int y = 0;
		for ( int tx = 0; tx + w <= sample.getWidth(); tx += w ) {
			for ( int ty = 0; ty + h <= sample.getHeight(); ty+=h ) {
				double error = 0;
				//取得error
				for ( int j = 0; j < tileimg.getWidth();j++) {
					for (int k = 0; k < tileimg.getHeight(); k++ ) {
						
						Color color = new Color( sample.getRGB(tx+j, ty+k) );
						Color ref = new Color( tileimg.getRGB(j, k) );
						double er = color.getRed() - ref.getRed();
						double eg = color.getGreen() - ref.getGreen();
						double eb = color.getBlue() - ref.getBlue();
						error += Math.sqrt( er * er + eb * eb + eg * eg );
					}
				}
				//比較
				if ( error < errorcost ) {
					x = tx;
					y = ty;
					errorcost = error;
				}
			}
		}
//		for ( int i = 0; i < rand; i++) {
		
//			Date d = new Date();
//			Random r = new Random( d.getTime() );
//			int tx = r.nextInt( sample.getWidth() );
//			int ty = r.nextInt( sample.getWidth() );
//			while( tx + w >= sample.getWidth() ) tx = r.nextInt( sample.getWidth() );
//			while( ty + h >= sample.getHeight() ) ty = r.nextInt( sample.getHeight() );
//			double error = 0;
//			//取得error
//			for ( int j = 0; j < tileimg.getWidth();j++) {
//				for (int k = 0; k < tileimg.getHeight(); k++ ) {
//					Color color = new Color( sample.getRGB(tx+j, ty+k) );
//					Color ref = new Color( tileimg.getRGB(j, k) );
//					double er = color.getRed() - ref.getRed();
//					double eg = color.getGreen() - ref.getGreen();
//					double eb = color.getBlue() - ref.getBlue();
//					error += Math.sqrt( er * er + eb * eb + eg * eg );
//				}
//			}
//			//比較
//			if ( error < errorcost ) {
//				x = tx;
//				y = ty;
//				errorcost = error;
//			}
//		}
		return sample.getSubimage( x, y, w, h );
	}
	/*
	 * 根據Tile和Sample間部份 取pixel值
	 * 請注意根據Tile大小 其實中間要做Graph cut的部分為[(w/2+1) * h/2)/2] * 4個Pixel
			   XXXOOO
			   XXXOOO  一共有[(w/2+1) * h/2)/2] * 4加入戰局
			   XXXOOO
			   AAABBB
			   AAABBB
			   AAABBB
	 * source 和destination位置分別在第一個和最後一個位置上
	 * 以方便Edge的連結
	 */
	private Vector<Vertex> produceVertexNode() {
		int w = tileimg.getWidth();
		int h = tileimg.getHeight();
	
//		int stile = ((w/2+1) * h/2) /2 * 4;
		Vector<Vertex> v = new Vector<Vertex>();
		v.add( new Pixel() );
		
		//設定Pixel壓
		int yup = h / 2 - 1;
		int ydown = h / 2;
		for ( int x = 0; x < w; x++ ) {
			//對每一行進行處理
			if ( x < w / 2) {
				//將圖的上線以及下線設定好給Pixel
				for ( int y = yup; y <= ydown; y++ )
					v.add( new Pixel(x ,y));
				yup--;
				ydown++;	
			}
			else {
				yup++;
				ydown--;
				//將圖的上線以及下線設定好給Pixel
				for ( int y = yup; y <= ydown; y++ )
					v.add( new Pixel(x ,y));
			}
			
		}
		
		v.add( new Pixel() );
		return v;
	}
	
	/*
	 * 產生圖壓 快結束了 撐住
	 * boundary的部分會有一條線連到Source node 其他都連到sink
	 */
	private MCutMFlowGraphic produceGraphic(Vector<Vertex> v , BufferedImage img ) {
		MCutMFlowGraphic g = new MCutMFlowFVersion(v , 0 ,v.size() - 1);
//		MCutMFlowGraphic g = new MCutMFlowGraphic(v , 0 ,v.size() - 1);
		/*
		   			XO  
			   	   XXOO   
			   	  XXXOOO
			   	  AAABBB
			   	   AABB 
			   	    AB   
			將這些存成一個1-D array並做處理
		 */
		int w = tileimg.getWidth();
		int h = tileimg.getHeight();
		int yup = h / 2 - 1;
		int ydown = h / 2;
		
		for ( int x = 0; x < w; x++ ) {
			if ( x < w / 2) {
				for ( int y = yup; y <= ydown; y++ ){
					//取要處理之Pixel
					int i = getPixelV(v ,x , y );
					Pixel s = (Pixel) v.get(i);
//					g.setEdge(v.get(0), s, Double.MAX_VALUE );
//					g.setEdge(s ,v.get( v.size()-1 ) , Double.MAX_VALUE);
					
					
					
					if ( y == yup || y == ydown ) {
						//處理邊界 連到source
						g.setEdge(v.get(0), s, Double.MAX_VALUE );	
					}
					
					//處理內部連接處  先接到sink 讓他一定要被取代掉
					if ( x != 0 && ( x == w / 2 - 1 || y == w / 2 - 1 || y == w / 2) ) {
						if ( y != yup && y != ydown) 
							g.setEdge(s ,v.get( v.size()-1 ) , Double.MAX_VALUE);
					}
					/*
					 * 接四邊  主要都是接下面和右邊 除非沒辦法接下面和右邊了
					 */
					int li = ydown - yup;//此Node距離left node的index
					int ri = ydown - yup + 2;
						
						if ( x == w / 2 - 1 ) ri--;//如果在中間 則他的右邊index要另外算
					
						if (y < w / 2 )
							g = addLURDEdge(g ,img ,i ,i - li , i + ri ,2 );
						else g = addLURDEdge(g ,img ,i ,i - li , i + ri ,3 );
					
				}
				yup--;
				ydown++;
				
			}else {
				yup++;
				ydown--;
				//右半
				for ( int y = yup; y <= ydown; y++ ){
					if( x == 20 && y == 27 ) {
						int i = 0;
						i++;
					}
					//取要處理之Pixel
					int i = getPixelV(v ,x , y );
					Pixel s = (Pixel) v.get(i);
//					g.setEdge(v.get(0), s, Double.MAX_VALUE );
//					g.setEdge(s ,v.get( v.size()-1 ) , Double.MAX_VALUE);
					
					
					
					if ( y == yup || y == ydown ) {
						//處理邊界 連到source
						g.setEdge(v.get(0), s, Double.MAX_VALUE );
						
					}
					
					if ( x != w - 1 && ( x == w / 2  || y == w / 2 - 1 || y == w / 2 ) ) {
						if ( y != yup && y != ydown ) 
							g.setEdge(s ,v.get( v.size()-1 ) , Double.MAX_VALUE);
					}
					/*
					 * 接四邊 
					 */
					int ri = ydown - yup;//此Node距離left node的index
					int li = ydown - yup + 2;
					if ( x == w / 2 ) li--;//如果在中間 則他的右邊index要另外算
		
					if (y < w / 2 )
						g = addLURDEdge(g ,img ,i ,i - li , i + ri ,1);	
					else g = addLURDEdge(g ,img ,i ,i - li , i + ri ,4);
					
				}
				
				
			}
		}
	
		return g;
	}
	/**
	 * 根據<code>i</code>在g中加入Edge 而這些Edge分別是和i的四周相連
	 * 為了防止雙向連接要做超久 我們使用一個小技巧
	 * 以中央部分做四象限
	 * 					|
	 * 					|
	 * 			-->		|	    <--
	 * 			|		|         |
	 * 			v		|		  v
	 *  --------------------------------->
	 * 			^		|         ^
	 * 			|		|		  |
	 * 			|		|		  |
	 * 			-->		|		<--
	 * 					|
	 * @param g
	 * @param i pixel所在的Vertex位置
	 * @param li 左邊pixel在Vertex的位置
	 * @param ri 右邊pixel的Vertex的位置
	 * @param quadrant 依據圖示連接
	 * @return
	 */
	private MCutMFlowGraphic addLURDEdge(MCutMFlowGraphic g ,BufferedImage img ,int i ,int li ,int ri ,int quadrant){
		Vector<Vertex> v = g.getVertex();
		Pixel s = (Pixel) v.get(i);
		//取上下左右的pixel產生Cost
		Pixel t = null;
		double cost = 0;
//		
//		t = (Pixel) v.get( i+1 );
//		cost = calCost(s , t, img);
//		g.setEdge(s , t, cost);
//		//右
//		t = (Pixel) v.get( ri );
//		cost = calCost(s , t, img);
//		g.setEdge(s , t, cost);
		if ( quadrant <= 2 ) {
			//下
			if ( s.y != img.getHeight() / 2 - 1 ) {
//				System.out.println( s.x +" "+s.y);
//				if ( s.x == 31 && s.y == 63) {
//					System.out.println( s.x +" "+s.y);
//				}
				t = (Pixel) v.get( i+1 );
				cost = calCost(s , t, img);
				g.setEdge(s , t, cost);
			}

			switch (quadrant) {
				case 1:
					//右
					if ( s.x ==img.getWidth() / 2 - 1 ) break;
					t = (Pixel) v.get( ri );
					cost = calCost(s , t, img);
					g.setEdge(s , t, cost);
					break;
				case 2:
					//左
					t = (Pixel) v.get( li );
					cost = calCost(s , t, img);
					g.setEdge(s , t, cost);
					break;
				
			}
		}else {
			//上
			if ( s.y != img.getHeight() / 2  ){
				t = (Pixel) v.get( i-1 );
				cost = calCost(s , t, img);
				g.setEdge(s , t, cost);
			}
			switch (quadrant) {
				case 3:
					//右
					if ( s.x ==img.getWidth() / 2 - 1 ) break;
					t = (Pixel) v.get( ri );
					cost = calCost(s , t, img);
					g.setEdge(s , t, cost);
					break;
				case 4:
					//左
					t = (Pixel) v.get( li );
					cost = calCost(s , t, img);
					g.setEdge(s , t, cost);
					break;
			
			}
		}
		
		
		return g;
	}
	/*
	 * 從<code>v</code>中  以<code>begin</code>為起點至終點<code>end</code>搜尋(x ,y) 的index 
	 * @param v
	 * @param begin 搜尋起點
	 * @param end 要搜尋的中點
	 * @param x
	 * @param y
	 * @return
	 */
	private int getPixelV( Vector<Vertex> v ,int x ,int y) {
		for ( int i = 0; i < v.size(); i++  ) {
			Pixel p = (Pixel) v.get(i);
			if ( p.isPixel( x ,y ) ) return i;
		}
		
		return -1;
	}
	/*
	 * 計算 <code>s</code>到<code>t</code>的Cost為何 計算公式為
	 * | T(s) - S(s) | + | T(t) - S(t) |  / GT(s) + GT(t) + GS(s) + GS(t)
	 * 其中T(s)表示 s點在<code>tileimg</code>上的RGB值 S(s)為<code>sample</code> RGB值 
	 * @param s
	 * @param t
	 * @param sample 如果要對sample image計算cost 請在此加入sample image
	 * @return
	 */
	private double calCost(Pixel s ,Pixel t ,BufferedImage img ) {
		double cost = 0;
//		int Ts = tileimg.getRGB(s.x, s.y) ;
//		int Tt =  tileimg.getRGB(t.x, t.y);
//		int Ss = img.getRGB(s.x, s.y) ;
//		int St = img.getRGB(t.x, t.y) ;
//		cost = Math.abs(Ts - Ss)+Math.abs(Tt - St);
//		return cost;
		
		Color Ts = new Color( tileimg.getRGB(s.x, s.y) );
		Color Tt = new Color( tileimg.getRGB(t.x, t.y) );
		Color Ss = new Color( img.getRGB(s.x, s.y) );
		Color St = new Color( img.getRGB(t.x, t.y) );
		int[] dif = new int[3];
		dif[0] = Ts.getRed() -Ss.getRed();
		dif[1] = Ts.getGreen() - Ss.getGreen();
		dif[2] = Ts.getBlue() - Ss.getBlue();
		cost += Math.sqrt( dif[0] * dif[0] + dif[1] * dif[1] + dif[2] * dif[2] );
		dif[0] = Tt.getRed() -St.getRed();
		dif[1] = Tt.getGreen() - St.getGreen();
		dif[2] = Tt.getBlue() - St.getBlue();
		cost += Math.sqrt( dif[0] * dif[0] + dif[1] * dif[1] + dif[2] * dif[2] );
		
		//除以Gradient中
		double[] v = new double[2];
		v[0] = t.x - s.x;
		v[1] = t.y - s.y;
		double length = Math.sqrt( v[0] * v[0] + v[1] * v[1] );
		v[0] /= length;
		v[1] /= length;
		int x = s.x + (int) v[0];
		int y = s.y + (int) v[1];
		//GT(s)
		Color tmp = new Color( tileimg.getRGB(x, y) );
		dif[0] = tmp.getRed() -Ts.getRed();
		dif[1] = tmp.getGreen() - Ts.getGreen();
		dif[2] = tmp.getBlue() - Ts.getBlue();
		double deno = Math.sqrt( dif[0] * dif[0] + dif[1] * dif[1] + dif[2] * dif[2] );
		//GS(s)
		tmp = new Color( img.getRGB(x, y) );
		dif[0] = tmp.getRed() -Ss.getRed();
		dif[1] = tmp.getGreen() - Ss.getGreen();
		dif[2] = tmp.getBlue() - Ss.getBlue();
		deno += Math.sqrt( dif[0] * dif[0] + dif[1] * dif[1] + dif[2] * dif[2] );
		//GT(t)
		x = t.x - (int) v[0];
		y = t.y - (int) v[1];
		tmp = new Color( tileimg.getRGB(x, y) );
		dif[0] = tmp.getRed() -Tt.getRed();
		dif[1] = tmp.getGreen() - Tt.getGreen();
		dif[2] = tmp.getBlue() - Ts.getBlue();
		deno += Math.sqrt( dif[0] * dif[0] + dif[1] * dif[1] + dif[2] * dif[2] );
		//GS(t)
		tmp = new Color( img.getRGB(x, y) );
		dif[0] = tmp.getRed() -St.getRed();
		dif[1] = tmp.getGreen() - St.getGreen();
		dif[2] = tmp.getBlue() - Ss.getBlue();
		deno += Math.sqrt( dif[0] * dif[0] + dif[1] * dif[1] + dif[2] * dif[2] );
		return cost /= deno;
		
	}
	
}
