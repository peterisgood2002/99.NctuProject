/******************************************
FILE
    main.cpp
PURPOSE
    程式主要進入點
	此程式將會進行HDR的檔案讀寫入
	並且可以透過tone mapping產生一張圖

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/




/*一定要加否則你會出現R6034錯誤
  http://www.cnblogs.com/lain/archive/2008/09/16/1291955.html
*/
#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.VC90.CRT'  \
              version='9.0.21022.8' processorArchitecture='X86' publicKeyToken='1fc8b3b9a1e18e3b' language='*'\"")

/*正常使用*/
#include <iostream>

//#include "HDRLoader.h"
//#include "Essential/Histogram.h"
//#include "Essential/Matrix.h"
//#include "ToneAdjustment.h"
//#include <cmath>
//#include "Essential/Filter.h"
//#include "Essential/GaussianPyramid.h"
//#include "SaliencyMap.h"
#include "MotionSaliencyMap.h"
#include "ThesisToneMapping.h"
#include "HDRVideo.h"
#include "./Loader/EXRLoader.h"
#include <exception>

using std::cout;

/*找尋Memory Leak用*/
#include <stdlib.h>

#include <crtdbg.h>


#ifdef _DEBUG

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)

#define new DEBUG_NEW

#endif
/*找尋Memory Leak用*/

int main(){

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	/*HDR Video*/
	//HDRVideo hdr( ".\\EXR\\sun-hdrframes\\sun-hdrframes" );
	//HDRVideo hdr( ".\\EXR\\Test" );

	//HDRVideo hdr(".\\EXR\\sb-tunnel-exr\\" );
	//HDRVideo hdr( ".\\EXR\\New-20100715" );
	//HDRVideo hdr( ".\\EXR\\NewScene2-20100720\\" );
	//hdr.performWithTemporal("./NewScene2--20100721/");
	//hdr.performWithoutTemporal( "./EXR Videowo--20100714/" );
	//hdr.performWithoutTemporal( "./EXR Videowo--20100714/" );
	//hdr.performWithoutTemporal("./Newwo--20100719/");
	//hdr.performWithTemporal("./EXR Videosb--20100716/" );
	//hdr.performWithTemporal("./New--20100719/" );
	//hdr.performWithoutTemporal( "./EXR Video--20100718(0.66TA+0.33SA)/" );
	//hdr.performWithTemporal( "./EXR Video--20100718(0.66TA+0.33SA)/" );
	//hdr.performWithTemporal( "./EXR Video--20100731/" );
	//hdr.performWithTemporal( "./EXR Videosb--20100802/" );
	//hdr.performWithTemporal( "./EXR Videosb--20100802(0.330.66)/" );
	//hdr.performWithTemporal( "./papernocondition/" );
	//hdr.performWithTemporal( "./EXR Video--20100802(papernocondition)/" );
	//hdr.performWithTemporal( "./Test5/" );
	//hdr.performWithTemporal( "./Test8/" );
	//hdr.performWithTemporal( "./Allconst/" );
	//hdr.performWithTemporal( "./Test6NewBug/" );
	//array<String^>^ files = Directory::GetFiles( ".\\EXR\\sun-hdrframes\\sun-hdrframes" );
	
	/*ThesisToneMapping測試*/
	try {
		//cout << i << "\n";
		//ThesisToneMapping t( "./image/memorial.hdr" );
		//ThesisToneMapping t( "./image/scene.exr" ,2);
		//ThesisToneMapping t( "./image/AtriumNight.hdr");
		//ThesisToneMapping t( "./image/dani_synagogue.hdr");
		//ThesisToneMapping t( "./image/dani_belgium.hdr" );
		//ThesisToneMapping t( "./image/bigFogMap_oDAA.hdr" );
		//ThesisToneMapping t( "./image/Montreal_float_o935.hdr" );
		//ThesisToneMapping t( "./image/Tree_oAC1.hdr");
		//ThesisToneMapping t( "./image/bathroom.hdr");
		//ThesisToneMapping t( "./image/rosette_oC92.hdr" );
		//ThesisToneMapping t( "./image/SpheronSiggraph2001_oF1E.hdr" ,2);
		//ThesisToneMapping t( "./image/SpheronNapaValley_oC5D.hdr" );
		//ThesisToneMapping t( "./image/MtTamWest_o281.hdr" );
		//ThesisToneMapping t( ".\\EXR\\sb-tunnel-exr\\00167.exr"  );
		//ThesisToneMapping t( "./image/rend07_o85C.hdr" );
		//ThesisToneMapping t( "./image/rend10_oF1C.hdr" );
		//ThesisToneMapping t( "./image/SpheronPriceWestern_o264.hdr");
		//ThesisToneMapping t( "./image/SpheronNice_o9E0.hdr");
		//ThesisToneMapping t( "./image/Spheron3_oBAC.hdr");
		//ThesisToneMapping t( "./image/Display1000_float_o446.hdr");
		//ThesisToneMapping t( "./image/vinesunset.hdr" ,2 );
		//ThesisToneMapping t( "./EXR/apple output/0001.exr" ,2);
		//ThesisToneMapping t( "./image/apple.exr" ,2);
		//ThesisToneMapping t( "./EXR/output2/temp0041.sc.exr"  );
		//ThesisToneMapping t( ".\\EXR\\sun-hdrframes\\sun-hdrframes\\832.exr" );
		//ThesisToneMapping t("./EXR/New-20100715/Temp0000.sc.exr");
		//ThesisToneMapping t("./EXR/NewScene2-20100720/Temp0000.sc.exr");
		//ThesisToneMapping t("./Temp3.exr");
		ThesisToneMapping t("./新資料夾 (3)/swan.jpg");
		t.perform();
		//t.saveResult("./result-0.01interpolationreinhard.jpg");
		t.saveResult("./result-20100915-2.jpg");
	}catch (char* c ) {
		cout << c;
	}catch ( std::exception e ) {
		cout << e.what() <<"\n";
	}
	
 //   Matrix* St_1 = NULL;
	////判斷是不是最後的frame
	//ThesisToneMapping t( ".\\EXR\\sb-tunnel-exr\\00166.exr" ,true ,false );
	//t.perform( St_1 );
	//St_1 = t.getAttentionMap();
	//ThesisToneMapping t1( ".\\EXR\\sb-tunnel-exr\\00167.exr" ,true ,true);
	//t1.perform( St_1 );
	//t1.saveResult("./result-20100801.jpg");

	/*ThesisToneMapping測試結束*/

	/*EXRLoader測試*/
    //EXRLoader* l = new EXRLoader("./image/000.exr");
	//cout << l->getY(0 ,0);
	//delete l;
	/*ThesisToneMapping測試結束*/

	/*SaliencyMap測試*/
	//SaliencyMap s( "./image/scene.exr" );
	//s.displayMap( *s.getSaliencyMap() ,"SaliencyMap2009.jpg");
	//s.displayMap( *s.getIConspicuityMap() ,"iCMap.jpg" );
	//s.displayMap( *s.getCConspicuityMap() ,"cCMAP.jpg" );
	//s.displayMap( *s.getOConspicuityMap() ,"oCMAP.jpg" );
	/*Matrix m( 2 ,2 );
	m.setData(0 ,0 ,1 );
	m.setData( 1 ,1 ,1 );
	cout << *SaliencyMap::interpolationFromCoarseToFine(m ,6 ,6 );*/
	
	//SaliencyMap s ( "./image/bathroom.hdr" );
	//GaussianPyramid k;
	

	/*Test Motion Salience Map*/
	//array<String^>^ files = Directory::GetFiles( ".\\Test\\tmp" );
	//for ( int i = 0; i < files->Length; i++ ) {
	//	String^ file = files[i];
	//	
	//	if ( file->Contains( ".jpg" ) ){
	//		try {
	//			string str = S2str( file );
	//			bool lastframe = false;
	//			if ( i == files->Length - 1 ) 
	//				lastframe = true;
	//			SaliencyMap* s = new MotionSaliencyMap( str ,lastframe );
	//			//SaliencyMap* s = new SaliencyMap( str  );

	//			Matrix* sm = s->getSaliencyMap();

	//			if ( i != 0 ) {
	//				string dest = "./Test/motion salience/";
	//				int firstindex = str.rfind( "\\" );
	//				int finalindex = str.rfind( "." );

	//				dest += str.substr( firstindex+1 ,finalindex - firstindex - 1 );
	//				dest +=".jpg";
	//				s->displayMap(*sm ,dest );
	//				delete sm;
	//				delete s;
	//			}
	//		}catch ( std::exception ex ) {
	//			cout << ex.what();
	//		}

	//		cout <<"==================\n";
	//	}
	//}
	/*SaliencyMap* s = new MotionSaliencyMap( "./EXR/sb-tunnel-exr/00000.exr" ,false );
	Matrix* sm = s->getSaliencyMap();
	delete sm;
	SaliencyMap* s1 = new MotionSaliencyMap( "./EXR/sb-tunnel-exr/00001.exr" ,true );
	sm = s1->getSaliencyMap();
	s1->displayMap(*sm ,"./results.jpg" );
	delete sm;
	delete s;
	delete s1;*/

	/*SaliencyMap *s = new SaliencyMap("./EXR/sun-hdrframes/sun-hdrframes/000.exr" );
	Matrix* sm = s->getSaliencyMap();
	s->displayMap(*sm ,"originals.jpg" );
	delete s;
	delete sm;*/

	//double a[8] = {1 ,2 ,3 ,4, 5, 6, 7, 8};
	//CNumberArray data(4 ,a);
	//CNumberArray data3;
	//CNumberArray data2 = data;
	//data3 = data2;
	////data3.resetSize(4 );
	//cout << "data2 = "<< data2 <<"\n";
	//cout << "data3 = "<< data3 <<"\n";
	//data3 = data2[2] * data3;
	//cout <<data3 <<"\n";

	/*int row = 3 ,col = 2;
	double* * d = create2DArray(row ,col ,2 );
	CNumberMatrix c(d ,row ,col );
	CNumberMatrix c2 (c);
	cout << c2;*/
	
	//double a[8] = {1 ,2 ,3 ,4 , 5, 6, 7, 8};
	
	//CNumberArray A (6 , a );
	//cout << A<<"\n";
	/*A.setData(0 ,30 ,0 );
	A.setData(1 ,-1 ,-1 );
	A.setData(2 ,0 ,0 );
	A.setData(3 ,-1 ,1 );*/
	//cout <<A<<" 之 Fourier Transformation\n";
	//CNumberArray B = FFT::FFT_1D(A );
	//cout << B<<"\n";
	//cout << A <<" 之 inverse Fourier Transformation\n";
	//cout << FFT::iFFT_1D(B)<<"\n";
	
	/*int row = 4 ,col = 5;
	double* * d = create2DArray(row ,col ,2 );
	d[0][0] = 1; d[1][1] = 4; 
	GaussianPyramid gp( d ,row ,col ,6 );
	cout << *gp.getLevelData( 3 );

	cout << gp;*/
	//CNumberMatrix D(row ,col ,d );
	//D[1].setData(2 ,3 ,4 );
	////c.resetSize(5 ,5 );
	//cout << FFT::FFT_2D(c );
	
	/*GaussianKernel k;
	cout << k;
	cout << "============\n";*/
	/*double* * r = Filter::GaussianFilter(d ,row ,col ,k );

	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ )
			cout << r[i][j] <<" ";
		cout <<"\n";
	}*/
	//cout << D;
	//D.resetRow(3);
	//cout << D;
	//D.resetCol(4);
	//cout << D;
	//D.resetSize(row ,col );
	/*cout << "D:\n"<< D;
	
	
	CNumberMatrix E( row ,col ,d );
	ComplexNumber c(1 ,2 );
	E[0][1] = c;A 
	cout <<"E:\n"<< E;

	cout << "D * E:\n" << D * E;*/
	//cout<<"轉換開始\n";
	//CNumberMatrix FD = FFT::FFT_2D(D );
	//////cout << FD;
	//cout<<"轉換結束\n";
	//
	//cout << FFT::iFFT_2D( FD );
	//cout<<"==============\n";
	//cout <<"inverse:\n"<< FFT::iFFT_2D(FD );
	
	/*Tone adjustment測試*/
	//ToneAdjustment ta("./image/bathroom.hdr" ,1 ,300 );
	//ToneAdjustment ta("./image/vinesunset.hdr" ,1 ,300 );
	
	//ta.reduceImage( 64 ,45 );
	//ta.runAlgorithm( 22 ,34 );
	//ta.getImage().saveImage("test image.jpg");
	/*HDR Loader測試*/
	//HDRLoader image = ta.getImage();
	//image.saveImage("test1.jpg");
	//HDRLoader loader("./image/grace_new_cross.hdr");
	//HDRLoader loader("./image/bathroom.hdr");
	//HDRLoader loader("./image/result.hdr");
	//HDRLoader loader("./image/als_chair.hdr");
	//int x = 140 ,y = 215;
	//cout << loader.getR(x,y) <<" \n";
	//cout << loader.getG(x,y) <<"\n";
	//cout << loader.getB(x,y) <<"\n";
	//ColorSpaceTransformation* cs = loader.headinfo.rgb2xyz;
	

	/*顏色轉換測試*/
	
	/*double a[3] ={91.5 ,91.5 ,91.5 };
	Vector rgb(3 , a );
	cout <<"RGB:"<< rgb;
	Vector lab = cs->transform(rgb);
	cout <<"LAB:"<<lab;
	lab[0] = 300;
	bool y = false;
	cout << cs->inverseTransform(lab ,y);*/
	//double a[3] = {0};
	//
	////a[0] = loader.getL(0 ,0); a[1] =loader.getA(0 ,0); a[2] = loader.getB(0 ,0);
	//bool yy = false;
	////Vector lab(3 , a );
	////cout <<"LAB:"<< lab;
	//cout << "XYZ:"<<loader.getX(x,y) <<" " << loader.getY(x,y) <<" "<<loader.getZ(x,y) <<"\n";
	//////Vector d = cs->inverseTransform( lab,y);
	//a[0] = loader.getR(x,y); a[1] = loader.getG(x,y); a[2] = loader.getB(x,y);
	//Vector rgb(3 ,a );
	//cout << rgb;
	//Vector d = cs->transform(rgb);
	//////
	//cout << "RGB->LAB:" << cs->transform(rgb);
	//cout << "LAB->RGB:"<< cs->inverseTransform(d ,yy);
	

	
	/*Matrix測試*/

	//double* * d = create2DArray(3 ,3 ,2.0);
	//cout << d[0][0]<<"\n";
	//d[0][0] = 4; d[1][1] = -0.9; d[2][1] = 9.9;
	
	//Matrix m(d ,3 ,3 );
	//Matrix m2 = m;
	//cout << m<<"\n";

	//Histogram h(d ,3 ,3 ,3 );

	//cout << h <<"\n";
	//m.scaleRow(0 ,4 );
	//cout << m <<"\n";
	//cout << m.determinant()<<"\n";
	//cout << m.isInvertible()<<"\n";
	//cout << m.getInverseMatrix() <<"\n";


	//d[1][1] = 4; d[2][2] = 1;
	//Matrix m2(d ,3 ,3);
	//cout << m2 <<"\n";
	//Matrix m3 = m * m2;
	//cout << m3 <<"\n";
	//m *= m2;
	//cout << m <<"\n";*/
	//for ( int i = 0; i < 3; i++ )
	//	cout << m.getOneCol(2)[i]<<" ";
	//cout<<"\n";
	//for ( int i = 0; i < 3; i++ )
	//	cout << m.getOneRow(1)[i]<<" ";
	//for ( int i = 0; i < 3; i++ )
	//	cout << m.multiplyX( d[0] ,3 )[i]<<" ";

	return 0;
}

	