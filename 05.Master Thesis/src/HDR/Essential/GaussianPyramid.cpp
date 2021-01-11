/******************************************
FILE
    GaussianPyramid.cpp
PURPOSE
    �w�qGaussian pyramid�������禡


NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

#include "GaussianPyramid.h"
#include <cmath>
#include <cassert>

GaussianPyramid::GaussianPyramid( float* * const data ,int row ,int col ,int level ,const GaussianKernel* k) {
	Matrix m( data ,row , col );
	pyramids = NULL;
	setGaussianPyramid( m ,level , k ); 
}
GaussianPyramid::GaussianPyramid( const Matrix& m ,int level ,const GaussianKernel* k  ) {
	setGaussianPyramid( m ,level , k );
}
GaussianPyramid::GaussianPyramid( const GaussianPyramid& g ) {

	tlevel = g.tlevel;

	pyramids = new Matrix[ tlevel ];

	for ( int i = 0; i < tlevel; i++ )
		pyramids[i] = g.pyramids[i];


}
GaussianPyramid::~GaussianPyramid() {
	delete [] pyramids;
}
GaussianPyramid& 
GaussianPyramid::operator= ( const GaussianPyramid& g ) {

	delete [] pyramids;

	tlevel = g.tlevel;

	pyramids = new Matrix[ tlevel ];

	for ( int i = 0; i < tlevel; i++ )
		pyramids[i] = g.pyramids[i];

	return *this;
}
int 
GaussianPyramid::getLevelAmount() const {
	return tlevel;
}
Matrix* 
GaussianPyramid::getLevelData( int l ) const {
	assert( l >= 0 );

	if ( l < tlevel  ) {
		return new Matrix( pyramids[l] );
	}
	else return NULL;
}
//���|�ʨ쪫�󤺪����
const Matrix* 
GaussianPyramid::getLevelDataWithoutCopy( int l ) const {
	assert( l >= 0 );
	if ( l < tlevel ) 
		return &pyramids[l];
	else return NULL;
}
/*Perform Gaussian Pyramid Alforithm*/
void 
GaussianPyramid::setGaussianPyramid( const Matrix& m ,int level ,const GaussianKernel* k ) {
	const GaussianKernel* tmp = k;
	if ( tmp == NULL )
		tmp = new GaussianKernel();
	
	
	tlevel = level;
	pyramids = new Matrix[ tlevel ];
	int i = 0;
	pyramids[i] = m;
	do {
		if ( pyramids[i].getElementAmount() == 1 )
			break;

		int row = pyramids[i].getRowCount();
		int col = pyramids[i].getColCount();
		float* * data = pyramids[i].getDatas();
		float* * d = Filter::BasicFiltering( data ,row ,col ,tmp );
		
		/*�i��Subsample�åBoutput*/
		int r = int( ceil( (double)row / 2.0 ) );

		int c = int( ceil( (double)col / 2.0 ) );
		Matrix subsample( r ,c );
		int y = 0;
		for ( int j = 0; j < row; j+=2 ) {
			int x = 0;
			for ( int k = 0; k < col; k+=2 ) 
				subsample.setData( y , x++ ,d[j][k] );

			y++;

		}

		pyramids[++i] = subsample;

		dispose( data ,row ,col );
		dispose( d ,row , col );
	} while ( i < tlevel - 1 );

	tlevel = i + 1;

	/*�p�Gk�ONULL ���tmp�����{��new�X�Ӫ� �~���ڥ������D�o�ӪF�誺�s�b ��o�R��*/
	if ( k == NULL )
		delete tmp;

}

ostream& operator<< (ostream& out ,const GaussianPyramid& gp ) {

	for ( int i = 0; i < gp.getLevelAmount(); i++ ) {
		out << "Level:" << i << "\n";
		out << *gp.getLevelData(i);
		out << "----------------\n";
	}

	out << "==================\n";

	return out;
}
