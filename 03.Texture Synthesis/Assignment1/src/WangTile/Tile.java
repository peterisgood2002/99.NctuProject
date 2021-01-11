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
	 * �z�L<code>nesw</code>�����ǱNsample's images����Tile
	 * @param samples 
	 * @param nesw
	 */
    public abstract void produceTile(BufferedImage[] samples ,int[] nesw);
	/**
	 * �ݬݦ�Tile�O�_���M<code>ttn</code>,<code>ttw</code>�O�_��match
	 * @param t another tile
	 * @return
	 */
    public boolean isMatch( Tile ttn ,Tile ttw);
	/**
	 * ���oimage
	 * @return
	 */
	public BufferedImage getImage();
	/**
	 * ���o�Y�Ӥ�V��subsample
	 * @param i �n�����O���@�Ӥ�V ���O�ON = 0 ,E = 1 ,S = 2 ,W = 3
	 * @return
	 */
	public BufferedImage getImage(int i);
	/**
	 *  ���ostitch�L�᪺img 
	 *  @param d 
	 *      d = 0�� right-top patch
	 *      d = 1�� right-down patch
	 *      d = 2�� left-down patch
	 *      d = 3�� left-top patch
	 *      other d �� all img
	 * @return �^�Ǫ�img�M�쥻��img�@��
	 * @throws NullPointerException �p�Gstitch�O�Ū� �N�|��X�oexception�i��
	 */
	public BufferedImage getStitchImg( int d ) throws NullPointerException;
	
	/**
	 * ���oTile image���e��
	 * @return
	 */
	public int getImageWidth();
	/**
	 * ���oTile image������
	 * @return
	 */
	public int getImageHeight();
	/**
	 * ���oTiles��NESW��T
	 * @return
	 */
	public int[] getNESW();

	/**
	 * �z�L<code>sample</code>�NTile image �i���_�X
	 * �Х��T�{�z���_�X�t��k�O�_�ݭnsample ���ݭn�ɽЦۦ�B�zNULL Point�����D
	 * ���F��K debug �|�ǥXGC �Ҥ��X���϶��X��
	 * @param sample
	 */
	public abstract BufferedImage stitch( BufferedImage sample );
	
	
	

}
