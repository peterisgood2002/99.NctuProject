/**
 * PURPOSE
      this is an instance of graphic
      �L�N�p��maximum flow min cut
      1.Edge���x�s�Hadjacency matrix 
        �ҥH�z�פWEdge���j�p��vertex.length * vertex.length
        Edge����ܪk���ӬO
	                25
	      1  ---------------> 3
	      ��1��row������3��col��cost ��25
	 * �p�G�S��Edge�h�i�H��null
      2.�ϥ�Edmonds-Karp Algorithm
        �ԽнШ� http://en.wikipedia.org/wiki/Edmonds-Karp_algorithm
 * @author peter
 */
package Algorithm;

import java.util.Vector;
import Algorithm.Vertex.PART;


public class MCutMFlowGraphic implements Graphic{
	private Vector<Vertex> nodes;
	//�`�@�|�����Edge���O�OVertex��inner edge�Mouter edge
	private Vector< Vector<Edge> > viedges;//Vertex��inner edge
	private Vector< Vector<Edge> > voedges;//outer edge
	private Vertex source;
	private Vertex sink;
	/**
	 * Constructor 
	 * Edge����ܪk���ӬO
	 *               25
	 *     1  ---------------> 3
	 *     ��1��row������3��col��cost ��25
	 * �p�G�S��Edge�h�i�H��null
	 * @param v �I���Ӽ�
	 * @param e �� �]����Ӥj �ҥH��Vector of Vector�i���x�s
	 * @param s �ĴX���I�n��source
	 * @param sink �ĴX���I�n��sink
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
	 * �qVertex���إ�edge
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
	 * �إߤ@��2D Array �̭����O�ŭ�
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
	 * ����Ƶ��c�k�s
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
	 * ���oVertex �L��index
	 * �ǤJ�GVertex
	 * �ǥX�G�L�bArray��index
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
			//�p�G���qsource�X�h�٦�edge �S���� ����٥i�H���
			cap = findAugmented(source ,sink ,Double.MAX_VALUE);
			
			System.out.println("���F"+ ++index);
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
	//���o�@��vertex��OuterEdge�MInnerEdge
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
	 * ���禡�N�|���q<code>source</code>�X�h���@��Augmented Path<br />
	 * �åB�^�Ǧ�Augmented Path�̤p��capacity
	 */
	private double findAugmented(Vertex source ,Vertex sink ,double mincap) {
		double cap = 0;
		
		if ( source == sink ) return mincap;
		
		source.marked = true;
		
		//���Ҧ� source -------> i �S������
		Vector<Edge> soe = getOuterEdge(source);
		for ( Edge edge : soe ) {
			if ( !edge.v.marked && !edge.isFull() && cap == 0) {
				cap = Math.min( mincap, edge.residualCapacity() );
				cap = findAugmented(edge.v ,sink , cap);
				if ( cap > 0 ) {//�[�Jflow
					edge.addFlow(cap);
					System.out.println(getVIndex(source) +" " + getVIndex(edge.v) + " " + edge.getFlow() );
				}
			}
		}
		
		//�p�G���V�䤣�� �i�H���ϦV�� ���Ҧ�i -------->source �S������
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
	 * ��M�̵u��Cut
	 * @return
	 */
	public void calCut() {
		if ( !isrun ) run();
		
		/*
		 * �qSource part����M�Ҧ�Edge���S���� ���F�N��@��F
		 */
		Vector<Vertex> spart = new Vector<Vertex>();//�x�s�Ҧ�source�ݪ�node
		spart.add(source);
		while ( !spart.isEmpty() ) {
			Vertex u = spart.remove(0);
			u.part = PART.SOURCE;
			
			//��Ҧ�edge  u ------>v ���S�����[�i�h
			Vector<Edge> soe = getOuterEdge(u);
			for ( Edge edge: soe ) {
				Vertex v = edge.v;
				if ( !spart.contains(v) && ! edge.isFull() && v.part == PART.UNKNOWN )
					spart.add(v);
			}
		
			//��Ҧ�edge  v ------>u �� �Ū��[�i��
			Vector<Edge> sie = getInnerEdge(u);
			for ( Edge edge: sie ) {
				Vertex v = edge.u;
				if ( !spart.contains(v) && !edge.isEmpty() 
						&& v.part == PART.UNKNOWN )
					spart.add(v);
				
			}

		}		
		
		//��L�S��쳣�k��Sink�h
		for ( int i = 0; i < nodes.size(); i++ ) 
			if ( nodes.get(i).part == PART.UNKNOWN ) 
				nodes.get(i).part = PART.SINK;
		
		
		for ( int i = 0; i < nodes.size(); i++ )
			System.out.println(i +" "+ nodes.get(i).part );
			
		
	}
}
