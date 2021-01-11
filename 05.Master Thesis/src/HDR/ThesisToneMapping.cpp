/******************************************
FILE
    ThesisToneMapping.h
PURPOSE
    this define the tone mapping of my thesis.
	I hope it'll work in order to graduate.

NOTE
	
	

AUTHOR
    9757553 顏志晟
*******************************************/

#include "ThesisToneMapping.h"
#include "Essential/Filter.h"
#include "Essential/Kernel.h"
#include "MotionSaliencyMap.h"
#include "./Loader/HDRLoader.h"
#include "./Loader/EXRLoader.h"
#include "./Loader/IMGLoader.h"
#include <iostream>
#include <vector>
#include <algorithm>

using std::cout;
using std::vector;

ThesisToneMapping::ThesisToneMapping( const string& fname ,bool isvideo ,bool islastframe ) : filename(fname){

	cout << "開始處理檔案：" << fname <<"\n";
	if ( fname.find( ".hdr" ) != string::npos )
		image = new HDRLoader( fname );
	else if( fname.find( ".exr" ) != string::npos )
		image = new EXRLoader( fname );
	else if( fname.find( ".jpg" ) != string::npos )
		image = new IMGLoader( fname );
	isforvideo = isvideo;

	if ( isforvideo == false )
		saliencemap = new SaliencyMap( fname );
	else saliencemap = new MotionSaliencyMap( fname ,islastframe);
	ld = NULL;
	ap = NULL;
}
ThesisToneMapping::ThesisToneMapping( ThesisToneMapping& t ) {

	if ( t.isforvideo == false )
		saliencemap = new SaliencyMap( *t.saliencemap );
	else saliencemap = new MotionSaliencyMap( *t.saliencemap );
	
	if ( t.image != NULL ) {
		if ( t.filename.find( ".hdr" ) != string::npos ) {
			HDRLoader* tmp = (HDRLoader*) t.image;
			image = new HDRLoader( *tmp );
		}
		else if( t.filename.find( ".exr" ) != string::npos ) {
			EXRLoader* tmp = (EXRLoader*) t.image;
			image = new EXRLoader( *tmp );
		}
	}

	ld = new Matrix( *t.ld);
	
	ap = NULL;

}
ThesisToneMapping& 
ThesisToneMapping::operator= (const ThesisToneMapping& t ) {

	/*刪掉自己本身的指標配置的記憶體*/
	if ( saliencemap != NULL )
		delete saliencemap;
	if ( image != NULL ) {
		delete image;
	}

	if ( t.saliencemap != NULL )
		saliencemap = new SaliencyMap( *t.saliencemap );

	if ( t.image != NULL ) {
		if ( t.filename.find( ".hdr" ) != string::npos ) {
			HDRLoader* tmp = (HDRLoader*) t.image;
			image = new HDRLoader( *tmp );
		}
		else if( t.filename.find( ".exr" ) != string::npos ) {
			EXRLoader* tmp = (EXRLoader*) t.image;
			image = new EXRLoader( *tmp );
		}
	}

	ld = new Matrix( *t.ld );

	ap = NULL;

	return *this;

}
ThesisToneMapping::~ThesisToneMapping() {

	int row = image->getHeight();
	int col = image->getWidth();

	if ( saliencemap != NULL )
		delete saliencemap;
	if ( image != NULL )
		delete image;
	if ( ap != NULL )
		dispose( ap ,row ,col );
	if ( ld != NULL )
		delete ld;
	
}
/*It 'll perform my tone mapping It'll change the value of the HDRLoader */
float getAttentionEffect( const bool* * const bsm ,int row ,int col ,int x ,int y);

#using <System.Drawing.dll>
#using <mscorlib.dll>
using namespace System::Drawing;
using namespace System;

void 
ThesisToneMapping::perform( const Matrix* ratio ) {

	cout << "ThesisToneMapping:開始跑我的演算法\n";
	int row = image->getHeight();
	int col = image->getWidth();
	//cout << "MaxL=" << image->getMaxL() << " ,MinL=" << image->getMinL() << "\n";

	/*
	  1.算出整張圖各個位置的Contrast
	    並找出salience value的平均
	*/
	cout << "ThesisToneMapping:找每一個pixel的Contrast\n";
	Matrix C( row ,col );//Contrast
	
	//saliencemap->displayMap( *sm ,"./SalienceMap.jpg");
	double sav = 0;//salience value的平均
	double lwbar = image->getAvgL();
	double lwmax = Double::MinValue;
	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j< col; j++ ) {
			double lw = image->getLuminance( j ,i );
			//C.setData( i ,j , (lw - lwbar) / lwbar );
			double tmp = 0.18 * lw / lwbar;
			//C.setData( i ,j , 0.18 *(lw ) / lwbar );
			C.setData( i ,j , (lw ) / lwbar );

			if ( tmp > lwmax ) lwmax = tmp;
			
		}
	}
	
	lwmax *= lwmax;
	/*
	  2.透過
	            C
	    C = --------- 來調整產生新的Contrast
              1 + C
	    需要注意的是 他會跟隨著salience map進行處理
		如何決定是否越Salience
		我們是透過和平均做比較來做決定
	*/

	cout << "ThesisToneMapping:根據每一個pixel的salience值調整Contrast\n";
	if ( ap == NULL )
		ap = getAttentionEffect();

	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			double contrast = C.getData(i ,j);
			
			//double contrast = contrast /( 1 + contrast );
			
			/*double contrast1 = contrast + 0.02;
			contrast1 /= ( 1 + contrast1 );*/
			/*
			       C
			   --------- 
			     N + C

			   Version 2010/06/09：這邊經過數度改變 目前為Sustained Attention的效果
			                       而我們視Sustained Attention為Adaptation 所以他狀況目前不會變
			   Version 2010/07/01: N = 2
			   Version 2010/07/14: N = 12
			*/
			//double y = -0.6 * ap[i][j] + 1.4;
			double SA = contrast / (12 + contrast );
			//contrast1 += 0.01 * ap[i][j];
			//contrast1 -= 0.02;
			/*
			       N*C
			   --------- 
			     1 + N*C


			  透過linear interpolation插出C*N應該要的N值
			  -1為Unattended 1為Attended
			  這邊可以視為Sustained Attention
			  Version 1.1:這邊因為想要改成Contrast Adaptation 的效果 所以就跟Attention無關
			  Version 2010/06/09：根據一篇paper 正式將這邊定名為Transient Attention的效果

				d - 1.2     1.2 - 0.8
			  ---------- = -----------
			    x -  1        1  - (-1)
			*/
			//double contrast2 = contrast * 4;
			//double contrast2 = contrast * ( -0.05*ap[i][j]+0.15 );
			//double TA = contrast * ( 0.05*ap[i][j]+0.15 );
			double TA = contrast * (0.2*ap[i][j]+1);
			//double TA = contrast * 0.8;
			//double contrast2 = contrast* 0.15;
			//double contrast2 = contrast * 0.1;
			//double TA = 1.2 * contrast;
			TA /= ( 1 + contrast );
			//contrast2 *= ( 1 + contrast / lwmax );

			/*
			       C
			   ---------   N = 2 .. 5
			     N + C
			*/
			//double scale = 1.5 * ap[i][j] + 3.5;
			//double contrast1 = contrast / (5 + contrast );
			//double contrast1 = contrast / (scale + contrast );
			//contrast2 += 0.01;
			//contrast = (contrast1 +  2 * contrast2) /3;
			//contrast = (contrast1 + 4 * contrast2) / 5;
			//contrast =  contrast / ( 2 + contrast);
			//contrast += ap[i][j]*0.01;
			//contrast += 0.1;

			/*
			  Reinhard's global operator
			*/
			//double contrast2 = contrast / ( 1 + contrast );
			//contrast2 = contrast2 * ( 1 + contrast / lwmax );

			//if ( ratio == NULL )
			if ( isforvideo == false )
				//contrast = (0.8 * TA + SA) / 1.8;
				//contrast = (0.8 * TA + SA);
				//contrast = 0.5 * TA + 0.5 * SA;
				contrast = (  TA + 2 * SA ) / 3;
			else {
				/*透過
				   X - (0)       (0) - 1         1
				  ---------- = ------------ = ------
				   Y -  1         1   - 2       1
				  找出SA的比例
				  利用
				   X - (0)       (0) - 1       -1
				  ---------- = ------------ = ------
				   Y -  2         2   - 1       1
				  找出TA的比例
				  並透過比例 找出正確的Contrast

				  Version 2010/07/18：Attention 越高 TA的比例應該越大 如果Attention越低 TA比例應該越小

				*/
				//double r = ratio->getData( i ,j );
				double r = ap[i][j];
				//double r = 0.064437;
				double r1 = (1.0/6) * r + 0.5;
				contrast = r1 * TA + ( 1 - r1 ) * SA;
				
				/*if ( i == 82 && j==266 )
					int h = 0;*/
				//if ( r > 0 ) {
				//	//double r1 = 0.5 * r + 1.5;
				//	double r1 = - r + 2;
				//	//double r2 = -0.5 * r + 1.5;
				//	double r2 = r + 1;
				//	contrast = ( r2* TA + r1 * SA ) /3;
				//	//Version:2010/07/31
				//	//double r1 = 1/6 * r + 0.5;
				//	//contrast = r1 * TA + ( 1 - r1 ) * SA;
				//	
				//}else {
				//	contrast = (TA +  2 * SA) / 3;
				//}
				
				/*double r = ratio->getData( i ,j );
				r = abs( r );
				contrast =  (r * TA + SA) /(1 + r ) ;*/
				//contrast = r * contrast1 + 2 * contrast2;
				//contrast /= (2 + r );
				//if ( r == 0 )
				//	contrast = 0;
				//if ( r > 0 ) {//對注意到的時候
				//	contrast = r * contrast1 +  ( 1 - r ) * contrast2;
				//	contrast /= (2 - r );
				//}
				//else {//非注意到的其Contrast值調整
				//	r = abs(r);
				//	contrast = ( 1 - r ) * contrast1 +  r * contrast2;
				//}
				
				
			}
			//contrast = (contrast1 +  5 * contrast2) /6;

			
			//contrast2 += 0.01 * ap[i][j];
			//SA *= ( 1 + SA / lwmax );

			C.setData( i ,j ,contrast );	
			
		}
	}
	
	/*
	  3.找出Ld的值 
	*/
	ld = new Matrix( C );
	
}
Matrix* 
ThesisToneMapping:: getAttentionMap() {
	if ( ap == NULL ) 
		ap = getAttentionEffect();

	return new Matrix( ap ,image->getHeight() ,image->getWidth() );
}
static String^
str2String( const string& ss)
{
	if (ss.empty())
		return gcnew System::String("");

	IntPtr ptr(static_cast<System::IntPtr>(static_cast<void*> (const_cast<char*>(ss.c_str()))));

	String^ ret(System::Runtime::InteropServices::Marshal::PtrToStringAnsi(ptr));

	return ret;
}
void
ThesisToneMapping:: saveResult( string fname ) {

	if ( ld == NULL ) 
		throw "ThesisToneMapping error : ld is not assigned. please  perform this algorithm first";

	cout << "ThesisToneMapping:圖檔寫出\n";

	int width = image->getWidth();
	int height = image->getHeight();

	Matrix* R = new Matrix( height ,width );
	Matrix* G = new Matrix( height ,width );
	Matrix* B = new Matrix( height ,width );
	Matrix* L = new Matrix( height ,width );
	
	for ( int i = 0; i < height; i++ ) {
		for ( int j = 0; j < width; j++ ) {
			
			double lw = image->getLuminance( j ,i );
			double s = ld->getData( i ,j ) / lw;
			
			R->setData( i ,j ,image->getRed( j ,i ) * s );
			G->setData( i ,j ,image->getGreen( j ,i ) * s );
			B->setData( i ,j ,image->getBlue( j ,i ) * s );
			L->setData( i ,j ,lw * s );
		}
	}
	
	applyGammaAndNormalize(R ,G ,B ,L);
	
	Bitmap output( width ,height );
	for ( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			
     		int r = int( R->getData( y ,x ) * 255 );
			if ( r > 255 ) r = 255;
			else if ( r < 0 ) r = 0;
			int g = int( G->getData( y ,x ) * 255 );
			if ( g > 255 ) g = 255;
			else if ( g < 0 ) g = 0;
			int b = int( B->getData( y ,x ) * 255 );
			if ( b > 255 ) b = 255;
			else if ( b < 0 ) b = 0;
			Color c = Color::FromArgb( r ,g ,b );

			output.SetPixel( x , y ,c );
			
		}
	}
	
	String^ filename = str2String( fname );
	
	output.Save( filename );
	
	delete R;
	delete G;
	delete B;
	delete L;
}
/*
 *It'll get the scale value which the contrast need to add
*/

float* * 
ThesisToneMapping::getAttentionEffect() {

	Matrix* sm = saliencemap->getSaliencyMap();
	int row = sm->getRowCount();
	int col = sm->getColCount();

	/*Video上面會有問題*/
	//if ( isforvideo == false ) {
	//	//Version:20100730
	//	/*Matrix s( row ,col);
	//	for ( int i = 0; i < row;i++ ) {
	//		for ( int j = 0; j < col; j++ ) {
	//			double d = sm->getData(i, j);
	//			if ( d >= 0 )
	//				s.setData(i ,j , 255);
	//			else s.setData(i ,j ,128);
	//		}
	//	}
	//	int firstindex = filename.rfind( "\\" );
	//	int finalindex = filename.rfind( "." );
	//	string dest = "./";
	//	dest += filename.substr( firstindex+1 ,finalindex - firstindex - 1 );
	//	dest +="s.jpg";
	//	saliencemap->displayMap(s ,dest);*/

	//	float* * data = sm->getDatas();
	//	delete sm;
	//	return data;
	//}
	//else {
	//	//Version:20100818 想方設法解決Flicker
	//	//float* * data = sm->getDatas();
	//	//delete sm;
	//	//BoxKernel k( 31 ,31 );
	//	////GaussianKernel k(101 ,101 ,16);
	//	//float* * result = Filter::BasicFiltering( data ,row ,col ,&k );
	//	//dispose( data ,row ,col );

	//	//return result;

	//	float* * data = sm->getDatas();
	//	delete sm;
	//	return data;
	//}
	/*Video上面會有問題*/
	/*
	 *找到Attention處 這是透過將salience map砍threshold的方式得到
	*/
	//初始化資料
	float* *apart = create2DArray(row ,col ,0);

	//找出salience 平均 平均以上的都當作是Attention部分
	//double sav = 0;//salience value的平均
	//for ( int i = 0; i < row; i++ ) 
	//	for ( int j = 0; j< col; j++ ) 
	//		sav += sm->getData( i ,j );	
	//sav /= ( row * col );
	//找出四分位數
	double* s = new double[ row * col ];
	int i = 0;
	for ( int j = 0; j < row; j++ ) 
		for ( int k = 0; k < col; k++ )
			s[i++] = sm->getData( j ,k );
	vector<double> d( s , s+i );
	sort( d.begin() ,d.end() );
	double q1 = d[ row*col / 4 ];
	double q3 = d[ row*col*3/4 ];
	delete [] s;
	
	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			//if ( sm->getData( i ,j ) > sav )
			if ( sm->getData( i ,j ) < q1 )//四分位數 先暫時用這蠢法 希望能有很結果
				apart[i][j] = -1;
				//apart[i][j] = 128;
			else if ( sm->getData( i ,j ) > q3 )
				apart[i][j] = 1;
				//apart[i][j] = 255;
		}
	}
	

	/*for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			if ( i < row / 2 )
				ap[i][j] = 1;
			else ap[i][j] = -1;
		}
	}*/
	//Matrix m(apart ,row ,col );
	//saliencemap->displayMap(*sm ,"./saliencemap.jpg");
	//for ( int i = 0; i < row; i++ ) {
	//	for ( int j = 0; j < col; j++ ) {
	//		if ( m.getData( i ,j ) == 1 ) 
	//			m.setData( i ,j ,255 );
	//		else if ( m.getData( i ,j ) == -1 )
	//			m.setData( i ,j ,128 );
	//	}
	//}
	/*int firstindex = filename.rfind( "\\" );
	int finalindex = filename.rfind( "." );
	string dest = "./EXR Video/Salience/";
	dest += filename.substr( firstindex+1 ,finalindex - firstindex - 1 );
	dest +="s.jpg";
	saliencemap->displayMap(m ,dest);*/
	//saliencemap->displayMap(m ,"attention.jpg");
	////Version:20100730
	//float* * data = sm->getDatas();
	delete sm;
	//return data;

	//return ap;//跳掉程式用用 請注意
	/*
	 *讓前面得到的Attention part去和一個Kernel為全是1的進行Convolution得到
	  漂亮的結果
	*/
	BoxKernel k( 101 ,101 );
	//GaussianKernel k(51 ,51 ,8);
	//GaussianKernel k(101 ,101 ,16);
	//GaussianKernel k(31 ,31 ,5);
	float* * result = Filter::BasicFiltering( apart ,row ,col ,&k );
	
	//cout << result[13][13];
	/*for ( int i = 0; i < row; i++ ) 
		for ( int j = 0; j < col; j++ )*/
			//result[i][j] *= 0.01 ;
			//result[i][j] *= 0.02 ;
	
	/*{
		Matrix m( result ,row ,col );
		cout << result[13][13];
		saliencemap->displayMap( m ,"saliencemapr.jpg");
	}*/

	dispose( apart ,row ,col );

	return result;

}
/*normalize the matrix*/
Matrix* 
ThesisToneMapping::normalize( Matrix* m  ,double max ,double min ) {
	
	int row = m->getRowCount();
	int col = m->getColCount();

	

	double range = max - min;
	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			double val = m->getData( i , j );
			val = ( val - min ) / range;

			m->setData( i ,j ,val );
		}
	}

	return m;
}
/*apply gamma correction*/
void 
ThesisToneMapping::gamma( Matrix* r ,Matrix* g ,Matrix* b ) {
	
	int row = r->getRowCount();
	int col = r->getColCount();

	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			//r
			double val = r->getData( i ,j );
			val = Math::Pow( val ,0.5 );
			r->setData(i ,j ,val );
			//g
			val = g->getData( i ,j );
			val = Math::Pow( val ,0.5);
			g->setData(i ,j ,val );
			//b
			val = b->getData( i ,j );
			val = Math::Pow( val ,0.5 );
			b->setData(i ,j ,val );
		}
	}

	
}

void 
ThesisToneMapping::applyGammaAndNormalize( Matrix* r ,Matrix* g ,Matrix* b  ,const Matrix* l) {
	
	int row = l->getRowCount();
	int col = l->getColCount();
	/*找到最大最小*/
	double max = System::Double::MinValue;
	double min = System::Double::MaxValue;
	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			double val = l->getData( i ,j );
			if ( max < val ) max = val;
			if ( min > val ) min = val;
		}
	}
	normalize( r ,max ,min );
	normalize( g ,max ,min );
	normalize( b ,max ,min );
	gamma( r ,g ,b );
	normalize( r ,max ,min );
	normalize( g ,max ,min );
	normalize( b ,max ,min );

	
}