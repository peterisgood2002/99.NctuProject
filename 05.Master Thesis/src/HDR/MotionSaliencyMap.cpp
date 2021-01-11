/******************************************
FILE
    MotionSaliencyMap.cpp
PURPOSE
    this class add the motion information to the salience map
	according to "Realistic Avatar Eye and Head Animation Using a Neurobiological Model of Visual Attention"

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/
#include <iostream>
#include <cassert>
#include <exception>
#include "MotionSaliencyMap.h"

using std::cout;

/*Static Data member���غc�l*/
GaussianPyramid* MotionSaliencyMap::preGI = NULL;//�Ψӵ�Flicker Map�Ϊ�
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
	
	cout << "MotionSaliencyMap:���o feature map of flicker\n";
	if ( flickerfeaturemap == NULL )
		flickerfeaturemap = new Matrix[ ffmsize ];

	if ( GI == NULL ) 
		makeIntensityPyramid( k );

	/*�p�G�S��preGI �N��ܳo�OFrame�}�Y ���y�ܻ� Assign�ݭn����T���L*/
	if ( preGI == NULL )
		preGI = new GaussianPyramid( *GI );

	/*Get the feature map of flicker*/
	int i = 0;
	for ( int c = 2; c <= 4; c++ ) {
		Matrix* Ic =  subMatrix( GI->getLevelDataWithoutCopy(c) ,preGI->getLevelDataWithoutCopy(c) );
		if ( Ic == NULL ) {//Feature map�p�G���ͤ��X�� �N����A���ͤF
			ffmsize = i++;
			break;
		}
		

		for ( int delta = 3; delta <= 4; delta++ ) {
			Matrix* tmpIs = subMatrix( GI->getLevelDataWithoutCopy( c+delta ) ,preGI->getLevelDataWithoutCopy( c+delta ) );
			
			if ( tmpIs == NULL ) {
				ffmsize = i++;
				break;
			}
			
			/*�N�̤W�h���i�椺�������U�h �]�N�O�N���W������������o��*/
			Matrix* Is = interpolationFromCoarseToFine( *tmpIs ,Ic->getRowCount() ,Ic->getColCount() );

			flickerfeaturemap[i] = (*Ic) - (*Is) ;

			ABS( flickerfeaturemap[i] );//�������

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

	/*assign�çR���¦�����T*/
	GaussianPyramid* tmp = preGI;
	preGI = new GaussianPyramid( *GI );//�O�����e����T �H�K�U�@���I�s�ϥ�
	delete tmp;

	cout << "���o����\n";
}
Matrix* subMatrix( const Matrix* a ,const Matrix* b) {
	assert( a->getColCount() == b->getColCount() && a->getRowCount() == b->getRowCount() );

	Matrix* m = new Matrix( a->getRowCount() ,a->getColCount() ,0);
	*m = *a - *b;

	return m;
}
/*
  ���ӵ{�����ӭn��Orientation pyramids ���O���F�[�ֳt�� �ڭ̨ϥ�intensity pyramids
  �ӧڭ̱o�� S �o��map���覡�O�z�L
*/
static const int orinum = 4;
void 
MotionSaliencyMap::doMotionFeatureMap( const GaussianKernel* k ) {

	cout << "MotionSaliencyMap:���o feature map of motion\n";
	if ( motionfeaturemap == NULL )
		motionfeaturemap = new Matrix[ mfmsize ];

	if ( GI == NULL ) {//�p�G�S��intensity pyramids �N�s�y�@��
		makeIntensityPyramid( k );
	}

	/*�p�G�S��preGI �N��ܳo�OFrame�}�Y ���y�ܻ� Assign�ݭn����T���L*/
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

				ABS( motionfeaturemap[mi] );//�������
				performNormalization( orientationfeaturemap[mi] );

				mi++;
				delete Snst;
			}
		}
	}

	delete [] Rn;

	//��assign���ʧ@�H��K�U�@����
	Matrix* tmp = preSP;
	preSP = SP;
	delete [] tmp;
	GaussianPyramid* tmpGI = preGI;
	preGI = new GaussianPyramid( *GI );//�O�����e����T �H�K�U�@���I�s�ϥ�
	delete tmpGI;

}
/*Build the shiftd pyramids according to the note above*/
Matrix* shiftMatrix( const Matrix* source , int dir );//Copy source matrix to new matrix
Matrix* 
MotionSaliencyMap::buildShiftedPyr() {
	/*
	  ���禡�ھ�Intensity Pyramids��C�@��scale�i��|�ؤ��P������
	  ���O�O �W�U���k(���O�N��0  �åH�H�U�覡�ƦC
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

			delete s;//s�i�H���ΤF �]���w�g�ƻs��sp
		}
	}

	return sp;
}
/*
  �ھ�dir���ʾ��Matrix
*/
enum Direction {UPtoDown = 0 ,DOWNtoUP ,LEFTtoRIGHT ,RIGHTtoLEFT };
Matrix* 
shiftMatrix( const Matrix* source , int dir ) {

	int row = source->getRowCount();
	int col = source->getColCount();
	Matrix* dst = new Matrix( row ,col ,0 );

	//�]�wCopy��Source �MDestination�϶�
	int startx = 0 ,endx = col - 1 ,dststartx = 0, dstendx = col - 1;
	int starty = 0 ,endy = row - 1 ,dststarty = 0, dstendy = row - 1;

	switch ( dir ) {
		case UPtoDown://�Nsource ���W�b ����destination�U�b�h
			/*
			  src:             dst:
			     XXXO             000 0 
				 XXXO ======      XXX 0 
				 XXXO ======      XXX 0 
				 OOOO             XXX 0 
			*/
			dststarty++; dstendy++;
			break;
		case DOWNtoUP://�NSouce�U�b����Destination�W�b�h
			/*
			  src:             dst:
			     OOOO             XXX 0  
				 XXXO ======      XXX 0 
				 XXXO ======      XXX 0 
				 XXXO             000 0
			*/
			starty++; endy++;
			break;
		case LEFTtoRIGHT://�Nsource���b ���destination���k��
			/*
			  src:             dst:
			     XXXO             0 XXX
				 XXXO ======      0 XXX
				 XXXO ======      0 XXX
				 OOOO             0 000
			*/
			dststartx++; dstendx++;
			break;
		case RIGHTtoLEFT://�Nsource�k�b ���destination����
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
  �ھ�Paper�W��Reichardt model �إ�Rn

  Paper����
  O_n-1 = preGI ��O_n = GI
  S_n-1 = preSP ��S_n = �ѥثeGI�Һ�X��SP
	
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

			/*������~�� ���p�G�[�c�g�o�n���ӥi�H���κ� �]�������o���function private�F*/
			assert( row == Sn->getRowCount() && col == Sn->getColCount() );
			assert( row == Sn_1->getRowCount() && col == Sn_1->getColCount() );
			
			/*�}�l�p��*/
			R[ i*orinum+j ] = (*Sn);
			Matrix* Rn = &R[ i*orinum+j ];//����ƥH���K�nassign
			

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
	/*check���S�����LFeature map*/
	if ( flickerfeaturemap == NULL ) 
		doFlickerFeatureMap();

	cout << "MotionSaliencyMap:���oconspicuity map of Flicker\n";

	int row = flickerfeaturemap[ ifmsize - 1 ].getRowCount();//�Ҧ���Ƴ�downsample��̫�@�Ӧn�[�[
	int col = flickerfeaturemap[ ifmsize - 1 ].getColCount();

	/*�N����downsample�å[�_�ӴN��F*/
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

	cout << "���o����\n";

	return result;
}
Matrix* 
MotionSaliencyMap::getMConspicuityMap() {
	/*check���S�����LFeature map*/
	if ( motionfeaturemap == NULL )
		doMotionFeatureMap();

	cout << "MotionSaliencyMap:���oconspicuity map of Motion\n";

	int row = motionfeaturemap[ mfmsize - 1 ].getRowCount();//�Ҧ���Ƴ�downsample��̫�@�Ӧn�[�[
	int col = motionfeaturemap[ mfmsize - 1 ].getColCount();

	Matrix* result = new Matrix( row ,col );
	for ( int i = 0; i < ofmsize; i++ ) {
		Matrix* M = getSubSample( motionfeaturemap[i] ,row ,col );

		*result += *M;

		delete M;
	}

	performNormalization( *result );

	cout << "���o����\n";

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

	//�i��Normalize��[-1 .. 1]
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

	cout << "�R��MotionSaliencyMap����\n";
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