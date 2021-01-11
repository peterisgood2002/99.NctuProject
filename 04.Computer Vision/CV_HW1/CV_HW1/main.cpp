/******************************************
FILE
    main.cpp
PURPOSE
    �{���D�n�i�J�I

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/
#include <iostream>
#include <fstream>
#include <vector>
#include "Picture.h"
#include "ImageReader.h"
#include "PhotoStereo.h"
#include <string>

using std::cout;
using std::vector;
using std::string;


/*�g��Obj��*/
void writeOBJ(string dn ,double* *hmap ,Normal* *nmap ,int row ,int col);
/*Debug��*/
void 
outputNormal(Normal* *nmap ,int row ,int col ) {
	
	std::ofstream output("./debug.txt");
	for ( int i = 0; i < row; i++ ) 
		for ( int j = 0; j < col;j++ ) {
			output << "("<<j<<","<<i<<")"<<nmap[i][j].x << " " << nmap[i][j].y << " " << nmap[i][j].z <<"\n";
		}

}
Normal* *readNormal(string str ,int row ,int col ) {
	std::ifstream input( str.c_str() );


	Normal* *nmap = create3DArray(row ,col );
	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			string str1;
			input >> str1;
			str1 = str1.substr( str1.find_first_of(")")+1);
			nmap[i][j].x = strtod( str1.c_str() ,(char **)NULL );
			input >> nmap[i][j].y;
			input >> nmap[i][j].z;
			

		}
	}

	return nmap;
}
/*Debug��*/
int 
main() {
	
	//��l��GDI+�������F��
	Picture::initGDIPlus();

	vector<Picture> imgs = readIMG();
	/*imgs[0].drawPicture("D:/Program/CV_HW1/CV_HW1/1.jpeg");*/
		
	cout << "�p��Normal Map��\n";
	Normal** nmap = buildNormalMap(imgs);

	/*Debug*/
	//outputNormal(nmap , imgs[0].getHeight() ,imgs[0].getWidth() );
	//string str = "./images/teapot/debug.txt";
	//Normal** nmap = readNormal(str ,imgs[0].getHeight() ,imgs[0].getWidth() );
	/*Debug end*/
	cout << "Normalize normal�ç�X(-df/dx ,-df/dy ,1 )\n";
	Picture p = imgs[0];
	int row = p.getHeight();
	int col = p.getWidth();
	nmap = normalizeN(nmap ,row ,col );
	nmap = findF(nmap ,row ,col );
	cout << "Normalize����\n";

	cout << "�إ�Height Map\n";
	double* *hmap = buildHeightMap(nmap ,row ,col );
	cout << "�إ�Height map����\n";
	
	cout << "�i��ϧΥ��Ƥ�\n";
	hmap = smoothHMap(hmap ,row ,col);
	cout << "���Ƶe����\n";
	cout << "�g�XObj��\n";
	writeOBJ(p.getDirName() ,hmap ,nmap ,row ,col);

	/*�R���O�����T*/
	for ( int i = 0; i < row; i++ ) {
		delete hmap[i];
		delete nmap[i];
	}

	/*test function correct*/
	/*double* * hmap = create2DArray(3 ,4 );
	Normal* * nmap = create3DArray(3 ,4 );
	int ti = 10;
	for ( int i = 0; i < 3; i++ ) 
		for ( int j = 0; j < 4; j++ ) {
			ti+=10;
			hmap[i][j] = ti ;
			Normal n;
			n.x = i;
			n.y = j;
			n.z = ti;
			nmap[i][j] = n;
		}
	writeOBJ("1.obj",hmap ,nmap,3 ,4);*/
	/*test function����*/
	cout<<"����\n";

}

void writeOBJ(string dn ,double* *hmap ,Normal* *nmap ,int row ,int col) {
	dn +="model.obj";
	FILE* output = fopen(dn.c_str() ,"w" );

	try {
		fprintf( output ,"g  default\n");
		//��output�Ҧ��I����m
		for ( int i = 0; i < row; i++ )
			for ( int j = 0; j < col; j++ ) {
				fprintf( output ,"v %d %d %f\n",i ,j ,hmap[i][j]);
				//fprintf( output ,"v %d %d %f\n",i ,j , 3 * nmap[i][j].y);
				//printf("v %d %d %f\n",i ,j ,hmap[i][j]);
			}

		//Output �Ҧ���Normal
		for ( int i = 0; i < row; i++ )
			for ( int j = 0; j < col; j++ ) {
				fprintf( output ,"vn %f %f %f\n",nmap[i][j].x ,nmap[i][j].y ,nmap[i][j].z);
				//printf("vn %f %f %f\n",nmap[i][j].x ,nmap[i][j].y ,nmap[i][j].z);
			}
	
		//Output �Ҧ���face
		int fn = ( row - 1 ) * ( col  ) ;
		for ( int i = 1; i < fn; i++ ) {
			if ( i % col != 0 ) {
				//�Ĥ@�ӭ�
				fprintf( output ,"f %d//%d %d//%d %d//%d \n",i ,i ,(i+col) ,(i+col) ,i+1 ,i+1 );
				//printf("f %d//%d %d//%d %d//%d \n",i ,i ,i+1 ,i+1 ,(i+col) ,(i+col) );
				//�۹�������
				fprintf( output ,"f %d//%d %d//%d %d//%d \n",(i+col) ,(i+col) , (i+col+1) ,(i+col+1) ,i+1 ,i+1);
				//printf("f %d//%d %d//%d %d//%d \n",i+1 ,i+1  ,(i+col+1) ,(i+col+1) ,(i+col) ,(i+col));
			}
		}
		fclose(output);

	}catch (char* e) {
		std::cerr << e;
	}
	


}


