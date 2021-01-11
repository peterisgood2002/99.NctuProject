/**
 * PURPOSE
      This class is interface for wang tile synthesis
      �]���٧Ϋ����i����� �]���ڭ̨ϥΤ@�ذ����٧� �]�N�O�ڭ̰��]���٧Ϊ��s�b
      �ԽнШ�����
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
	 * �|���ͤ@��JPanel���x�ssynthesis��Tile ���G
	 * @return
	 */
	public JPanel drawSynthesisTilesResult();
	/**
	 * �ھ�<code>w</code>�M<code>h</code>���X img���o�̫᪺�v�����
	 * @param w �̫�img�e��
	 * @param h �̫�img����
	 * @return
	 */
	public BufferedImage getSynthesisResult(int w ,int h);
	
	
}
