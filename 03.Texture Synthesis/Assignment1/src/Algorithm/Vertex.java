/**
 * PURPOSE
      作為儲存Graph的Vertex用 
 * @author peter
 */
package Algorithm;

public class Vertex {
	static public enum PART {SOURCE ,SINK ,UNKNOWN};
	boolean marked;//是否被選為當Augmented path上的點
	PART part = PART.UNKNOWN;
	Vertex parent = null;//從sink走至source用
	/**
	 * Constructor
	 */
	public Vertex() {
		
	}
	public Vertex( Vertex v ) {
		this.marked = v.marked;
		this.parent = v.parent;
		this.part = v.part;
	}
	/**
	 * 取得此Node為哪一個部分 是屬於Source Sink 還是目前還不知道
	 * @return
	 */
	public PART getWhatPart() {
		return part;
	}
	
}


