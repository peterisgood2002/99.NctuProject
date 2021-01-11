/**
 * PURPOSE
      �N��Ϫ��@��Edge 
 * @author peter
 */
package Algorithm;

public class Edge {
	double capacity;//edge�̤j���y�q
	double flow;//�ثe�y�q
	Vertex u;// u ------>v
	Vertex v;
//	boolean inverseedgetopath = false;//��path�Ө��o�Oinverse edge��
	/**
	 * Constructor �ǤJ��Edge���̤j�y�q����
	 * @param cap
	 */
	public Edge(Vertex s ,Vertex t ,double cap) {
		u = s;
		v = t;
		capacity = cap;
		flow = 0;
	}
	
	/**
	 * �[�J�y�q
	 * @param f �n�[�J���y�q
	 */
	public void addFlow(double f) {
		flow += f;
	}
	/**
	 * �y�q�O�_���F
	 * @return �p�G���F �^��true
	 */
	public boolean isFull() {
		return (flow < capacity)? false : true;
	}
	/**
	 * �y�q�O�_���Ū�
	 * @return
	 */
	public boolean isEmpty() {
		return ( flow == 0 )? true :false;
	}
	/**
	 * ���L�y�q
	 * @return
	 */
	public boolean hasFlow() {
		return ( flow > 0 )? true : false;
	}
	/**
	 * ���oflow��
	 * @return
	 */
	public double getFlow() {
		return flow;
	}
	/**
	 * �N�y�q�ɪ�
	 */
	public void emptyFlow() {
		flow = 0;
	}
	
	/**
	 * Compute the Residual capacity
	 * @return
	 */
	public double residualCapacity() {
		return capacity - flow;
	}

}
