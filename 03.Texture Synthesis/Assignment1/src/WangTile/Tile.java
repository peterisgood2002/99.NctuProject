/**
 * PURPOSE
      This class is a tile for wang tile texture synthesis
 * @author peter
 */
package WangTile;

import java.awt.Color;
import java.awt.image.BufferedImage;

public interface Tile {
	/**
	 * 透過<code>nesw</code>的順序將sample's images產生Tile
	 * @param samples 
	 * @param nesw
	 */
    public abstract void produceTile(BufferedImage[] samples ,int[] nesw);
	/**
	 * 看看此Tile是否有和<code>ttn</code>,<code>ttw</code>是否有match
	 * @param t another tile
	 * @return
	 */
    public boolean isMatch( Tile ttn ,Tile ttw);
	/**
	 * 取得image
	 * @return
	 */
	public BufferedImage getImage();
	/**
	 * 取得某個方向的subsample
	 * @param i 要取的是哪一個方向 分別是N = 0 ,E = 1 ,S = 2 ,W = 3
	 * @return
	 */
	public BufferedImage getImage(int i);
	/**
	 *  取得stitch過後的img 
	 *  @param d 
	 *      d = 0為 right-top patch
	 *      d = 1為 right-down patch
	 *      d = 2為 left-down patch
	 *      d = 3為 left-top patch
	 *      other d 為 all img
	 * @return 回傳的img和原本的img共用
	 * @throws NullPointerException 如果stitch是空的 就會丟出這exception告知
	 */
	public BufferedImage getStitchImg( int d ) throws NullPointerException;
	
	/**
	 * 取得Tile image的寬度
	 * @return
	 */
	public int getImageWidth();
	/**
	 * 取得Tile image的高度
	 * @return
	 */
	public int getImageHeight();
	/**
	 * 取得Tiles的NESW資訊
	 * @return
	 */
	public int[] getNESW();

	/**
	 * 透過<code>sample</code>將Tile image 進行縫合
	 * 請先確認您的縫合演算法是否需要sample 不需要時請自行處理NULL Point的問題
	 * 為了方便 debug 會傳出GC 所切出的區塊出來
	 * @param sample
	 */
	public abstract BufferedImage stitch( BufferedImage sample );
	
	
	

}
