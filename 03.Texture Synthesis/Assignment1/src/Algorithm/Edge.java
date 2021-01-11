/**
 * PURPOSE
      代表圖的一個Edge 
 * @author peter
 */
package Algorithm;

public class Edge {
	double capacity;//edge最大的流量
	double flow;//目前流量
	Vertex u;// u ------>v
	Vertex v;
//	boolean inverseedgetopath = false;//對path而言這是inverse edge嗎
	/**
	 * Constructor 傳入此Edge的最大流量為何
	 * @param cap
	 */
	public Edge(Vertex s ,Vertex t ,double cap) {
		u = s;
		v = t;
		capacity = cap;
		flow = 0;
	}
	
	/**
	 * 加入流量
	 * @param f 要加入的流量
	 */
	public void addFlow(double f) {
		flow += f;
	}
	/**
	 * 流量是否滿了
	 * @return 如果滿了 回傳true
	 */
	public boolean isFull() {
		return (flow < capacity)? false : true;
	}
	/**
	 * 流量是否為空的
	 * @return
	 */
	public boolean isEmpty() {
		return ( flow == 0 )? true :false;
	}
	/**
	 * 有無流量
	 * @return
	 */
	public boolean hasFlow() {
		return ( flow > 0 )? true : false;
	}
	/**
	 * 取得flow值
	 * @return
	 */
	public double getFlow() {
		return flow;
	}
	/**
	 * 將流量盡空
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
