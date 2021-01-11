/******************************************
FILE
    MotionSaliencyMap.cpp
PURPOSE
    this class add the motion information to the salience map
	according to "Realistic Avatar Eye and Head Animation Using a Neurobiological Model of Visual Attention"

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include <iostream>
#include <cassert>
#include <exception>
#include "MotionSaliencyMap.h"

using std::cout;

/*Static Data member的建構子*/
GaussianPyramid* MotionSaliencyMap::preGI = NULL;//用來給Flicker Map用的
Matrix* MotionSaliencyMap::preSP = NULL;//previous Shifted Pyramids

MotionSaliencyMap::MotionSaliencyMap( const string& fname ,bool islastFrame ) : SaliencyMap(fname) {
	lastframe = islastFrame;
	init();
}
MotionSaliencyMap::MotionSaliencyMap (const MotionSaliencyMap& v ) : SaliencyMap( v ){
}
MotionSaliencyMap& MotionSaliencyMap::operator= (const MotionSaliencyMap& v ) {
	SaliencyMap::operator =( v );

	dispose();

	init();
	return *this;
}
MotionSaliencyMap::MotionSaliencyMap (const SaliencyMap & v ) : SaliencyMap( v ) {
}
/*
  Destructor
*/
MotionSaliencyMap::~MotionSaliencyMap() {
	
	dispose();
}
/*Perform the center-surround differences and normalization to obtain feature map*/
/*a - b*/
Matrix* subMatrix( const Matrix* a ,const Matrix* b);
void 
MotionSaliencyMap::doFlickerFeatureMap( const GaussianKernel* k ) {
	
	cout << "MotionSaliencyMap:取得 feature map of flicker\n";
	if ( flickerfeaturemap == NULL )
		flickerfeaturemap = new Matrix[ ffmsize ];

	if ( GI == NULL ) 
		makeIntensityPyramid( k );

	/*如果沒有preGI 就表示這是Frame開頭 換句話說 Assign需要的資訊給他*/
	if ( preGI == NULL )
		preGI = new GaussianPyramid( *GI );

	/*Get the feature map of flicker*/
	int i = 0;
	for ( int c = 2; c <= 4; c++ ) {
		Matrix* Ic =  subMatrix( GI->getLevelDataWithoutCopy(c) ,preGI->getLevelDataWithoutCopy(c) );
		if ( Ic == NULL ) {//Feature map如果產生不出來 就停止再產生了
			ffmsize = i++;
			break;
		}
		

		for ( int delta = 3; delta <= 4; delta++ ) {
			Matrix* tmpIs = subMatrix( GI->getLevelDataWithoutCopy( c+delta ) ,preGI->getLevelDataWithoutCopy( c+delta ) );
			
			if ( tmpIs == NULL ) {
				ffmsize = i++;
				break;
			}
			
			/*將最上層的進行內插到比較下層 也就是將粗糙的內插到比較精緻的*/
			Matrix* Is = interpolationFromCoarseToFine( *tmpIs ,Ic->getRowCount() ,Ic->getColCount() );

			flickerfeaturemap[i] = (*Ic) - (*Is) ;

			ABS( flickerfeaturemap[i] );//取絕對值

			//std::cout << intensityfeaturemap[i];
			//std::cout <<"=====================\n";
			performNormalization( flickerfeaturemap[i] );
			//std::cout << intensityfeaturemap[i];
			//std::cout <<"=====================\n";

			i++;

			delete Is;
			delete tmpIs;
		}

		delete Ic;
	}

	/*assign並刪除舊有的資訊*/
	GaussianPyramid* tmp = preGI;
	preGI = new GaussianPyramid( *GI );//記錄之前的資訊 以便下一次呼叫使用
	delete tmp;

	cout << "取得完畢\n";
}
Matrix* subMatrix( const Matrix* a ,const Matrix* b) {
	assert( a->getColCount() == b->getColCount() && a->getRowCount() == b->getRowCount() );

	Matrix* m = new Matrix( a->getRowCount() ,a->getColCount() ,0);
	*m = *a - *b;

	return m;
}
/*
  本來程式應該要對Orientation pyramids 但是為了加快速度 我們使用intensity pyramids
  而我們得到 S 這個map的方式是透過
*/
static const int orinum = 4;
void 
MotionSaliencyMap::doMotionFeatureMap( const GaussianKernel* k ) {

	cout << "MotionSaliencyMap:取得 feature map of motion\n";
	if ( motionfeaturemap == NULL )
		motionfeaturemap = new Matrix[ mfmsize ];

	if ( GI == NULL ) {//如果沒有intensity pyramids 就製造一個
		makeIntensityPyramid( k );
	}

	/*如果沒有preGI 就表示這是Frame開頭 換句話說 Assign需要的資訊給他*/
	if ( preGI == NULL )
		preGI = new GaussianPyramid( *GI );

	if ( preSP == NULL )
		preSP = buildShiftedPyr();
	
	Matrix* SP = buildShiftedPyr();
	Matrix* Rn = buildReichardtModel( SP );

	int mi = 0;
	for ( int c = 2; c <= 4; c++ ) {
		
		for ( int delta = 3; delta <=4; delta++ ) {
			/*Rn(c,s,t) = | Rn(c,t) - Rn(s,t) | */
			
			for ( int t = 0; t < orinum; t++ ) {
				Matrix* Rnct = &Rn[ c*orinum+t ];
				Matrix* Snst = interpolationFromCoarseToFine( Rn[ (c+delta)*orinum+t ] ,Rnct->getRowCount() ,Rnct->getColCount() );

				motionfeaturemap[mi] = (*Rnct) - (*Snst);

				ABS( motionfeaturemap[mi] );//取絕對值
				performNormalization( orientationfeaturemap[mi] );

				mi++;
				delete Snst;
			}
		}
	}

	delete [] Rn;

	//做assign的動作以方便下一筆用
	Matrix* tmp = preSP;
	preSP = SP;
	delete [] tmp;
	GaussianPyramid* tmpGI = preGI;
	preGI = new GaussianPyramid( *GI );//記錄之前的資訊 以便下一次呼叫使用
	delete tmpGI;

}
/*Build the shiftd pyramids according to the note above*/
Matrix* shiftMatrix( const Matrix* source , int dir );//Copy source matrix to new matrix
Matrix* 
MotionSaliencyMap::buildShiftedPyr() {
	/*
	  本函式根據Intensity Pyramids對每一個scale進行四種不同的移動
	  分別是 上下左右(分別代表0  並以以下方式排列
	   c2		    c3				c4
	  ----------------------------------------------
	  |				|				|				|
	  |UP	Down...	|UP		Down....|UP		Down	|
	  ----------------------------------------------
	  |		|		|		|		|		|		|
      ----------------------------------------------
	*/
	
	int scale = GI->getLevelAmount();

	Matrix* sp = new Matrix[ scale*orinum ];
	for ( int i = 0; i < scale; i++ ) {
		const Matrix* tmpI = GI->getLevelDataWithoutCopy(i);
		
		for ( int j = 0; j < orinum; j++ ) {
			Matrix* s = shiftMatrix(tmpI ,j );
			
			sp[ i*4+j ] = *s;

			delete s;//s可以不用了 因為已經複製到sp
		}
	}

	return sp;
}
/*
  根據dir移動整個Matrix
*/
enum Direction {UPtoDown = 0 ,DOWNtoUP ,LEFTtoRIGHT ,RIGHTtoLEFT };
Matrix* 
shiftMatrix( const Matrix* source , int dir ) {

	int row = source->getRowCount();
	int col = source->getColCount();
	Matrix* dst = new Matrix( row ,col ,0 );

	//設定Copy的Source 和Destination區塊
	int startx = 0 ,endx = col - 1 ,dststartx = 0, dstendx = col - 1;
	int starty = 0 ,endy = row - 1 ,dststarty = 0, dstendy = row - 1;

	switch ( dir ) {
		case UPtoDown://將source 的上半 移到destination下半去
			/*
			  src:             dst:
			     XXXO             000 0 
				 XXXO ======      XXX 0 
				 XXXO ======      XXX 0 
				 OOOO             XXX 0 
			*/
			dststarty++; dstendy++;
			break;
		case DOWNtoUP://將Souce下半移到Destination上半去
			/*
			  src:             dst:
			     OOOO             XXX 0  
				 XXXO ======      XXX 0 
				 XXXO ======      XXX 0 
				 XXXO             000 0
			*/
			starty++; endy++;
			break;
		case LEFTtoRIGHT://將source左半 放到destination的右邊
			/*
			  src:             dst:
			     XXXO             0 XXX
				 XXXO ======      0 XXX
				 XXXO ======      0 XXX
				 OOOO             0 000
			*/
			dststartx++; dstendx++;
			break;
		case RIGHTtoLEFT://將source右半 放到destination左邊
			/*
			  src:             dst:
			     OXXX             XXX 0 
				 OXXX ======      XXX 0 
				 OXXX ======      XXX 0 
				 OOOO             000 0 
			*/
			startx++; endx++;
			break;
	}

	/*Copy*/
	for ( int k = starty ,i = dststarty; k < endy && i < dstendy ; k++ ,i++ ) {
		for ( int l = startx ,j = dststartx; l < endx && j < dstendx; l++  ,j++ ) {
			double color = source->getData( k , l );
			dst->setData( i ,j ,color );
		}
	}
	
	return dst;

}
/*
  根據Paper上的Reichardt model 建立Rn

  Paper當中的
  O_n-1 = preGI 而O_n = GI
  S_n-1 = preSP 而S_n = 由目前GI所算出的SP
	
*/
Matrix* 
MotionSaliencyMap::buildReichardtModel( const Matrix* SP ) {
	
	assert( GI != NULL && preGI != NULL && preSP != NULL && SP != NULL );

	int scale = GI->getLevelAmount();
	Matrix* R = new Matrix[ scale*orinum ];
	
	for ( int i = 0; i < scale; i++ ) {
		const Matrix* On = GI->getLevelDataWithoutCopy( i );
		const Matrix* On_1 = preGI->getLevelDataWithoutCopy( i );
		
		int row = On->getRowCount();
		int col = On->getColCount();

		for ( int j = 0; j < orinum; j++ ) {
			const Matrix* Sn = &SP[ i*orinum+j ];
			const Matrix* Sn_1 = &preSP[ i*orinum+j ];

			/*防止錯誤用 其實如果架構寫得好應該可以不用管 因為畢竟這整個function private了*/
			assert( row == Sn->getRowCount() && col == Sn->getColCount() );
			assert( row == Sn_1->getRowCount() && col == Sn_1->getColCount() );
			
			/*開始計算*/
			R[ i*orinum+j ] = (*Sn);
			Matrix* Rn = &R[ i*orinum+j ];//取資料以後方便好assign
			

			for ( int k = 0; k < row; k++ ) {
				for ( int l = 0; l < col; l ++ ) {
					double val = On->getData( k ,l ) * Sn_1->getData( k ,l ) - On_1->getData( k ,l ) * Sn->getData( k ,l );
					
					Rn->setData( k , l ,val );
				}
			}
		}
	}
	return R;
}
/*Perform the across-scale combination and normalization to obtain Conspicuity map*/
Matrix* 
MotionSaliencyMap::getFConspicuityMap() {
	/*check有沒有做過Feature map*/
	if ( flickerfeaturemap == NULL ) 
		doFlickerFeatureMap();

	cout << "MotionSaliencyMap:取得conspicuity map of Flicker\n";

	int row = flickerfeaturemap[ ifmsize - 1 ].getRowCount();//所有資料都downsample到最後一個好加加
	int col = flickerfeaturemap[ ifmsize - 1 ].getColCount();

	/*將全部downsample並加起來就對了*/
	Matrix* result = new Matrix( row ,col );
	for ( int i = 0; i < ifmsize; i++ ) {
		Matrix* I = getSubSample( flickerfeaturemap[i] ,row ,col );
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
MotionSaliencyMap::getMConspicuityMap() {
	/*check有沒有做過Feature map*/
	if ( motionfeaturemap == NULL )
		doMotionFeatureMap();

	cout << "MotionSaliencyMap:取得conspicuity map of Motion\n";

	int row = motionfeaturemap[ mfmsize - 1 ].getRowCount();//所有資料都downsample到最後一個好加加
	int col = motionfeaturemap[ mfmsize - 1 ].getColCount();

	Matrix* result = new Matrix( row ,col );
	for ( int i = 0; i < ofmsize; i++ ) {
		Matrix* M = getSubSample( motionfeaturemap[i] ,row ,col );

		*result += *M;

		delete M;
	}

	performNormalization( *result );

	cout << "取得完畢\n";

	return result;
}
/*Perform the salience map algorithm to get the salience map*/
Matrix* 
MotionSaliencyMap::getSaliencyMap() {

	Matrix result = getOriginalSaliencyMap();
	if ( flickerfeaturemap == NULL )  {
		try {
			doFlickerFeatureMap();
		} catch ( std::exception ex ) {
			cout << ex.what();
		}
	}
	if ( motionfeaturemap == NULL ) {
		try {
			doMotionFeatureMap();
		} catch ( std::exception ex ) {
			cout << ex.what();
		}
	} 
	

	if ( flickercm == NULL ) {
		flickercm = getFConspicuityMap();
	}
	if ( motioncm == NULL )
		motioncm = getMConspicuityMap();

	result += *flickercm + *motioncm;

	result.scale( 1.0/3 );

	Matrix* tmp = interpolationFromCoarseToFine( result ,row ,col );

	//進行Normalize到[-1 .. 1]
	normalize( *tmp );

	return tmp;
}
/*
  Initialize
*/
void 
MotionSaliencyMap::init() {
	ffmsize = 6;
	flickerfeaturemap = NULL;
	

	mfmsize = 24;
	motionfeaturemap = NULL;

	flickercm = NULL;
	motioncm = NULL;
}
/*
  Dispose
*/
void 
MotionSaliencyMap::dispose() {

	cout << "刪除MotionSaliencyMap物件\n";
	if ( lastframe == true ) {
		delete preGI;
		preGI = NULL;
		delete [] preSP;
		preSP = NULL;
	}

	if ( flickerfeaturemap != NULL )
		delete [] flickerfeaturemap;
	if ( motionfeaturemap != NULL )
		delete [] motionfeaturemap;

	if ( flickercm != NULL ) 
		delete flickercm;
	if ( motioncm != NULL )
		delete motioncm;
}