/**
 * PURPOSE

   因為Edmonds-Karp Algorithm 太慢了 慢到哭餓
        因此我們使用
        "An Experimental Comparison of Min-Cut/Max-Flow Algorithms
		 for Energy Minimization in Vision."
		的Algorithm 
		最主要是它們透過把邊界的概念來進行尋找
		也就是說 
		  PPA
		  PPA
		s PPAA
		  PPPPA
		  PPAA 
		透過A來進行Edge連結 
		主要是屬於S的部分他往Outer edge長
		屬於T的部分他往Inner Edge方向長 
	2.Orphan再決定時則是透過相反的方式決定edge 決定他是否要變成Free node
 * @author peter
 */
package Algorithm;

import java.util.HashMap;
import java.util.Vector;
import Algorithm.Vertex.PART;

public class MCutMFlowFVersion extends MCutMFlowGraphic{

	public MCutMFlowFVersion(Vector<Vertex> v, int s, int sink) {
		super(v, s, sink);
	}
	
	@Override
	public void run() {
		//初始化資訊
		Vector<Vertex> active = new Vector<Vertex>();
		source.part = PART.SOURCE;
		sink.part = PART.SINK;
		active.add(source);
		active.add(sink);
		
		//開跑
		int index = 1;
		while ( true ) {
//			System.out.println(index++);
			
			
//			System.out.println("開始grow");
			Edge keyedge = grow( active );
			if (  keyedge == null ) break; 
//			System.out.println("開始Augment");
			Vector<Vertex> orphan = augment(keyedge);
//			System.out.println("開始adopt");
			active = adopt( active ,orphan);
			
		}
		
//		for ( HashMap< Integer ,Edge> e : voedges ) {
//			for (Edge edge : e.values() ) {
//				int u = getVIndex(edge.u);
//				int v = getVIndex(edge.v);
//				System.out.printf("edges[%d][%d] flow = %f\n", u ,v ,edge.getFlow());
//			}
//		}
	
	}

	/*
	 * 透過<code>active</code>長出一個augmented path並傳出path讓source長到sink 最小的capacity
	 * 傳出：最小的residual capacity的Edge
	 * @param active
	 * @return
	 */
	private Edge grow( Vector<Vertex> active) {
		
		while ( !active.isEmpty() ) {
			Vertex p = active.remove(0);
//			System.out.println("Grow");
			HashMap< Integer ,Edge> edge = null;
			//如果屬於SOURCE 則為out edge SINK則為inner edge
			if ( p.part == PART.SOURCE )
				edge = getOuterEdge(p);
			else if ( p.part == PART.SINK )
				edge = getInnerEdge(p);
			else continue;//不知道為什麼他會找到Unkown的 所以我當作沒看到^^
		
			for ( Edge e : edge.values() ) {
//				System.out.println("Grow2");
				if ( !e.isFull()  ) {
					Vertex unknown = null ,known = null;//已經知道以及不知道之Vertex屬於哪一個部分
					if ( p.part == PART.SOURCE ) {//表示e.u ----->e.v可以填
						known = e.u;
						unknown = e.v;
					}
					else if ( p.part == PART.SINK ) {//表示e.u ------>e.v可填
						unknown = e.u;
						known = e.v;
					}
					
					
					if ( unknown.part == PART.UNKNOWN  ) {
						unknown.parent = known;//設定unknown的parent和known的child
						unknown.part = known.part;//設定TREE(u) = TREE(v)
						if( !active.contains(unknown) ) 
							if ( !active.contains( unknown ) )
								active.add( unknown );
					}
					
					//交界處 讓關鍵的edge丟出去
					if ( unknown.part != PART.UNKNOWN && unknown.part != known.part) {
						return e;
					}
						
				}
			}
		}
		
		return null;
	}
	/*
	 * 做擴充的動作 填充那一條path 從該條關鍵edge往兩處進行填充 此Edge左邊一定是都是source部分 右邊一定都是sink部分
	 * @param edge 填充的量
	 * @return orphan資料結構
	 */
	private Vector<Vertex> augment( Edge edge) {
		Vector<Vertex> orphan = new Vector<Vertex>();
		double cap = computeCap( edge );
		
		//Key edge填充  左邊一定是都是source部分 右邊一定都是sink部分
		edge.addFlow(cap);
		
		//往source端去找 看看有沒有orphan 以及進行填充
		Vertex node = edge.u;
		while ( node != source ) {
//			System.out.println("Augment偵錯 --source");
			Edge e = null;
			Vertex nextnode = node.parent;//為了防止node變孤兒就完了
			try {
				e = getEdge(node.parent ,node);
				e.addFlow(cap);
			}catch (NullPointerException ne) {//此Edge為此path中的反向Edge
				e = getEdge(node ,node.parent);
				e.addFlow(-cap);
			}
			
			if (  e.isFull() ) {
				Vertex s = e.u ,t = e.v;//s ----->t
				if ( s.part == t.part ) {//讓他孤單
					if ( node.part == PART.SOURCE  ) {
							t.parent = null;
							orphan.add( t );
					}
				}
			}
			
			node = nextnode;
		}
		
		//往sink端找
		node = edge.v;
		while ( node != sink ) {
//			System.out.println("Augment偵錯--sink");
			Edge e = null;
			Vertex nextnode = node.parent;//為了防止node變孤兒就完了
			try {
				e = getEdge( node ,node.parent );
				e.addFlow(cap);
			}
			catch (NullPointerException ne) {
				e = getEdge( node.parent ,node);
				e.addFlow(-cap);
				
			}
			if (  e.isFull() ) {
				Vertex s = e.u ,t = e.v;//s ----->t
				if ( s.part == t.part ) {//讓他孤單
					if ( node.part == PART.SINK ) {
							s.parent = null;
							orphan.add( s );
						
					}
					
				}
			}

			node = nextnode;
		}
		return orphan;
	}
	/*
	 * 取得Edge <code>u</code>--------><code>v</code>
	 * @param u
	 * @param v
	 * @return
	 */
	private Edge getEdge(Vertex u ,Vertex v) throws NullPointerException{
		HashMap< Integer ,Edge> out = getOuterEdge(u);
		Edge e = out.get( getVIndex(v) );
		if ( e == null ) new NullPointerException("there are no edge in u ---->v\n");
		return out.get( getVIndex(v) );
	}
	/*
	 * 透過 e的兩端點 找出兩邊的residual capacity最小值
	 * @param e
	 * @return
	 */
	private double computeCap(Edge edge) {
		double cap = edge.residualCapacity();
		
		Vertex node = edge.u;//往source端找
		while ( node != source ) {
//			System.out.println("計算source端");
			Edge e = null;

			try {
				e = getEdge( node.parent ,node );
				cap = Math.min(cap, e.residualCapacity() );
			}catch (NullPointerException ne) {
				//表示u-->v沒有線 但是程式又說這是這條路徑中 反向的線 所以要回流壓
				e = getEdge( node ,node.parent );
			}finally {
				cap = Math.min(cap, e.residualCapacity() );
			}
			
			node = node.parent;
		}
		
		node = edge.v;//往sink端找
		while ( node != sink ) {
//			System.out.println("計算sink端");
			Edge e = null;
			
			try {
				e = getEdge( node ,node.parent );
				cap = Math.min(cap, e.residualCapacity() );
			}catch (NullPointerException ne) {
				//表示u-->v沒有線 但是程式又說這是這條路徑中 反向的線 所以要回流壓
				e = getEdge( node.parent ,node  );
			}finally {
				cap = Math.min(cap, e.residualCapacity() );
			}
			
			node = node.parent;
			
		}
		
//		System.out.println("cap = "+cap);
		return cap;
	}
	/*
	 * 進行adopt step
	 * Each node p being processed tries to find a new valid parent within the same search tree; 
	 * in case of success p remains in the tree but with a new parent, 
	 * otherwise it becomes a free node and all its children are added to O.
	 
	 * @param active
	 * @param orphan
	 * @return active 以方便後面實做
	 */
	private Vector<Vertex> adopt(Vector<Vertex> active, Vector<Vertex> orphan) {
		while ( !orphan.isEmpty() ) {
//			System.out.println("ADopt偵錯");
			Vertex p = orphan.remove(0);
			//想辦法找neighborhood看看能不能找到新的parent 
			
			/*
		     * A valid parent u should satisfy: 
			 * TREE(u) = TREE(v)
			 * cap(u -->v) > 0 TREE(v)= S u-->v TREE(v)= T v-->u
			 * the “origin” of u should be either source or sink.
			*/
			HashMap< Integer ,Edge> edges = null;
			if ( p.part == PART.SOURCE )
				edges = getInnerEdge(p);
			else if ( p.part == PART.SINK )
				edges = getOuterEdge(p);
			for ( Edge e : edges.values() ) {
				
				Vertex q = null;
				if ( e.u == p ) q = e.v;
				else if ( e.v == p ) q = e.u;
				
				Vertex s = origin(q);
				if ( p.part == q.part && !e.isFull()  &&  ( s.part == PART.SOURCE || s.part == PART.SINK ) ) 
					p.parent = q;
		
			}
			if ( p.parent == null ) {//沒有找到適合的 變成free node拉
				edges = getEdge(p);//將和他連結的只要沒滿的Edge之Node都加進來
				for ( Edge e : edges.values() ) {
					Vertex q = null;
					if ( e.u == p ) q = e.v;
					else if ( e.v == p ) q = e.u;
					
					/*
					 * p becomes free 
					 * all its neighbors connected through non-saturated edges should became active.
					 * It may happen that some neighbor q did not qualify as a valid parent 
					 * during adoption stage because it did not originate from the source or the sink
					 */
					if ( !e.isFull() ) 
						if ( !active.contains(q) )
							active.add( q );
					if ( q.parent == p ) {
						if( !orphan.contains(q) )
							orphan.add(q);
						q.parent = null;
					}
					
						
				}
				
				p.part = PART.UNKNOWN;
				while ( active.contains(p) )
					active.remove(p);
			}
		}
	
		return active;
	}
	
	/**
	 * 找到起源
	 * @param n
	 * @return
	 */
	private Vertex origin(Vertex n ) {
		Vertex v = n;
		Vertex pre = v;
		while ( v != null ) {
			pre = v;
			v = v.parent;
		}
		
		return pre;
	}
	

}
