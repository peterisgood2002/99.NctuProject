/**
 * PURPOSE
      the <code>Graphic</code> class is an interface for the algorithm to run
      if there are other graphic's algorithm 
      ,they will need to implements this interface in order to provide consistency
 * @author peter
 */
package Algorithm;

import java.util.Vector;

public interface Graphic {
	/*Set attribute*/
	public void setVertex( Vector<Vertex> v);
	/**
	 * 用來設定Edge u----->v的cost
	 * @param u
	 * @param v
	 * @param cost
	 */
	public void setEdge( int u ,int v ,double cost );
	
	/**
	 * in order to run the graphics algorithm
	 */
	public void run();
	
}
