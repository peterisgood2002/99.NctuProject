/**
 * PURPOSE
      �@���x�sGraph��Vertex�� 
 * @author peter
 */
package Algorithm;

public class Vertex {
	static public enum PART {SOURCE ,SINK ,UNKNOWN};
	boolean marked;//�O�_�Q�אּ��Augmented path�W���I
	PART part = PART.UNKNOWN;
	Vertex parent = null;//�qsink����source��
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
	 * ���o��Node�����@�ӳ��� �O�ݩ�Source Sink �٬O�ثe�٤����D
	 * @return
	 */
	public PART getWhatPart() {
		return part;
	}
	
}


