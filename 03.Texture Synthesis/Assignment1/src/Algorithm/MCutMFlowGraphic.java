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
	    �p�G�S��Edge�h�i�H��null
      2.�ϥ�Edmonds-Karp Algorithm
        �ԽнШ� http://en.wikipedia.org/wiki/Edmonds-Karp_algorithm
      
 * @author peter
 */
package Algorithm;

import java.util.HashMap;
import java.util.Vector;

import Algorithm.Vertex.PART;


public class MCutMFlowGraphic implements Graphic{
	protected Vector<Vertex> nodes;
	//�`�@�|�����Edge���O�OVertex��inner edge�Mouter edge
	protected Vector< HashMap< Integer ,Edge> > viedges;//Vertex��inner edge
	protected Vector< HashMap< Integer ,Edge> > voedges;//outer edge
	protected Vertex source;
	protected Vertex sink;
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
		HashMap< Integer,Edge> edge = voedges.get(u);
		if ( !edge.containsKey(v) ) 
			edge.put(v, e);//�p�G���]�t�o��Edge �h�[�JEdge
		edge = viedges.get(v);
		if ( !edge.containsKey(u) )
			edge.put(u, e);
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
		 viedges = new Vector< HashMap< Integer,Edge> >();
		 voedges = new Vector< HashMap< Integer,Edge> >();
		 
		 for ( int i = 0; i < vsize; i++ ) {
			 viedges.add( new HashMap< Integer,Edge>() );
			 voedges.add( new HashMap< Integer,Edge>() );
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
		
		for ( HashMap< Integer,Edge> ne : voedges ) {
			
			for ( Edge edge : ne.values() ) {
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
//		for ( HashMap< Integer ,Edge> e : voedges) {
//			for (Edge edge : e.values() ) {
//				int u = getVIndex(edge.u);
//				int v = getVIndex(edge.v);
//				System.out.printf("edges[%d][%d] flow = %f\n", u ,v ,edge.getFlow());
//			}
//		}
		int index = 1;
		while( cap != 0 ) {
//			System.out.println("���F"+ index++);
			//�p�G���qsource�X�h�٦�edge �S���� ����٥i�H���
			cap = findAugmented(source ,sink ,Double.MAX_VALUE);
			
//			if ( index == 9 ) {
//				for ( HashMap< Integer ,Edge> e : voedges) {
//					for (Edge edge : e.values() ) {
//						int u = getVIndex(edge.u);
//						int v = getVIndex(edge.v);
//						System.out.printf("edges[%d][%d] flow = %f\n", u ,v ,edge.getFlow());
//					}
//				}
//			}

			
		}
		
		//�ˬd�s��sink��Edge
//		HashMap< Integer ,Edge> e = viedges.get( nodes.size() - 1);
//		for ( HashMap< Integer ,Edge> e : voedges) {
//			for (Edge edge : e.values() ) {
//				int u = getVIndex(edge.u);
//				int v = getVIndex(edge.v);
//				System.out.printf("edges[%d][%d] flow = %f\n", u ,v ,edge.getFlow());
//			}
//		}

		isrun = true;
	}
	
	//���o�@��vertex��OuterEdge�MInnerEdge
	protected HashMap< Integer ,Edge> getOuterEdge( Vertex node ) {
		try {
			return voedges.get( getVIndex(node) );
		}catch (ArrayIndexOutOfBoundsException e) {
			return null;
		}
	}
	protected HashMap< Integer ,Edge> getInnerEdge( Vertex node) {
		try {
			return viedges.get( getVIndex(node) );
		}catch (ArrayIndexOutOfBoundsException e) {
			return null;
		}
	}
	protected HashMap< Integer ,Edge> getEdge( Vertex node ) {
		try {
			HashMap< Integer ,Edge> edge = new HashMap< Integer ,Edge>();
			edge.putAll( getOuterEdge(node) );
			edge.putAll( getInnerEdge(node) );
			return edge;
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
		
		if ( source == sink ) 
			return mincap;
		
		source.marked = true;
		
		//���Ҧ� source -------> i �S������
		HashMap< Integer ,Edge> soe = getOuterEdge(source);
		for ( Edge edge : soe.values() ) {
			if ( !edge.v.marked && !edge.isFull() && cap == 0) {
				cap = Math.min( mincap, edge.residualCapacity() );
//				System.out.println( ( (Pixel)edge.v ).x + " " + ( (Pixel)edge.v ).y );
				cap = findAugmented(edge.v ,sink , cap);
				if ( cap > 0 ) {//�[�Jflow
					edge.addFlow(cap);
//					System.out.println(getVIndex(source) +" " + getVIndex(edge.v) + " " + edge.getFlow() );
				}
			}
		}
		
		//�p�G���V�䤣�� �i�H���ϦV�� ���Ҧ�i -------->source �S������
		if ( cap == 0 ) {
			HashMap< Integer ,Edge> sie = getInnerEdge(source);
			for ( Edge edge : sie.values() ) {
				if ( !edge.u.marked && edge.hasFlow() && cap == 0) {
					cap = Math.min( mincap, edge.flow );
					cap = findAugmented(edge.u ,sink , cap);
					if ( cap > 0 ) {
//						System.out.println(getVIndex(edge.u) +" " + getVIndex(source) + " " + edge.getFlow() );
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
			HashMap< Integer ,Edge> soe = getOuterEdge(u);
			for ( Edge edge: soe.values() ) {
				Vertex v = edge.v;
				if ( !spart.contains(v) && ! edge.isFull() && v.part == PART.UNKNOWN )
					spart.add(v);
			}
		
			//��Ҧ�edge  v ------>u �� �Ū��[�i��
			HashMap< Integer ,Edge> sie = getInnerEdge(u);
			for ( Edge edge: sie.values() ) {
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
		
		
//		for ( int i = 0; i < nodes.size(); i++ )
//			System.out.println(i +" "+ nodes.get(i).part );
			
		
	}
}
