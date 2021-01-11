/******************************************
FILE
    main.cpp
PURPOSE
    程式主要進入點
	此次作業是為了從幾個相對應點中找尋Camera的外部參數
	並且從這些外部參數中 推出這幾張圖所對應的3D幾何圖形為何
	有著以下幾個相關事項需要處理
	1.Epipolar algebra
	  p'：image 2 的某點
	  p：image 1 的某點
	  p' 垂直於T * p'
	  因此在沒有內部參數的情況下
	  0 = p' (T * p') = p' [T * (Rp + T) ] = p' (T * R)   p = p' E  p
	  在有內部參數的情況下
	  0 = p'  (K' E K)  p = p' F  p
    2.我們要透過p' p求解E 可以透過解最小值的方式求解 P E  = 0
	  換句話說只要求解最小的eigen value即可
    3.我們將使用Hartley Normalize 8-ptr algorithm來算出較佳的結果
      其矩陣為：
	     [ sqrt(2) / Avg     1               -m ]
	  S =[       1         sqrt(2) / Avg     -n ]
	     [       0			0				1 ]
    4.利用DP來進行對應
	  (1)你找對應的時候可能沒有對應點 此時我們使用插值的方法來插出對應點

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <gdiplus.h>
#include "Picture.h"
#include "readData.h"
#include "Stereopsis.h"
#include "ArrayManipulation.h"
#include "rectification.h"
#include "calFMatrix.h"
//#include "DPAlgo.h"


using std::cout;
using std::cin;
using std::vector;
using std::ofstream;
using std::ifstream;
using namespace Gdiplus;

/*寫成Obj檔*/
void writeOBJ(string dn ,Vertex* *v ,int row ,int col);
/*將depth寫成img*/
void writeImg(string dn ,Vertex* *v ,int row ,int col);

/*Debug用*/
void 
outputVertex(Vertex* *nmap ,int row ,int col ) {
	
	std::ofstream output("./debug.txt");
	for ( int i = 0; i < row; i++ ) 
		for ( int j = 0; j < col;j++ ) {
			output << "("<<j<<","<<i<<")"<<nmap[i][j].x << " " << nmap[i][j].y << " " << nmap[i][j].z <<"\n";
		}

}
Vertex* *readVertex(string str ,int row ,int col ) {
	std::ifstream input( str.c_str() );


	Vertex* *nmap = create3DArray(row ,col );
	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			string str1;
			input >> str1;
			str1 = str1.substr( str1.find_first_of(")")+1);
			nmap[i][j].x = strtod( str1.c_str() ,(char **)NULL );
			input >> str1;
			nmap[i][j].y = atof(str1.c_str());
			input >> str1;
			nmap[i][j].z =  atof(str1.c_str());
			

		}
	}

	return nmap;
}
/*Debug用*/
int 
main () {
	
	//ofstream output("./data.txt");
	//初始化GDI+的相關東西
	Picture::initGDIPlus();
	
	/*讀取相關資料*/
	string dname = "./images/pic004/";
	cout <<"請問處理之資料夾為何：";
	//cin >> dname;
	cout <<"是否需要rectification(Y/N)\n";
	char yorn ='y';
	//cin >> yorn;
	yorn = tolower(yorn);
	/*讀相關資料*/
	vector<Picture> p = getPicture( dname );
	if ( yorn == 'y' ) {
		vector<CorPoint> cp = getCorrespondPoint( dname +"correspond.txt" );
		double* * nl = create2DArray(3 ,3) , **nr = create2DArray(3 ,3);
		int imgw = p[0].getWidth() ,imgh = p[1].getHeight();
		vector<CorPoint> ncp = normalizeCP(cp ,imgw ,imgh ,nl ,nr ) ;

		cout <<"nl\n";
		for ( int i = 0; i < 3; i++ ) {
			for ( int j = 0; j < 3; j++ ) 
				cout << nl[i][j] <<" ";
			cout <<"\n";
		}
		cout<<"===========\n";
		for ( int i = 0; i < 3; i++ ) {
			for ( int j = 0; j < 3; j++ ) 
			cout << nr[i][j] <<" ";
		}
		cout <<"==========\n";

		double* * f = calF(ncp);
		/*output <<"f之前\n";
		for ( int i = 0; i < 3; i++ ) {
				
			for ( int j = 0; j < 3; j++ )
				output << f[i][j] <<" ";
			output<<"\n";
		}*/
		f = calKFK(nr ,f ,nl ,3 ,3);
		cout <<"f之後\n";
		for ( int i = 0; i < 3; i++ ) {
			for ( int j = 0; j < 3; j++ )
				cout << f[i][j] <<" ";
			cout <<"\n";
		}
		cout<<"=============\n";
		/*cout << "是否想畫出epipolar line看看結果\n";
		cin >> yorn;
		yorn = tolower(yorn);
		if ( yorn =='y' ) {
			drawEpiLine(p[0] ,dname + "epipolar line L.jpg" ,f ,cp ,true);
			drawEpiLine(p[1] ,dname + "epipolar line R.jpg" ,f ,cp ,false);
		}*/
		double* t = calT(f ,cp ,3 ,3 );
		cout <<"T\n";
		for ( int i = 0; i < 3; i++ ) 
			cout << t[i] <<" ";
		cout <<"\n================\n";
		double* * r = calR( f, t );
		cout <<"R\n";
		for ( int i = 0; i < 3; i++ ) {
			for ( int j= 0; j < 3; j++ ) 
				cout << r[i][j] <<" ";
			cout <<"\n";
		}
		cout << "================\n";
		p = getRectifyImage(p, f , t , r );
		p[0].drawPicture( dname + "retificate L.jpg" );
		p[1].drawPicture( dname + "retificate R.jpg" );
		dispose(nl ,3 ,3);
		dispose(nr ,3 ,3);
		dispose(f ,3 ,3);
		/*output.close();*/
	}

	/*要求1*/
	Vertex* * ldmap = get3DValue(p[0] ,p[1] ,25 , 3 , 3);
	/*Debug mode*/
	outputVertex(ldmap ,p[0].getHeight() ,p[0].getWidth() );
	//Vertex* * ldmap = readVertex("./debug.txt",p[0].getHeight() ,p[0].getWidth() );
	writeImg(dname ,ldmap ,p[0].getHeight() ,p[0].getWidth() );
	/*Debug mode*/
	cout <<"輸出成Obj檔\n";
	writeOBJ( p[0].getDirName(),ldmap ,p[0].getHeight() ,p[0].getWidth()  );
	cout <<"輸出成Obj檔結束\n";
	
	
	for ( int i = 0; i < p[0].getHeight(); i++ ) {
		delete ldmap[i];
	}
	delete ldmap;
	/*要求1*/

	/*測試程式區*/
	/*vector<WColorPtr>* vp ;
	WColorPtr wp = new WColor();
	vp->push_back(wp);
	WColorPtr tmp = (*vp)[0];*/
	/*double l[4] = { 4 ,3 ,2 ,1};
	double r[4] = { 5 ,4 ,2 ,1};
	double* * d = dpAlgorithm(l ,4 ,r ,4);
	int* a = getCorrespondence(d ,4 ,4 ,COL);
	for ( int i = 0; i < 4; i++ ) 
		cout <<a[i]<<" ";*/
	/*double l[8] = { 1 ,2 ,3 ,4 ,5 ,6 ,7 ,8};
	double r[8] = { 1 ,2 ,4 ,3 ,10 ,6 ,7 ,8};
	double* * d = dpAlgorithm(l ,8 ,r ,8);
	int* a = getCorrespondence(d ,8 ,8 ,ROW);
	for ( int i = 0; i < 8; i++ ) 
		cout <<a[i]<<" ";*/
}


void 
writeOBJ(string dn ,Vertex* *v  ,int row ,int col) {
	dn +="model.obj";
	FILE* output = fopen(dn.c_str() ,"w" );

	try {
		fprintf( output ,"g  default\n");
		//先output所有點的位置
		for ( int i = 0; i < row; i++ )
			for ( int j = 0; j < col; j++ ) {
				fprintf( output ,"v %d %d %f\n",i ,j ,-v[i][j].z * 10);
				//fprintf( output ,"v %d %d %f\n",i ,j , 3 * nmap[i][j].y);
				//printf("v %d %d %f\n",i ,j ,hmap[i][j]);
			}
		//沒有normal
		//Output 所有的face
		int fn = ( row - 1 ) * ( col  ) ;
		for ( int i = 1; i < fn; i++ ) {
			if ( i % col != 0 ) {
				//第一個面
				fprintf( output ,"f %d// %d// %d// \n",i  ,(i+col)  ,i+1  );
				//printf("f %d//%d %d//%d %d//%d \n",i ,i ,i+1 ,i+1 ,(i+col) ,(i+col) );
				//相對應的面
				fprintf( output ,"f %d// %d// %d// \n",(i+col) , (i+col+1) ,i+1 );
				//printf("f %d//%d %d//%d %d//%d \n",i+1 ,i+1  ,(i+col+1) ,(i+col+1) ,(i+col) ,(i+col));
			}
		}
		fclose(output);

	}catch (char* e) {
		std::cerr << e;
	}
	
}

/*將depth寫成img*/
/*畫圖確定有寫進去用*/
WCHAR* getUnicode( const string str );
int   
GetEncoderClsid(const   WCHAR*   format,   CLSID*   pClsid);
void 
writeImg(string dn ,Vertex* *v ,int row ,int col) {
	dn +="depth.jpg";
	
	Bitmap* img = new Bitmap(col ,row ,PixelFormat24bppRGB);

	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			int h = (int)v[i][j].z * 50;
			Color c((BYTE)h ,(BYTE)h ,(BYTE)h );
			img->SetPixel(j ,i ,c);
		}
	}

	WCHAR* fname = getUnicode(dn);
	CLSID clsid;
	GetEncoderClsid(L"image/jpeg", &clsid);//取得codec
	Status s = img->Save(fname , &clsid);
}