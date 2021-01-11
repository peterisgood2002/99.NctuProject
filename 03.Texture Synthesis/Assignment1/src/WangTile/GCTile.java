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
	private int[] nesw;//�_�F�n��
	/**
	 * Constructor ���wimage�M�_�F�n�誺�C��� ���ͤ@�Ӥp�p��tile
	 * @param img ����
	 * @param nesw ���O�x�s�_�F��n��color��T
	 */
	public GCTile(BufferedImage img , int[] nesw ) {
		tileimg = img;
		this.nesw = nesw;
	}
	/**
	 * �ھ�<code>nesw</code>�����ǱNsamples��Ʋ���Tiles
	 * @param samples
	 * @param nesw
	 */
	public GCTile(BufferedImage[] samples ,int[] nesw ) {
		produceTile(samples ,nesw);
		this.nesw = nesw;
	}
	/**
	 * �ھ�<code>nesw</code>���ͤ@��tile img
	 * �Ӧ�img�j�p��samples���N��n + w���j�p �ӱƦC�覡���U��
	 *        W     N
	 *        S     E
	 * �N�L�f�ɰw��45�N�OCohen����tile�榡
	 * @param samples �n���X��sample's image
	 * @param nesw����
	 */
	public void produceTile(BufferedImage[] samples ,int[] nesw) {
		//��l��tileimg
		int w = samples[0].getWidth() + samples[3].getWidth();
		int h = samples[0].getHeight() + samples[3].getHeight();
		tileimg = new BufferedImage(w ,h ,samples[0].getType() );
		
		/*�N������ƶ�Jwt��*/
		int cx = tileimg.getWidth() / 2;
		int cy = tileimg.getHeight() / 2;
		setTileImg( samples[ nesw[0] ] ,cx ,0);
		setTileImg( samples[ nesw[1] ] ,cx ,cy );
		setTileImg( samples[ nesw[2] ] ,0 ,cy );
		setTileImg( samples[ nesw[3] ] ,0 ,0 );
	}
	/*
	 * �۹���m(sx ,sy)�ӳ]�wTile
	 */
	private void setTileImg(BufferedImage sample ,int sx ,int sy) {
		if ( tileimg != null ) {
			for ( int i = 0; i < sample.getWidth(); i++ ) {
				for ( int j = 0; j < sample.getHeight(); j++ ) {
					//���Xsample��color��
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
		// �N�|��Graph cut���F��
		try {
			int w = tileimg.getWidth();
			int h = tileimg.getHeight();
			//���H����X�� patch��error�̤p���MTile image�j�p�@�몺sample image
			BufferedImage img = errorMinG(sample ,w ,h);
			
			/*
			 * �u��Tile�H��img�������i��Graph cut�Y�i
			 * �åB�]��Vertex�|����cut����m�ù�C�@��Pixel���n�аO
			 * �]���ڭ̥u�n�NMin cut��Vertex�en�Ӯ��ӧ�b�s���Ϥ��W�Y�i
			 */
			Vector <Vertex> v = produceVertexNode();
			
			/*�ھ�img����Graphic�H��K��min cut*/
			MCutMFlowGraphic min = produceGraphic(v, img);
			min.run();
			min.calCut();
			
			/*�e�X���G*/
			stitchimg = new BufferedImage(w ,h ,img.getType() );
			BufferedImage tmp = new BufferedImage(w ,h ,img.getType() );//Debug��
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
			System.out.println("Tile set�ϥ�Graph cut Algorithm ���O�èS��sample image�n���Lcut �Ъ`�N\n");
			return null;
		}
		
	}
	
	/*
	 * random choose an image with minmum error
	 * @param sample
	 * @param w �n����sample�e��
	 * @param h �n����sample����
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
				//���oerror
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
				//���
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
//			//���oerror
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
//			//���
//			if ( error < errorcost ) {
//				x = tx;
//				y = ty;
//				errorcost = error;
//			}
//		}
		return sample.getSubimage( x, y, w, h );
	}
	/*
	 * �ھ�Tile�MSample������ ��pixel��
	 * �Ъ`�N�ھ�Tile�j�p ��ꤤ���n��Graph cut��������[(w/2+1) * h/2)/2] * 4��Pixel
			   XXXOOO
			   XXXOOO  �@�@��[(w/2+1) * h/2)/2] * 4�[�J�ԧ�
			   XXXOOO
			   AAABBB
			   AAABBB
			   AAABBB
	 * source �Mdestination��m���O�b�Ĥ@�өM�̫�@�Ӧ�m�W
	 * �H��KEdge���s��
	 */
	private Vector<Vertex> produceVertexNode() {
		int w = tileimg.getWidth();
		int h = tileimg.getHeight();
	
//		int stile = ((w/2+1) * h/2) /2 * 4;
		Vector<Vertex> v = new Vector<Vertex>();
		v.add( new Pixel() );
		
		//�]�wPixel��
		int yup = h / 2 - 1;
		int ydown = h / 2;
		for ( int x = 0; x < w; x++ ) {
			//��C�@��i��B�z
			if ( x < w / 2) {
				//�N�Ϫ��W�u�H�ΤU�u�]�w�n��Pixel
				for ( int y = yup; y <= ydown; y++ )
					v.add( new Pixel(x ,y));
				yup--;
				ydown++;	
			}
			else {
				yup++;
				ydown--;
				//�N�Ϫ��W�u�H�ΤU�u�]�w�n��Pixel
				for ( int y = yup; y <= ydown; y++ )
					v.add( new Pixel(x ,y));
			}
			
		}
		
		v.add( new Pixel() );
		return v;
	}
	
	/*
	 * ���͹��� �ֵ����F ����
	 * boundary�������|���@���u�s��Source node ��L���s��sink
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
			�N�o�Ǧs���@��1-D array�ð��B�z
		 */
		int w = tileimg.getWidth();
		int h = tileimg.getHeight();
		int yup = h / 2 - 1;
		int ydown = h / 2;
		
		for ( int x = 0; x < w; x++ ) {
			if ( x < w / 2) {
				for ( int y = yup; y <= ydown; y++ ){
					//���n�B�z��Pixel
					int i = getPixelV(v ,x , y );
					Pixel s = (Pixel) v.get(i);
//					g.setEdge(v.get(0), s, Double.MAX_VALUE );
//					g.setEdge(s ,v.get( v.size()-1 ) , Double.MAX_VALUE);
					
					
					
					if ( y == yup || y == ydown ) {
						//�B�z��� �s��source
						g.setEdge(v.get(0), s, Double.MAX_VALUE );	
					}
					
					//�B�z�����s���B  ������sink ���L�@�w�n�Q���N��
					if ( x != 0 && ( x == w / 2 - 1 || y == w / 2 - 1 || y == w / 2) ) {
						if ( y != yup && y != ydown) 
							g.setEdge(s ,v.get( v.size()-1 ) , Double.MAX_VALUE);
					}
					/*
					 * ���|��  �D�n���O���U���M�k�� ���D�S��k���U���M�k��F
					 */
					int li = ydown - yup;//��Node�Z��left node��index
					int ri = ydown - yup + 2;
						
						if ( x == w / 2 - 1 ) ri--;//�p�G�b���� �h�L���k��index�n�t�~��
					
						if (y < w / 2 )
							g = addLURDEdge(g ,img ,i ,i - li , i + ri ,2 );
						else g = addLURDEdge(g ,img ,i ,i - li , i + ri ,3 );
					
				}
				yup--;
				ydown++;
				
			}else {
				yup++;
				ydown--;
				//�k�b
				for ( int y = yup; y <= ydown; y++ ){
					if( x == 20 && y == 27 ) {
						int i = 0;
						i++;
					}
					//���n�B�z��Pixel
					int i = getPixelV(v ,x , y );
					Pixel s = (Pixel) v.get(i);
//					g.setEdge(v.get(0), s, Double.MAX_VALUE );
//					g.setEdge(s ,v.get( v.size()-1 ) , Double.MAX_VALUE);
					
					
					
					if ( y == yup || y == ydown ) {
						//�B�z��� �s��source
						g.setEdge(v.get(0), s, Double.MAX_VALUE );
						
					}
					
					if ( x != w - 1 && ( x == w / 2  || y == w / 2 - 1 || y == w / 2 ) ) {
						if ( y != yup && y != ydown ) 
							g.setEdge(s ,v.get( v.size()-1 ) , Double.MAX_VALUE);
					}
					/*
					 * ���|�� 
					 */
					int ri = ydown - yup;//��Node�Z��left node��index
					int li = ydown - yup + 2;
					if ( x == w / 2 ) li--;//�p�G�b���� �h�L���k��index�n�t�~��
		
					if (y < w / 2 )
						g = addLURDEdge(g ,img ,i ,i - li , i + ri ,1);	
					else g = addLURDEdge(g ,img ,i ,i - li , i + ri ,4);
					
				}
				
				
			}
		}
	
		return g;
	}
	/**
	 * �ھ�<code>i</code>�bg���[�JEdge �ӳo��Edge���O�O�Mi���|�P�۳s
	 * ���F�������V�s���n���W�[ �ڭ̨ϥΤ@�Ӥp�ޥ�
	 * �H�����������|�H��
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
	 * @param i pixel�Ҧb��Vertex��m
	 * @param li ����pixel�bVertex����m
	 * @param ri �k��pixel��Vertex����m
	 * @param quadrant �̾ڹϥܳs��
	 * @return
	 */
	private MCutMFlowGraphic addLURDEdge(MCutMFlowGraphic g ,BufferedImage img ,int i ,int li ,int ri ,int quadrant){
		Vector<Vertex> v = g.getVertex();
		Pixel s = (Pixel) v.get(i);
		//���W�U���k��pixel����Cost
		Pixel t = null;
		double cost = 0;
//		
//		t = (Pixel) v.get( i+1 );
//		cost = calCost(s , t, img);
//		g.setEdge(s , t, cost);
//		//�k
//		t = (Pixel) v.get( ri );
//		cost = calCost(s , t, img);
//		g.setEdge(s , t, cost);
		if ( quadrant <= 2 ) {
			//�U
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
					//�k
					if ( s.x ==img.getWidth() / 2 - 1 ) break;
					t = (Pixel) v.get( ri );
					cost = calCost(s , t, img);
					g.setEdge(s , t, cost);
					break;
				case 2:
					//��
					t = (Pixel) v.get( li );
					cost = calCost(s , t, img);
					g.setEdge(s , t, cost);
					break;
				
			}
		}else {
			//�W
			if ( s.y != img.getHeight() / 2  ){
				t = (Pixel) v.get( i-1 );
				cost = calCost(s , t, img);
				g.setEdge(s , t, cost);
			}
			switch (quadrant) {
				case 3:
					//�k
					if ( s.x ==img.getWidth() / 2 - 1 ) break;
					t = (Pixel) v.get( ri );
					cost = calCost(s , t, img);
					g.setEdge(s , t, cost);
					break;
				case 4:
					//��
					t = (Pixel) v.get( li );
					cost = calCost(s , t, img);
					g.setEdge(s , t, cost);
					break;
			
			}
		}
		
		
		return g;
	}
	/*
	 * �q<code>v</code>��  �H<code>begin</code>���_�I�ܲ��I<code>end</code>�j�M(x ,y) ��index 
	 * @param v
	 * @param begin �j�M�_�I
	 * @param end �n�j�M�����I
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
	 * �p�� <code>s</code>��<code>t</code>��Cost���� �p�⤽����
	 * | T(s) - S(s) | + | T(t) - S(t) |  / GT(s) + GT(t) + GS(s) + GS(t)
	 * �䤤T(s)��� s�I�b<code>tileimg</code>�W��RGB�� S(s)��<code>sample</code> RGB�� 
	 * @param s
	 * @param t
	 * @param sample �p�G�n��sample image�p��cost �Цb���[�Jsample image
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
		
		//���HGradient��
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
