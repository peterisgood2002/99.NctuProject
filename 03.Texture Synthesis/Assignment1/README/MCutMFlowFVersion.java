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
 * @author peter
 */
package Algorithm;

import java.io.IOException;
import java.util.HashMap;
import java.util.Vector;

import WangTile.Pixel;

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
			System.out.println(index++);
			if (index == 34)
				System.out.println(index);
			
			zeroPath();
			System.out.println("開始grow");
			Edge keyedge = grow( active );
			if (  keyedge == null ) break; 
			System.out.println("開始Augment");
			Vector<Vertex> orphan = augment(keyedge);
			System.out.println("開始adopt");
			active = adopt( active ,orphan);
			
		}
		
		for ( HashMap< Integer ,Edge> e : voedges ) {
			for (Edge edge : e.values() ) {
				int u = getVIndex(edge.u);
				int v = getVIndex(edge.v);
				System.out.printf("edges[%d][%d] flow = %f\n", u ,v ,edge.getFlow());
			}
		}
	
	}
	/**
	 * 將Edge中包含Path的資訊歸零
	 */
	private void zeroPath() {
		for ( HashMap< Integer,Edge> ne : voedges ) {
			for ( Edge edge : ne.values() ) {
				edge.inverseedgetopath = false;
			}
		}
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
			Pixel pixel = (Pixel) p;
			if ( pixel.x == 19 && pixel.y == 27) {
				int i = 0;
				i++;
			}
			HashMap< Integer ,Edge> edge = null;
			//如果屬於SOURCE 則為out edge SINK則為inner edge
			if ( p.part == PART.SOURCE )
				edge = getOuterEdge(p);
			else if ( p.part == PART.SINK )
				edge = getInnerEdge(p); 
			
			for ( Edge e : edge.values() ) {
				if ( !e.isFull() ) {
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
						if( !active.contains(unknown) ) active.add( unknown );
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
			System.out.println("Augment偵錯 --source");
			Edge e = null;
			try {
				e = getEdge(node.parent ,node);
				e.addFlow(cap);
			}catch (NullPointerException ne) {//此Edge為此path中的反向Edge
				e = getEdge(node ,node.parent);
				e.addFlow(-cap);
				e.inverseedgetopath = true;
				
			}
			
			if ( ( e.isFull() && !e.inverseedgetopath ) || ( e.isEmpty() && e.inverseedgetopath )) {
				Vertex s = e.u ,t = e.v;//s ----->t
				if ( s.part == t.part ) {//讓他孤單
					if ( node.part == PART.SOURCE  ) {
						if ( !e.inverseedgetopath ) {
							t.parent = null;
							orphan.add( t );
						}else {
							s.parent = null;
							orphan.add( s );
						}
					}
					
					
				}
			}
			if ( !e.inverseedgetopath ) node = e.u;
			else node = e.v;
		}
		
		//往sink端找
		node = edge.v;
		while ( node != sink ) {
			System.out.println("Augment偵錯--sink");
			Edge e = null;
			
			try {
				e = getEdge( node ,node.parent );
				e.addFlow(cap);
			}
			catch (NullPointerException ne) {
				e = getEdge( node.parent ,node);
				e.addFlow(-cap);
				e.inverseedgetopath = true;
				
			}
			if ( ( e.isFull() && !e.inverseedgetopath ) || ( e.isEmpty() && e.inverseedgetopath )) {
				Vertex s = e.u ,t = e.v;//s ----->t
				if ( s.part == t.part ) {//讓他孤單
					if ( node.part == PART.SINK ) {
						if ( !e.inverseedgetopath) {
							s.parent = null;
							orphan.add( s );
						}else {
							t.parent = null;
							orphan.add( t );
						}
					}
					
				}
			}
			if ( !e.inverseedgetopath ) node = e.v;
			else node = e.u;
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
			Pixel pixel = (Pixel) node;
			System.out.println( pixel.x +" "+ pixel.y);
			try {
				e = getEdge( node.parent ,node );
				cap = Math.min(cap, e.residualCapacity() );
				e.inverseedgetopath = false;
			}catch (NullPointerException ne) {
				//表示u-->v沒有線 但是程式又說這是這條路徑中 反向的線 所以要回流壓
				e = getEdge( node ,node.parent );
				e.inverseedgetopath = true;
			}finally {
				cap = Math.min(cap, e.residualCapacity() );
			}
			if ( !e.inverseedgetopath ) 
				node = e.u;
			else 
				node = e.v;
		}
		
		node = edge.v;//往sink端找
		while ( node != sink ) {
//			System.out.println("計算sink端");
			Edge e = null;

			try {
				e = getEdge( node ,node.parent );
				cap = Math.min(cap, e.residualCapacity() );
				e.inverseedgetopath = false;
			}catch (NullPointerException ne) {
				//表示u-->v沒有線 但是程式又說這是這條路徑中 反向的線 所以要回流壓
				e = getEdge( node.parent ,node  );
				e.inverseedgetopath = true;
			}finally {
				cap = Math.min(cap, e.residualCapacity() );
			}
			if ( !e.inverseedgetopath ) node = e.v;
			else node = e.u;
			
		}
		
		System.out.println("cap = "+cap);
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
			System.out.println("ADopt偵錯");
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
				edges = getOuterEdge(p);
			else if ( p.part == PART.SINK )
				edges = getInnerEdge(p);
			for ( Edge e : edges.values() ) {
				
				Vertex q = null;
				if ( e.u == p ) q = e.v;
				else if ( e.v == p ) q = e.u;
				
				if ( !e.inverseedgetopath ) {
					if ( p.part == q.part && !e.isFull()  &&  (q.part == PART.SOURCE || q.part == PART.SINK ) ) {
						p.parent = q;
					}
				}else {
					if ( p.part == q.part && !e.isEmpty()  &&  (q.part == PART.SOURCE || q.part == PART.SINK ) ) {
						p.parent = q;
					}
				}
				
			}
			
			if ( p.parent == null ) {//沒有找到適合的 變成free node拉
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
						if( !active.contains(q) )
							active.add( q );
					if ( q.parent == p ) {
						if( !orphan.contains(q) )
							orphan.add(q);
						q.parent = null;
					}
					
						
				}
				
				p.part = PART.UNKNOWN;
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
			v = n.parent;
		}
		
		return pre;
	}
	

}
