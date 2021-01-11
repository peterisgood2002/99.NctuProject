/**
 * PURPOSE
      This program is to produce texture synthesis algorithm for wang tile 
      and the Main class purpose is the entry point of the application
 * @author Zhi-Cheng Yan
 *
 */
package UI;

import java.awt.Image;
import java.io.File;
import java.util.ArrayList;
import java.util.TreeSet;
import java.util.Vector;

import Algorithm.Edge;
import Algorithm.Graphic;
import Algorithm.MCutMFlowFVersion;
import Algorithm.MCutMFlowGraphic;
import Algorithm.Vertex;


public class Main {
	
	public static void main(String[] args) {
		System.out.println("磅︽祘Α\n");
		
		Application a = new Application("Wang tile using Graph cut" ,"./Images/");
		System.out.println(System.getProperty("user.dir") );
		
		/*
		int[][] x ={ {1 ,2 ,3},
				     { 4 ,5 ,6},
				     { 7 ,8 ,9}};
		
		int h = 3;
		int inity = 0 ,iy = 1 ,finishy = 3;//y絛瞅
		switch ( h ){
			case 0://рtmpオ场だ
				inity = 0; 
				iy = 1;
				finishy = 3;
				break;
			case 1://рtmpオ场だ
				inity = 0; finishy = 3; 
				iy = -1;
				break;
			case 2://рtmp场だ
				inity = 0; 
				finishy = 1; 
				iy = 1;
				break;
			case 3://рtmp场だ
				inity = 3 - 1;
				finishy = 3; iy=-1;
				break;
		}
		
		for( int i = 0; i < 3; i++) {
			for(int j = inity; j < finishy; j++) {
				x[j][i] = 0;
			}
			
			if ( h == 0 || h == 3)
				inity += iy;
			else finishy += iy;
		}
		
		for ( int i = 0; i < 3; i++ ) {
			for ( int j = 0; j < 3; j++ )
				System.out.print(x[i][j] +" ");
			System.out.print("\n");
		}
		*/
		
		/*
		//癸把σCode糶Min cut秈︽场璽砫盎岿
		int num = 7;
		Vector<Vertex> nodes = new Vector<Vertex>(num);
		for ( int i = 0; i < num ;i++ )
			nodes.add( new Vertex() );
		
		
		MCutMFlowFVersion min = new MCutMFlowFVersion(nodes , 0 ,num-1);
//		MCutMFlowGraphic min = new MCutMFlowGraphic(nodes, 0 ,6);
		min.setEdge(0, 1, 3);
		min.setEdge(0, 3, 3);
		min.setEdge(1, 2, 4);
		min.setEdge(2, 0, 3);
		min.setEdge(2, 3, 1);
		min.setEdge(2, 4, 2);
		min.setEdge(3, 5, 6);
		min.setEdge(3, 4, 2);
		min.setEdge(4, 1, 1);
		min.setEdge(4, 6, 1);
		min.setEdge(5, 6, 9);
		
//		min.setEdge(0, 1, 16);
//		min.setEdge(0, 2, 13);
//		min.setEdge(1, 2, 10);
//		min.setEdge(1, 3, 12);
//		min.setEdge(2, 1, 4);
//		min.setEdge(2, 4, 14);
//		min.setEdge(3, 2, 9);
//		min.setEdge(4, 3, 7);
//		min.setEdge(4, 5, 4);
//		min.setEdge(3, 5, 20);
		
		System.out.println("秨﹍禲MINCUT");
		min.run();
		System.out.println("禲MINCUT挡");
		min.calCut();
		*/
		
		
		
	}

}

class I implements Comparable{
	int i;
	public I(int i ) {
		this.i = i;
	}
	@Override
	public int compareTo(Object arg0) {
		// TODO Auto-generated method stub
		I tmp = (I) arg0;
		
		if ( i < tmp.i ) return -1;
		else if ( i == tmp.i ) return 0;
		else return 1;
	}
	@Override
	public boolean equals(Object obj) {
		// TODO Auto-generated method stub
		I tmp = (I) obj;
		return i == tmp.i;
	}
}
