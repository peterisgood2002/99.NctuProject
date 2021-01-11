/**
 * PURPOSE
      This class is interface for wang tile synthesis
      因為菱形很難進行切割 因此我們使用一種假的菱形 也就是我們假設有菱形的存在
      詳請請見圖檔
 * @author peter
 */
package WangTile;

import java.awt.Dimension;
import java.awt.image.BufferedImage;

import javax.swing.JPanel;

public interface WangTiler {
	/**
	 * set the texture in order to do wang tile 
	 * @param fname the filename of the texture
	 * @return if set success return true
	 */
	public boolean setSourceImage(String fname);
	/**
	 * get the source image
	 * @return source image for the wang tile synthesis
	 */
	public BufferedImage getSourceImage();
	/**
	 * @return if there are samples return true
	 */
	public boolean hasSamples();
	/**
	 * pick <b>num</b> samples from original images
	 * @param num number of samples
	 * @param width sample's width
	 * @param height sample's height 
	 */
	public boolean pickSamples(int num ,int width ,int height);
	/**
	 * it 'll return the Sample image
	 * @return
	 */
	public BufferedImage[] getSampleImage();
	
	/**
	 * it'll return true if there are already some tiles
	 * @return
	 */
	public boolean hasTiles();
	/**
	 * get the tiles of the wang tile 
	 * it'll be a 3-d array. 
	 * In the row ,it'll store the same color of tiles in west.
	 * In the column ,it'll store the same color of tiles in north.
	 * In the third dimension ,it'll store all tiles which have the same color in west and the other in north
	 * @return if there is no tiles return null
	 */
	public Tile[][][] getTiles();
	/**
	 * it'll produce tile automaticaaly
	 */
	public void produceTiles();
	/**
	 * it'll return tile images
	 * @return tile images
	 */
	public BufferedImage[] getTileImgs();
	/**
	 * Synthesis images
	 * @param width the width of synthesis image
	 * @param height the height of synthesis image
	 * @return if synthesis sucess it'll return true
	 */
	public boolean synthesis(int width ,int height);
	/**
	 * get the dimension of synthesis tile set
	 * @return
	 */
	public Dimension getSynTileSetDim();
	/**
	 * get the tile images of synthesis result
	 * @return
	 */
	public BufferedImage[] getSynthesisTilesImgs();
	/**
	 * 會產生一個JPanel來儲存synthesis的Tile 結果
	 * @return
	 */
	public JPanel drawSynthesisTilesResult();
	/**
	 * 根據<code>w</code>和<code>h</code>切出 img取得最後的影像資料
	 * @param w 最後img寬度
	 * @param h 最後img高度
	 * @return
	 */
	public BufferedImage getSynthesisResult(int w ,int h);
	
	
}
