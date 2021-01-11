/**
 * PURPOSE
      This class represents the pixel in a image 
      Because it will be the subclass of  the  Vertex Class in min cut Algorithm
      ,it'll run min cut algorithm
 * @author peter
 */
package WangTile;

public class Pixel extends Algorithm.Vertex {
	public int x;
	public int y;
	public Pixel() {
		
	}
	public Pixel( int x , int y ) {
		super();
		
		setPixel( x ,y );
	}
	/**
	 * 設定Pixel的位置
	 * @param x
	 * @param y
	 */
	public void setPixel(int x ,int y ) {
		this.x = x;
		this.y = y;
	}
	/**
	 * test if this is a vertex of ( x ,y ) 
	 * @param x
	 * @param y
	 * @return if (x ,y) ==( this.x ,this.y ) return true
	 */
	public boolean isPixel(int x ,int y ) {
		return ( this.x == x && this.y == y )? true : false;
	}
	
}
