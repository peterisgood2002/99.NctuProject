/**
 * PURPOSE

   �]��Edmonds-Karp Algorithm �ӺC�F �C����j
        �]���ڭ̨ϥ�
        "An Experimental Comparison of Min-Cut/Max-Flow Algorithms
		 for Energy Minimization in Vision."
		��Algorithm 
		�̥D�n�O���̳z�L����ɪ������Ӷi��M��
		�]�N�O�� 
		  PPA
		  PPA
		s PPAA
		  PPPPA
		  PPAA 
		�z�LA�Ӷi��Edge�s�� 
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
		//��l�Ƹ�T
		Vector<Vertex> active = new Vector<Vertex>();
		source.part = PART.SOURCE;
		sink.part = PART.SINK;
		active.add(source);
		active.add(sink);
		
		//�}�]
		int index = 1;
		while ( true ) {
			System.out.println(index++);
			if (index == 34)
				System.out.println(index);
			
			zeroPath();
			System.out.println("�}�lgrow");
			Edge keyedge = grow( active );
			if (  keyedge == null ) break; 
			System.out.println("�}�lAugment");
			Vector<Vertex> orphan = augment(keyedge);
			System.out.println("�}�ladopt");
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
	 * �NEdge���]�tPath����T�k�s
	 */
	private void zeroPath() {
		for ( HashMap< Integer,Edge> ne : voedges ) {
			for ( Edge edge : ne.values() ) {
				edge.inverseedgetopath = false;
			}
		}
	}
	/*
	 * �z�L<code>active</code>���X�@��augmented path�öǥXpath��source����sink �̤p��capacity
	 * �ǥX�G�̤p��residual capacity��Edge
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
			//�p�G�ݩ�SOURCE �h��out edge SINK�h��inner edge
			if ( p.part == PART.SOURCE )
				edge = getOuterEdge(p);
			else if ( p.part == PART.SINK )
				edge = getInnerEdge(p); 
			
			for ( Edge e : edge.values() ) {
				if ( !e.isFull() ) {
					Vertex unknown = null ,known = null;//�w�g���D�H�Τ����D��Vertex�ݩ���@�ӳ���
					if ( p.part == PART.SOURCE ) {//���e.u ----->e.v�i�H��
						known = e.u;
						unknown = e.v;
					}
					else if ( p.part == PART.SINK ) {//���e.u ------>e.v�i��
						unknown = e.u;
						known = e.v;
					}
					
					
					if ( unknown.part == PART.UNKNOWN  ) {
						unknown.parent = known;//�]�wunknown��parent�Mknown��child
						unknown.part = known.part;//�]�wTREE(u) = TREE(v)
						if( !active.contains(unknown) ) active.add( unknown );
					}
					
					//��ɳB �����䪺edge��X�h
					if ( unknown.part != PART.UNKNOWN && unknown.part != known.part) {
						return e;
					}
						
				}
			}
		}
		
		return null;
	}
	/*
	 * ���X�R���ʧ@ ��R���@��path �q�ӱ�����edge����B�i���R ��Edge����@�w�O���Osource���� �k��@�w���Osink����
	 * @param edge ��R���q
	 * @return orphan��Ƶ��c
	 */
	private Vector<Vertex> augment( Edge edge) {
		Vector<Vertex> orphan = new Vector<Vertex>();
		double cap = computeCap( edge );
		
		//Key edge��R  ����@�w�O���Osource���� �k��@�w���Osink����
		edge.addFlow(cap);
		
		//��source�ݥh�� �ݬݦ��S��orphan �H�ζi���R
		Vertex node = edge.u;
		while ( node != source ) {
			System.out.println("Augment���� --source");
			Edge e = null;
			try {
				e = getEdge(node.parent ,node);
				e.addFlow(cap);
			}catch (NullPointerException ne) {//��Edge����path�����ϦVEdge
				e = getEdge(node ,node.parent);
				e.addFlow(-cap);
				e.inverseedgetopath = true;
				
			}
			
			if ( ( e.isFull() && !e.inverseedgetopath ) || ( e.isEmpty() && e.inverseedgetopath )) {
				Vertex s = e.u ,t = e.v;//s ----->t
				if ( s.part == t.part ) {//���L�t��
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
		
		//��sink�ݧ�
		node = edge.v;
		while ( node != sink ) {
			System.out.println("Augment����--sink");
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
				if ( s.part == t.part ) {//���L�t��
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
	 * ���oEdge <code>u</code>--------><code>v</code>
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
	 * �z�L e������I ��X���䪺residual capacity�̤p��
	 * @param e
	 * @return
	 */
	private double computeCap(Edge edge) {
		double cap = edge.residualCapacity();
		
		Vertex node = edge.u;//��source�ݧ�
		while ( node != source ) {
//			System.out.println("�p��source��");
			Edge e = null;
			Pixel pixel = (Pixel) node;
			System.out.println( pixel.x +" "+ pixel.y);
			try {
				e = getEdge( node.parent ,node );
				cap = Math.min(cap, e.residualCapacity() );
				e.inverseedgetopath = false;
			}catch (NullPointerException ne) {
				//���u-->v�S���u ���O�{���S���o�O�o�����|�� �ϦV���u �ҥH�n�^�y��
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
		
		node = edge.v;//��sink�ݧ�
		while ( node != sink ) {
//			System.out.println("�p��sink��");
			Edge e = null;

			try {
				e = getEdge( node ,node.parent );
				cap = Math.min(cap, e.residualCapacity() );
				e.inverseedgetopath = false;
			}catch (NullPointerException ne) {
				//���u-->v�S���u ���O�{���S���o�O�o�����|�� �ϦV���u �ҥH�n�^�y��
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
	 * �i��adopt step
	 * Each node p being processed tries to find a new valid parent within the same search tree; 
	 * in case of success p remains in the tree but with a new parent, 
	 * otherwise it becomes a free node and all its children are added to O.
	 
	 * @param active
	 * @param orphan
	 * @return active �H��K�᭱�갵
	 */
	private Vector<Vertex> adopt(Vector<Vertex> active, Vector<Vertex> orphan) {
		while ( !orphan.isEmpty() ) {
			System.out.println("ADopt����");
			Vertex p = orphan.remove(0);
			//�Q��k��neighborhood�ݬݯण����s��parent 
			
			/*
		     * A valid parent u should satisfy: 
			 * TREE(u) = TREE(v)
			 * cap(u -->v) > 0 TREE(v)= S u-->v TREE(v)= T v-->u
			 * the ��origin�� of u should be either source or sink.
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
			
			if ( p.parent == null ) {//�S�����A�X�� �ܦ�free node��
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
	 * ���_��
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
