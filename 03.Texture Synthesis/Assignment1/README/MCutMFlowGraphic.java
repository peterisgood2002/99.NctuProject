/**
 * PURPOSE
      this is an instance of graphic
      他將計算maximum flow min cut
      1.Edge的儲存以adjacency matrix 
        所以理論上Edge的大小為vertex.length * vertex.length
        Edge的表示法應該是
	                25
	      1  ---------------> 3
	      第1個row中的第3個col的cost 為25
	 * 如果沒有Edge則可以為null
      2.使用Edmonds-Karp Algorithm
        詳請請見 http://en.wikipedia.org/wiki/Edmonds-Karp_algorithm
 * @author peter
 */
package Algorithm;

import java.util.Vector;
import Algorithm.Vertex.PART;


public class MCutMFlowGraphic implements Graphic{
	private Vector<Vertex> nodes;
	//總共會有兩種Edge分別是Vertex的inner edge和outer edge
	private Vector< Vector<Edge> > viedges;//Vertex的inner edge
	private Vector< Vector<Edge> > voedges;//outer edge
	private Vertex source;
	private Vertex sink;
	/**
	 * Constructor 
	 * Edge的表示法應該是
	 *               25
	 *     1  ---------------> 3
	 *     第1個row中的第3個col的cost 為25
	 * 如果沒有Edge則可以為null
	 * @param v 點的個數
	 * @param e 邊 因為邊太大 所以用Vector of Vector進行儲存
	 * @param s 第幾個點要當source
	 * @param sink 第幾個點要當sink
	 */
	public MCutMFlowGraphic(Vector<Vertex> v  ,int s ,int sink) {
		setVertex(v);
		setDefaultEdges( v.size() );
		setSource(s);
		setSink(sink);
	}
	
	@Override
	public void setVertex( Vector<Vertex> v) {
		nodes = v;
	}
	@Override
	public void setEdge( int u ,int v ,double cap ) {
		Edge e = new Edge( nodes.get(u) , nodes.get(v) ,cap);
		/* u -------->v */
		voedges.get(u).add(e);
		viedges.get(v).add(e);
	}
	/**
	 * 從Vertex中建立edge
	 * @param u
	 * @param v
	 * @param cap
	 */
	public void setEdge( Vertex u ,Vertex v ,double cap ) {
		int ui = getVIndex(u);
		int vi = getVIndex(v);
		setEdge(ui ,vi , cap );
	}
	/*Get attribute*/
	public Vector<Vertex> getVertex() {
		return nodes;
	}
	/*
	 * 建立一個2D Array 裡面都是空值
	 */
	private void setDefaultEdges( int vsize ) {
		 viedges = new Vector< Vector<Edge> >();
		 voedges = new Vector< Vector<Edge> >();
		 
		 for ( int i = 0; i < vsize; i++ ) {
			 viedges.add( new Vector<Edge>() );
			 voedges.add( new Vector<Edge>() );
		 }
	}
	public void setSource ( int s ) {
		source = nodes.get(s);
		source.part = PART.SOURCE;
	}
	public void setSink( int s ) {
		sink = nodes.get(s);
		sink.part = PART.SINK;
	}
	
	
	/**
	 * 讓資料結構歸零
	 */
	public void setDefault() {
		for (Vertex v : nodes) 
			v.marked = false;
		
		for ( Vector<Edge> ne : voedges ) {
			for ( Edge edge : ne ) {
				edge.emptyFlow();
			}
		}
		
		isrun = false;
	}
	/*
	 * 取得Vertex 他的index
	 * 傳入：Vertex
	 * 傳出：他在Array的index
	 */
	public int getVIndex(Vertex n ) {
		for ( int i = 0; i < nodes.size(); i++ )
			if (nodes.get(i) == n)
				return i;
		return -1;
	}
	
	boolean isrun = false;
	@Override
	public void run() {
		setDefault();
		
		double cap = findAugmented(source ,sink ,Double.MAX_VALUE);
		for ( Vector<Edge> e : voedges) {
			for (Edge edge : e ) {
				int u = getVIndex(edge.u);
				int v = getVIndex(edge.v);
				System.out.printf("edges[%d][%d] flow = %f\n", u ,v ,edge.getFlow());
			}
		}
		int index = 1;
		while( cap != 0 ) {
			//如果有從source出去還有edge 沒有滿 表示還可以填喔
			cap = findAugmented(source ,sink ,Double.MAX_VALUE);
			
			System.out.println("做了"+ ++index);
//			for ( Vector<Edge> e : voedges) {
//				for (Edge edge : e ) {
//					int u = getVIndex(edge.u);
//					int v = getVIndex(edge.v);
//					System.out.printf("edges[%d][%d] flow = %f\n", u ,v ,edge.getFlow());
//				}
//			}
			
		}
		
//		for ( Vector<Edge> e : voedges) {
//			for (Edge edge : e ) {
//				int u = getVIndex(edge.u);
//				int v = getVIndex(edge.v);
//				System.out.printf("edges[%d][%d] flow = %f\n", u ,v ,edge.getFlow());
//			}
//		}

		isrun = true;
	}
	//取得一個vertex的OuterEdge和InnerEdge
	private Vector<Edge> getOuterEdge( Vertex node ) {
		try {
			return voedges.get( getVIndex(node) );
		}catch (ArrayIndexOutOfBoundsException e) {
			return null;
		}
	}
	private Vector<Edge> getInnerEdge( Vertex node) {
		try {
			return viedges.get( getVIndex(node) );
		}catch (ArrayIndexOutOfBoundsException e) {
			return null;
		}
	}
	/*
	 * 此函式將會找到從<code>source</code>出去的一條Augmented Path<br />
	 * 並且回傳此Augmented Path最小的capacity
	 */
	private double findAugmented(Vertex source ,Vertex sink ,double mincap) {
		double cap = 0;
		
		if ( source == sink ) return mincap;
		
		source.marked = true;
		
		//找到所有 source -------> i 沒有滿的
		Vector<Edge> soe = getOuterEdge(source);
		for ( Edge edge : soe ) {
			if ( !edge.v.marked && !edge.isFull() && cap == 0) {
				cap = Math.min( mincap, edge.residualCapacity() );
				cap = findAugmented(edge.v ,sink , cap);
				if ( cap > 0 ) {//加入flow
					edge.addFlow(cap);
					System.out.println(getVIndex(source) +" " + getVIndex(edge.v) + " " + edge.getFlow() );
				}
			}
		}
		
		//如果正向找不到 可以找找反向的 找到所有i -------->source 沒有滿的
		if ( cap == 0 ) {
			Vector<Edge> sie = getInnerEdge(source);
			for ( Edge edge : sie ) {
				if ( !edge.u.marked && edge.hasFlow() && cap == 0) {
					cap = Math.min( mincap, edge.flow );
					cap = findAugmented(edge.u ,sink , cap);
					if ( cap > 0 ) {
						System.out.println(getVIndex(edge.u) +" " + getVIndex(source) + " " + edge.getFlow() );
						
						edge.addFlow( -cap );
					}
				}
			}
		}
		
		source.marked = false;
		return cap;
		
	}
	
	/**
	 * 找尋最短的Cut
	 * @return
	 */
	public void calCut() {
		if ( !isrun ) run();
		
		/*
		 * 從Source part中找尋所有Edge中沒滿的 滿了就當作算了
		 */
		Vector<Vertex> spart = new Vector<Vertex>();//儲存所有source端的node
		spart.add(source);
		while ( !spart.isEmpty() ) {
			Vertex u = spart.remove(0);
			u.part = PART.SOURCE;
			
			//找所有edge  u ------>v 中沒滿的加進去
			Vector<Edge> soe = getOuterEdge(u);
			for ( Edge edge: soe ) {
				Vertex v = edge.v;
				if ( !spart.contains(v) && ! edge.isFull() && v.part == PART.UNKNOWN )
					spart.add(v);
			}
		
			//找所有edge  v ------>u 中 空的加進來
			Vector<Edge> sie = getInnerEdge(u);
			for ( Edge edge: sie ) {
				Vertex v = edge.u;
				if ( !spart.contains(v) && !edge.isEmpty() 
						&& v.part == PART.UNKNOWN )
					spart.add(v);
				
			}

		}		
		
		//其他沒算到都歸到Sink去
		for ( int i = 0; i < nodes.size(); i++ ) 
			if ( nodes.get(i).part == PART.UNKNOWN ) 
				nodes.get(i).part = PART.SINK;
		
		
		for ( int i = 0; i < nodes.size(); i++ )
			System.out.println(i +" "+ nodes.get(i).part );
			
		
	}
}
