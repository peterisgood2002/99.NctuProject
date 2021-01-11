/******************************************
FILE
    SaliencyMap.h
PURPOSE
    this class will do the Salience map operation in
	"a model of Saliecny-Based Visual Attention for Rapid Scence Analysis"

NOTE
	It first do gaussian pyramids to image and separate an image to colors ,intensity and orientation.
	Compute each Feature map and combine to Conspicuity Map
	Then perform linear combinations to obtain Salience Map

AUTHOR
    9757553 顏志晟
*******************************************/

#include "SaliencyMap.h"
#include "./Loader/HDRLoader.h"
#include "./Loader/EXRLoader.h"
#include "./Essential/GaussianPyramid.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include "exception"

#using <System.Drawing.dll>
#using <mscorlib.dll>


using namespace System::Drawing;
using namespace System;
using std::cout;

static String^
str2String( const string& ss)
{
	if (ss.empty())
		return gcnew System::String("");

	IntPtr ptr(static_cast<System::IntPtr>(static_cast<void*> (const_cast<char*>(ss.c_str()))));

	String^ ret(System::Runtime::InteropServices::Marshal::PtrToStringAnsi(ptr));

	return ret;
}

SaliencyMap::SaliencyMap( const string& fname) {
	
	setRGB( fname );
}

SaliencyMap::SaliencyMap (const SaliencyMap& v ) {
	init ( v.row ,v.col );

	for ( int i = 0; i < row ; i++ )
		for ( int j = 0; j < col; j++ )
			rgb[i][j] = v.rgb[i][j];


}
SaliencyMap::~SaliencyMap() {
	cout << "刪除SaliencyMap之物件\n";

	dispose();
}
SaliencyMap& SaliencyMap::operator= (const SaliencyMap& v ) {
	dispose();

	init ( v.row ,v.col );

	for ( int i = 0; i < row ; i++ )
		for ( int j = 0; j < col; j++ )
			rgb[i][j] = v.rgb[i][j];

	return *this;
}
/*Perform the center-surround differences and normalization*/
void 
SaliencyMap::doIntensityFeatureMap( const GaussianKernel* k  ) {

	cout << "SaliencyMap:取得 feature map of intensity\n";

	if ( intensityfeaturemap == NULL )
		intensityfeaturemap = new Matrix [ ifmsize ];
	
	if ( GI == NULL ) 
		makeIntensityPyramid( k );
	
	
	//std::cout << *GI->getLevelData(7);
	int i = 0;
	for ( int c = 2; c <= 4; c++ ) {
		Matrix* Ic = GI -> getLevelData( c );
		if ( Ic == NULL ) {//Feature map如果產生不出來 就停止再產生了
			ifmsize = i++;
			break;
		}
		

		for ( int delta = 3; delta <= 4; delta++ ) {
			Matrix* tmpIs = GI -> getLevelData( c+delta );
			
			if ( tmpIs == NULL ) {
				ifmsize = i++;
				break;
			}
			
			/*將最上層的進行內插到比較下層 也就是將粗糙的內插到比較精緻的*/
			Matrix* Is = interpolationFromCoarseToFine( *tmpIs ,Ic->getRowCount() ,Ic->getColCount() );

			intensityfeaturemap[i] = (*Ic) - (*Is) ;

			ABS( intensityfeaturemap[i] );//取絕對值

			if ( ishdr ) {
				/*
				  進行 "salience map of HDR image的algorithm
				  就是取contrast
				*/
				int row = Ic->getRowCount();
				int col = Ic->getColCount();

				for ( int j = 0; j < row; j++ ) {
					for ( int l = 0; l < col; l++ ) {
						double d = Is->getData( j , l );
						double I = intensityfeaturemap[i].getData( j ,l ) / d;

						intensityfeaturemap[i].setData( j ,l ,I );
					}
				}

			}

			//std::cout << intensityfeaturemap[i];
			//std::cout <<"=====================\n";
			performNormalization( intensityfeaturemap[i] );
			//std::cout << intensityfeaturemap[i];
			//std::cout <<"=====================\n";

			i++;

			delete Is;
			delete tmpIs;
		}

		delete Ic;
	}
			
	cout << "取得完畢\n";
}
void 
SaliencyMap::doColorfeatureMap( const GaussianKernel* k ) {
	
	cout << "SaliencyMap:取得feature map of color\n";
	
	if ( rgfeaturemap == NULL )
		rgfeaturemap = new Matrix[ cfmsize ];
	if ( byfeaturemap == NULL )
		byfeaturemap = new Matrix[ cfmsize ];
	/*準備RGBY的color matrix*/
	Matrix R( row ,col );
	Matrix G( row ,col );
	Matrix B( row ,col );
	Matrix Y( row ,col );

	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			RGB color = rgb[i][j];
			R.setData( i ,j ,color.R - ( color.G + color.B )/2 );
			G.setData( i ,j ,color.G - ( color.R + color.B )/2 );
			B.setData( i ,j ,color.B - ( color.R + color.G )/2 );
			Y.setData( i ,j ,( color.R + color.G )/2 - abs( color.R - color.G )/2 - color.B );
		}
	}

	/*準備Gaussian Pyramids*/
	GaussianPyramid GR( R ,9 ,k );
	GaussianPyramid GG( G ,9 ,k );
	GaussianPyramid GB( B ,9 ,k );
	GaussianPyramid GY( Y ,9 ,k );

	int i = 0;
	
	for ( int c = 2; c <= 4; c++ ) {
		Matrix* Rc = GR.getLevelData( c );
		Matrix* Gc = GG.getLevelData( c );
		Matrix* Bc = GB.getLevelData( c );
		Matrix* Yc = GY.getLevelData( c );

		if ( Rc == NULL && Gc == NULL && Bc == NULL && Yc == NULL ) {//Feature map如果產生不出來 就停止再產生了
			ifmsize = i++;
			break;
		}
		
		Matrix RGc = (*Rc) - (*Gc);
		Matrix BYc = (*Bc) - (*Yc);

		for ( int delta = 3; delta <= 4; delta++ ) {
			Matrix* tmpRs = GR.getLevelData( c+delta );
			Matrix* tmpGs = GG.getLevelData( c+delta );
			Matrix* tmpBs = GB.getLevelData( c+delta );
			Matrix* tmpYs = GY.getLevelData( c+delta );
			
			if ( tmpRs == NULL && tmpGs == NULL && tmpBs == NULL && tmpYs == NULL ) {
				ifmsize = i++;
				break;
			}

			Matrix tmpRGs = (*tmpRs) - (*tmpGs);
			Matrix tmpBYs = (*tmpBs) - (*tmpYs);

			Matrix* RGs = interpolationFromCoarseToFine( tmpRGs ,RGc.getRowCount() ,RGc.getColCount() );
			Matrix* BYs = interpolationFromCoarseToFine( tmpBYs ,BYc.getRowCount() ,BYc.getColCount() );
			
			rgfeaturemap[i] = RGc - (*RGs);
			byfeaturemap[i] = BYc - (*BYs);

			ABS( rgfeaturemap[i] );
			ABS( byfeaturemap[i] );
			performNormalization( rgfeaturemap[i] );
			performNormalization( byfeaturemap[i] );

			i++;

			delete RGs;
			delete BYs;
			delete tmpRs;
			delete tmpGs;
			delete tmpBs;
			delete tmpYs;
		}

		delete Rc;
		delete Gc;
		delete Bc;
		delete Yc;
	}

	cout << "取得完畢\n";
}
void 
SaliencyMap::doOrientationFeatureMap( const GaussianKernel* k ){
	
	cout << "SaliencyMap:取得 feature map of orientation\n";
	
	if ( orientationfeaturemap == NULL )
		orientationfeaturemap = new Matrix[ ofmsize ];

	if ( GI == NULL ) {//如果沒有intensity pyramids 就製造一個
		makeIntensityPyramid( k );
	}


	/*1.製作四個Garbor Kernel 以方便後面使用*/
	GaborKernel* g[4];
	for ( int i = 0; i < 4; i++ )
		g[i] = new GaborKernel( i * 45 );//製造0 45 90 135的Gabor Kernel

	/*根據上面的Gabor Kernel去filter 產生 24張orientation
	  放法是 如下

	  c2		    c3				c4
	  ----------------------------------------------
	  |				|				|				|
	  |s5	s6		|s6		s7		|s7		s8		|
	  ----------------------------------------------
	  |		|		|		|		|		|		|
      ----------------------------------------------
	*/
	int io = 0;//orientation feature map之index
	for ( int c = 2; c <= 4; c++ ) {
		Matrix* Ic = GI->getLevelData( c );
		if ( Ic == NULL ) {
			ofmsize = io++;
			break;
		}
		
		for ( int delta = 3; delta <=4; delta++ ) { 
			Matrix* Is = GI->getLevelData( c+delta );
			if ( Is == NULL ) {
				ofmsize = io++;
				break;
			}
			
			/*根據Gabor Kernel取出各自的O(c,theta)與O(s ,theta ) 並進行相減動作*/
			for ( int theta = 0; theta < 4; theta++ ) {
				//取O(c ,theta )
				Matrix Oct = performGaborFiltering( Ic ,g[theta] );

				if ( !ishdr ) {
					/*普通圖做的事情
					O( c ,s ,t ) = | O(c,t) - O(s,t) |
					
					*/
					//取O(s ,theta )
					Matrix tmpOst = performGaborFiltering( Is ,g[theta] );
					/*將最上層的進行內插到比較下層 也就是將粗糙的內插到比較精緻的*/
					Matrix* Ost = interpolationFromCoarseToFine( tmpOst ,Ic->getRowCount() ,Ic->getColCount() );
					orientationfeaturemap[io] = Oct - (*Ost) ;
					ABS( orientationfeaturemap[io] );//取絕對值
					
					delete Ost;

				}else{
					/*
					  進行 "salience map of HDR image的algorithm
					  就是取contrast

									O(c ,t)
					  O'(c,s,t) = --------------
									  I(s)
					*/
					orientationfeaturemap[io] = Oct;
					
					int row = Ic->getRowCount();
					int col = Ic->getColCount();

					Matrix* tmpIs = interpolationFromCoarseToFine( *Is ,row ,col );

					for ( int j = 0; j < row; j++ ) {
						for ( int l = 0; l < col;l++ ) {
							double d = tmpIs->getData( j , l );
							double I = orientationfeaturemap[io].getData( j ,l ) / d;

							orientationfeaturemap[io].setData( j ,l ,I );
						}
					}

					delete tmpIs;
				}
				performNormalization( orientationfeaturemap[io] );

				io++;

				
			
			}

			delete Is;

		}
		
		delete Ic;
		
	}

	for ( int i = 0; i < 4; i++ )
		delete g[i];
	cout << "取得完畢\n";
}

/*Perform the across-scale combination and normalization to obtain Conspicuity map*/
Matrix* 
SaliencyMap::getIConspicuityMap() {//Conspicuity map of intensity
	
	/*check有沒有做過Feature map*/
	if ( intensityfeaturemap == NULL )
		doIntensityFeatureMap();

	cout << "SaliencyMap:取得conspicuity map of intensity\n";

	int row = intensityfeaturemap[ ifmsize - 1 ].getRowCount();//所有資料都downsample到最後一個好加加
	int col = intensityfeaturemap[ ifmsize - 1 ].getColCount();

	/*將全部downsample並加起來就對了*/
	Matrix* result = new Matrix( row ,col );
	for ( int i = 0; i < ifmsize; i++ ) {
		Matrix* I = getSubSample( intensityfeaturemap[i] ,row ,col );
		//std::cout << *I;
		*result += *I;

		delete I;
	}

	//std::cout << *result;
	//std::cout << "==================\n";
	performNormalization( *result );
	//std::cout << *result;

	cout << "取得完畢\n";

	return result;

	
}
Matrix* 
SaliencyMap::getCConspicuityMap() {//Conspicuity map of color

	/*check有沒有做過Feature map*/
	if ( rgfeaturemap == NULL || byfeaturemap == NULL )
		doColorfeatureMap();

	std::cout << "SaliencyMap:取得conspicuity map of color\n";

	int row = rgfeaturemap[ cfmsize - 1 ].getRowCount();//所有資料都downsample到最後一個好加加
	int col = rgfeaturemap[ cfmsize - 1 ].getColCount();
	
	Matrix* result = new Matrix( row ,col );
	for ( int i = 0; i < cfmsize; i++ ) {
		Matrix* RG = getSubSample( rgfeaturemap[i] ,row ,col );
		Matrix* BY = getSubSample( byfeaturemap[i] ,row ,col );
		*result += (*RG) + (*BY);

		delete RG;
		delete BY;
	}

	performNormalization( *result );

	cout << "取得完畢\n";

	return result;
}
Matrix* 
SaliencyMap::getOConspicuityMap() {//Conspicuity map of orientation

	/*check有沒有做過Feature map*/
	if ( orientationfeaturemap == NULL )
		doOrientationFeatureMap();

	cout << "SaliencyMap:取得conspicuity map of orientation\n";

	int row = orientationfeaturemap[ ofmsize - 1 ].getRowCount();//所有資料都downsample到最後一個好加加
	int col = orientationfeaturemap[ ofmsize - 1 ].getColCount();

	Matrix* result = new Matrix( row ,col );
	for ( int i = 0; i < ofmsize; i++ ) {
		Matrix* O = getSubSample( orientationfeaturemap[i] ,row ,col );

		*result += *O;

		delete O;
	}

	performNormalization( *result );

	cout << "取得完畢\n";

	return result;

}
/*Perform the salience map algorithm to get the salience map*/
Matrix* 
SaliencyMap::getSaliencyMap() {

	
	Matrix result = getOriginalSaliencyMap();

	result.scale( 1.0/3 );
	/*displayMap( result ,"SaliencyMap.jpg");*/
	Matrix* tmp = interpolationFromCoarseToFine( result ,row ,col );

	//cout << "取得完畢\n";
	//cout << "==================\n";
	//displayMap( *tmp ,"saliencymap.jpg");
	//cout << "進行Normalize的動作\n";
	//進行Normalize到[-1 .. 1] 幹!! 真的很煩
	normalize( *tmp );

	return tmp;

}
Matrix 
SaliencyMap::getOriginalSaliencyMap() {
	if ( intensityfeaturemap == NULL )
		doIntensityFeatureMap();
	if ( rgfeaturemap == NULL || byfeaturemap == NULL )
		doColorfeatureMap();
	if ( orientationfeaturemap == NULL )
		doOrientationFeatureMap();

	if ( intensitycm == NULL )
		intensitycm = getIConspicuityMap();
	if ( colorcm == NULL )
		colorcm = getCConspicuityMap();
	if ( orientationcm == NULL )
		orientationcm = getOConspicuityMap();
	
	cout << "SaliencyMap:取得原本大小的Salience map\n";
	Matrix result = *intensitycm + *colorcm + *orientationcm;

	return result;

}
/*Display map in order to debug*/
void
SaliencyMap::displayMap( const Matrix& m ,const string fname ) {
	cout << "SaliencyMap:畫出Salience map\n";

	int height = m.getRowCount();
	int width = m.getColCount();

	Bitmap output( width ,height );
	/*讓matrix map 到0 - 255 進行output
	  最大值是10 最小值是0
	*/
	for ( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			int d = (int) m.getData(y , x );
			Color c;
			if ( d >= 255 )
				c = Color::FromArgb( 255 ,255 ,255 );
			else c = Color::FromArgb( d ,d ,d );

			output.SetPixel( x , y ,c );
		}
	}

	String^ filename = str2String( fname );
	
	output.Save( filename );
}
/*Private method*/
void 
SaliencyMap::setRGB( const string& fname ) {
	cout << "SaliencyMap:讀入資料:" << fname << "\n";
	/*
	    --------------->x
		|
		|
		|
		|
		v
		y
	*/
	
	try {
		String^ str = str2String( fname );

		Bitmap image( str );
		init( image.Height , image.Width );

		for ( int y = 0; y < row; y++ ) {
			for ( int x = 0; x < col; x++ ) {
				Color c = image.GetPixel( x , y );
				
				rgb[y][x].R = c.R;
				rgb[y][x].G = c.G;
				rgb[y][x].B = c.B;
			}
		}

		ishdr = false;

	}catch ( System::ArgumentException^ ) {
		/*表示為HDR file*/

		ishdr = true;
		Loader* loader;
		if ( fname.find(".hdr") != string::npos ) 
			loader = new HDRLoader( fname );
		else if ( fname.find(".exr") != string::npos )
			loader= new EXRLoader ( fname );

		init( loader->getHeight() ,loader->getWidth() );

		for ( int y = 0; y < row; y++ ) {
			for ( int x = 0; x < col; x++ ) {
	
				rgb[y][x].R = loader->getRed( x , y );
				rgb[y][x].G = loader->getGreen( x , y );
				rgb[y][x].B = loader->getBlue( x ,y );
			}
		}

		delete loader;
		

	}

}
void 
SaliencyMap::init( int r , int c ) {
	row = r;
	col = c;

	rgb = new RGB*[ row ];
	for ( int i = 0; i < row; i++ ) {
		rgb[i] = new RGB[ col ];
		
		for ( int j = 0; j < col; j++ ) 
			rgb[i][j].R = rgb[i][j].G = rgb[i][j].B = 0;
	}

	/*feature map initailize*/
	cfmsize = 6;//rg + by = 12
	rgfeaturemap = NULL;
	byfeaturemap = NULL;
	
	ifmsize = 6;
	intensityfeaturemap = NULL;


	ofmsize = 24;
	orientationfeaturemap = NULL;

	/*conspicuity map initailize*/
	intensitycm = NULL;
    colorcm = NULL;
	orientationcm = NULL;

	GI = NULL;
	
}
void 
SaliencyMap::makeIntensityPyramid( const GaussianKernel* k ) {
	/*calculate intensity*/
	Matrix intensity ( row ,col );

	for ( int i = 0; i < row; i++ )  {
		for ( int j = 0; j < col; j++ ) {
			double I = ( rgb[i][j].R + rgb[i][j].G + rgb[i][j].B ) / 3;

			intensity.setData( i ,j ,I );
		}
	}
	/*get Gaussian Pyramid of intensity*/
	GI = new GaussianPyramid( intensity , 9 ,k);

}
void 
SaliencyMap::dispose() {
	if ( rgb != NULL ) {
		for ( int i = 0; i < row; i++ )
			delete [] rgb[i];

		delete [] rgb;
	}


	if ( rgfeaturemap != NULL )
		delete [] rgfeaturemap;
	if ( byfeaturemap != NULL )
		delete [] byfeaturemap;
	if ( intensityfeaturemap != NULL )
		delete [] intensityfeaturemap;
	if ( orientationfeaturemap != NULL )
		delete [] orientationfeaturemap;

	/*刪掉Conspicuity map*/
	if ( intensitycm != NULL )
		delete intensitycm;
	if ( colorcm != NULL )
		delete  colorcm;
	if ( orientationcm != NULL )
		delete orientationcm;

	if ( GI != NULL )
		delete GI;
}
/*
 *Interpolation the coarse matrix m into the fine matrix row * col
 *Parameter：the coarse matrix information and the finelevel's row and col
 *Return：new matrix
*/
Matrix* 
SaliencyMap::interpolationFromCoarseToFine(const Matrix& cm ,int row ,int col ) {
	/*先確保m的大小比row和col都還來得小*/
	assert( cm.getRowCount() < row && cm.getColCount() < col && row >= 0 && col >= 0 );	

	Matrix* result = new Matrix( row ,col );
	
	/*
	 *雖然SaliencyToolBox上面只是拿最近的點當作 該點的值
	  但paper上面提到內插 因此還是做到內插比較保險 在此我們將進行Bilinear interpolation

				vx + 0.5         x + 0.5
	  ==>   --------------- = --------------
				  cr			   row

	  
	 *取相對應值並插值
      (Lx ,Ly)           (Hx ,Ly )	 
		 -------------------
		|					|
		|		(x ,y )		|
		|					|
		|					|
		 -------------------
	  (Lx ,Hx)			 (Hx ,Hy )
	
	*/
	int cr = cm.getRowCount();
	int cc = cm.getColCount();
	for ( int y = 0; y < row; y++ ) {
		double vy = ( y + 0.5 ) * cr / row - 0.5 ;
		if ( vy < 0 ) vy = 0;
		else if ( vy >= cr )
			vy = cr - 1;

		int Ly = (int) vy;//低點
		
		int Hy = Ly + 1;
		if ( Hy >= cr ) Hy = cr - 1;//如果上界超過圖的話 就以最高邊界為準

		double fy = vy - Ly;//此點距離低點多少
		

		for ( int x = 0; x < col; x++ ) {
			if ( x == 4 && y == 3)
				int i = 5;

			double vx = ( x + 0.5 ) * cc / col - 0.5;
			if ( vx < 0 ) vx = 0;
			else if ( vx >= cc )
				vx = cc - 1;
			//超出的地方 就回到boundary處
			if ( vx < 0 )
				vx = 0;
			else if ( vx >= cc )
				vx = cc - 1;
			int Lx = (int) vx;
			if ( Lx < 0 ) Lx = 0;//超出的地方 就回到boundary處
			int Hx = Lx + 1;
			if ( Hx >= cc ) Hx = cc - 1;
			
			double fx = vx - Lx;
			

			double d1 = cm.getData( Ly ,Lx );
			double d2 = cm.getData( Ly ,Hx );
			double d3 = cm.getData( Hy ,Lx );
			double d4 = cm.getData( Hy ,Hx );

			double dl = d1 * ( 1 - fx ) + d2 * fx;
			double dh = d3 * ( 1 - fx ) + d4 * fx;

			double d = dl * ( 1 - fy ) + dh * fy;

			result->setData( y ,x ,d );
		}
	}

	return result;
}
/*
 *It'll subsample the matrix m to row * col
 *Parameter：the matrix needed to subsample
 *Return：the subsample result
*/
int testNPower2( int n );//It'll test n is power of 2 
Matrix* 
SaliencyMap::getSubSample( const Matrix& cm ,int row ,int col ) {
	double r = cm.getRowCount();
	double c = cm.getColCount();
	assert( r >= row && c >= col );//確定是subsample
	if ( r == row && c == col ) {//大小一樣
		
		return new Matrix( cm );
	}
	
	int rs = (int) ceil( r / row );
	int cs = (int) ceil( c / col );

	Matrix* result = new Matrix( row ,col );
	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			double d = cm.getData( i * rs ,j * cs );
			
			result->setData( i ,j ,d );
		}
	}
	
	return result;
}
/*
 *It'll test n is power of 2
  If it is ,it'll return 1
  If it is not ,it'll return the factor
  for instance , 6 is not a power of 2 it'll return 3
 *Parameter：
    n to be test
 *Return：
    the indicate result
*/
int testNPower2( int n ) {
	while ( n % 2 == 0 ) 
		n /= 2;
	

	return n;
}
/*
 *Perform the Normalization operator in the paper section 2.2.
  It'll have the following instructions
  (1) normalizing hte values to a fixed range [ 0 .. M ]
  (2) finding the maximum pixel of the image and the average of all it's local maximum u
  (3) multiplying the map by ( M - u ) ^2
 *Parameter：the matrix need to do normalize operation.
             Be careful that it'll change the matrix
*/
#using <mscorlib.dll>
using System::Double;
void 
SaliencyMap::performNormalization( Matrix& m ) {
	
	
	/*諄循著參考的程式 將東西normalize到[ 0 - 10 ]*/
	double max = 10 ,min = 0;
	/*1.找到最大和最小以及Average的值*/
	
	double maxvalue = Double::MinValue;
	double minvalue = Double::MaxValue;
	double avg = 0;//Local Maximua的total
	int amount = 0;

	int row = m.getRowCount();
	int col = m.getColCount();
	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			
			double d = m.getData( i ,j );
			if ( d > maxvalue )
				maxvalue = d;
			if ( d < minvalue )
				minvalue = d;

			/*找local Maximum*/
			double dl = 0;//d的左邊
			if ( j - 1 >= 0 ) dl = m.getData( i ,j-1 );
			double dr = 0;//d的右邊
			if ( j + 1 < col ) dr = m.getData( i ,j+1 );
			double du = 0;//d的上面
			if ( i - 1 >= 0 ) du = m.getData( i-1 ,j );
			double dd = 0;//d的下面
			if ( i + 1 < row ) dd = m.getData( i+1 , j);
			if ( d > dl && d >  dr && d > du && d > dd ) {
				avg += d;
				amount++;
			} 
		}
	}

	if ( amount != 0 )
		avg /= amount;
	
	/*
	 算出平均值他們到最後會成為甚麼樣的值會 並求出 (M - avg )^2 以方便相乘

	         avg - minvalue         d - min
	    ----------------------- = ----------
		   maxvalue - minvalue     max - min
	*/
	double diff = maxvalue - minvalue;
	if ( diff + min == 0 ) {
		cout << "SalienceMap::performNormalization:Divide by Zeros\n";
		return;
	}

	avg = ( max - min ) * ( avg - minvalue ) / diff + min;
	double s = Math::Pow( max - avg ,2 );

	/*進行algorithm*/
	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			double d = m.getData( i ,j );

			
			d = ( max - min ) * ( d - minvalue ) / diff + min;//(1)
			d *= s;//(3)
			
			m.setData( i ,j , d );
		}
	}

}
/*
 *Take the absolute value of the matrix
*/
void 
SaliencyMap::ABS( Matrix& m ) {
	int row = m.getRowCount();
	int col = m.getColCount();

	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			double d = m.getData(i ,j );
			
			m.setData( i ,j ,abs(d) );
		}
	}
}
/*
 *use the GaborKernel k to perform Gabor filtering on m 
*/
Matrix 
SaliencyMap::performGaborFiltering( const Matrix* m , const GaborKernel* k ) {
	int row = m->getRowCount();
	int col = m->getColCount();
	float* * d = m->getDatas();

	float* * tmp = Filter::BasicFiltering( d , row ,col,k );/*取得和在此Theta底下的Filter結果*/
	Matrix result( tmp , row ,col );

	::dispose( tmp , row ,col );//刪掉值
	::dispose( d , row ,col );//刪掉data的值

	return result;//O(c ,theta)得到
}
/*
 *將傳進來的Matrix進行Normalize的動作
  將其值mapping到[-1..1]
*/
void  
SaliencyMap::normalize( Matrix& m ) {

	double maxvalue = Double::MinValue;
	double minvalue = Double::MaxValue;
	
	int row = m.getRowCount();
	int col = m.getColCount();
	double d = 0;
	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			d = m.getData( i ,j );
			if ( d > maxvalue )
				maxvalue = d;
			if ( d < minvalue )
				minvalue = d;

		}
	}
	/*
	    max = 1 min = -1

	         avg - minvalue         d - min
	    ----------------------- = ----------
		   maxvalue - minvalue     max - min
	*/
	double diff = maxvalue - minvalue;
	d = 0;
	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			d = m.getData( i ,j );
			d = 2 * ( d - maxvalue ) / diff + 1;
			m.setData( i ,j ,d );
		}
	}
}