/******************************************
FILE
    main.cpp
PURPOSE
    �{���D�n�i�J�I
	�����@�~�O���F�q�X�Ӭ۹����I����MCamera���~���Ѽ�
	�åB�q�o�ǥ~���ѼƤ� ���X�o�X�i�ϩҹ�����3D�X��ϧά���
	���ۥH�U�X�Ӭ����ƶ��ݭn�B�z
	1.Epipolar algebra
	  p'�Gimage 2 ���Y�I
	  p�Gimage 1 ���Y�I
	  p' ������T * p'
	  �]���b�S�������Ѽƪ����p�U
	  0 = p' (T * p') = p' [T * (Rp + T) ] = p' (T * R)   p = p' E  p
	  �b�������Ѽƪ����p�U
	  0 = p'  (K' E K)  p = p' F  p
    2.�ڭ̭n�z�Lp' p�D��E �i�H�z�L�ѳ̤p�Ȫ��覡�D�� P E  = 0
	  ���y�ܻ��u�n�D�ѳ̤p��eigen value�Y�i
    3.�ڭ̱N�ϥ�Hartley Normalize 8-ptr algorithm�Ӻ�X���Ϊ����G
      ��x�}���G
	     [ sqrt(2) / Avg     1               -m ]
	  S =[       1         sqrt(2) / Avg     -n ]
	     [       0			0				1 ]
    4.�Q��DP�Ӷi�����
	  (1)�A��������ɭԥi��S�������I ���ɧڭ̨ϥδ��Ȫ���k�Ӵ��X�����I

NOTE
	

AUTHOR
    9757553 �C����
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

/*�g��Obj��*/
void writeOBJ(string dn ,Vertex* *v ,int row ,int col);
/*�Ndepth�g��img*/
void writeImg(string dn ,Vertex* *v ,int row ,int col);

/*Debug��*/
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
/*Debug��*/
int 
main () {
	
	//ofstream output("./data.txt");
	//��l��GDI+�������F��
	Picture::initGDIPlus();
	
	/*Ū���������*/
	string dname = "./images/pic004/";
	cout <<"�аݳB�z����Ƨ�����G";
	//cin >> dname;
	cout <<"�O�_�ݭnrectification(Y/N)\n";
	char yorn ='y';
	//cin >> yorn;
	yorn = tolower(yorn);
	/*Ū�������*/
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
		/*output <<"f���e\n";
		for ( int i = 0; i < 3; i++ ) {
				
			for ( int j = 0; j < 3; j++ )
				output << f[i][j] <<" ";
			output<<"\n";
		}*/
		f = calKFK(nr ,f ,nl ,3 ,3);
		cout <<"f����\n";
		for ( int i = 0; i < 3; i++ ) {
			for ( int j = 0; j < 3; j++ )
				cout << f[i][j] <<" ";
			cout <<"\n";
		}
		cout<<"=============\n";
		/*cout << "�O�_�Q�e�Xepipolar line�ݬݵ��G\n";
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

	/*�n�D1*/
	Vertex* * ldmap = get3DValue(p[0] ,p[1] ,25 , 3 , 3);
	/*Debug mode*/
	outputVertex(ldmap ,p[0].getHeight() ,p[0].getWidth() );
	//Vertex* * ldmap = readVertex("./debug.txt",p[0].getHeight() ,p[0].getWidth() );
	writeImg(dname ,ldmap ,p[0].getHeight() ,p[0].getWidth() );
	/*Debug mode*/
	cout <<"��X��Obj��\n";
	writeOBJ( p[0].getDirName(),ldmap ,p[0].getHeight() ,p[0].getWidth()  );
	cout <<"��X��Obj�ɵ���\n";
	
	
	for ( int i = 0; i < p[0].getHeight(); i++ ) {
		delete ldmap[i];
	}
	delete ldmap;
	/*�n�D1*/

	/*���յ{����*/
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
		//��output�Ҧ��I����m
		for ( int i = 0; i < row; i++ )
			for ( int j = 0; j < col; j++ ) {
				fprintf( output ,"v %d %d %f\n",i ,j ,-v[i][j].z * 10);
				//fprintf( output ,"v %d %d %f\n",i ,j , 3 * nmap[i][j].y);
				//printf("v %d %d %f\n",i ,j ,hmap[i][j]);
			}
		//�S��normal
		//Output �Ҧ���face
		int fn = ( row - 1 ) * ( col  ) ;
		for ( int i = 1; i < fn; i++ ) {
			if ( i % col != 0 ) {
				//�Ĥ@�ӭ�
				fprintf( output ,"f %d// %d// %d// \n",i  ,(i+col)  ,i+1  );
				//printf("f %d//%d %d//%d %d//%d \n",i ,i ,i+1 ,i+1 ,(i+col) ,(i+col) );
				//�۹�������
				fprintf( output ,"f %d// %d// %d// \n",(i+col) , (i+col+1) ,i+1 );
				//printf("f %d//%d %d//%d %d//%d \n",i+1 ,i+1  ,(i+col+1) ,(i+col+1) ,(i+col) ,(i+col));
			}
		}
		fclose(output);

	}catch (char* e) {
		std::cerr << e;
	}
	
}

/*�Ndepth�g��img*/
/*�e�ϽT�w���g�i�h��*/
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
	GetEncoderClsid(L"image/jpeg", &clsid);//���ocodec
	Status s = img->Save(fname , &clsid);
}