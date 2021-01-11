/******************************************
FILE
    RGB2XYZ.cpp
PURPOSE
    進行RGB -> XY與XY -> RGB轉換

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/

#include "RGB2XYZ.h"
#include "../Essential/Matrix.h"
#include <string>

using std::string;

const string RGB2XYZ::type = "RGB2XYZ";
const int RGB2XYZ::dimension = 3;

/*Constructor*/
RGB2XYZ::RGB2XYZ() : ColorSpaceTransformation() {
	/*設定default value  CRT非線性色彩
		x    y
       0.64, 0.33,   // r
	   0.29, 0.60,   // g
	   0.15, 0.06    // b
	   0.33, 0.33    // w
	
	*/
	float* *d = create2DArray( dimension ,dimension );
	d[0][0] = 0.64;     d[0][1] = 0.29;     d[0][2] = 0.15;   
	d[1][0] = 0.33;     d[1][1] = 0.60;     d[1][2] = 0.06;
	d[2][0] = 0.03;     d[2][1] = 0.11;     d[1][2] = 0.79;
	float* w = new float[ dimension ];
	for ( int i = 0; i < dimension; i++ )
		w[i] = 1.0 / dimension;

	calTransMatrix(d ,w );

	dispose( d ,dimension ,dimension );
	delete [] w;
}
RGB2XYZ::RGB2XYZ( const Matrix&  m ) :ColorSpaceTransformation(m) {

}
RGB2XYZ::RGB2XYZ( float* * const chrom , const float* w ) : ColorSpaceTransformation() {
	calTransMatrix(chrom ,w );
}
//string 
//RGB2XYZ::getType()
//{ return type; }

/*
 計算轉換矩陣用
*/
#include <iostream>
void 
RGB2XYZ::calTransMatrix(float* * const chrom ,const float* w ) {
	
	/*
	  R + G + B = W
	  ==>
		arxr + agxg + abxb = xn/yn
        aryr + agyg + abyb = 1 
        arzr + agzg + abzb = zn/yn
		求A = ar ag ab
	*/
	Matrix m( chrom ,dimension ,dimension );

	double* ww = new double[ dimension ];
	for ( int i = 0; i < dimension; i++ ) {
		ww[i] = w[i] / w[1];
	}
	
	double* A = m.getInverseMatrix().multiplyX( ww ,dimension );
	
	for ( int i = 0; i < dimension; i++ )
		m.scaleCol( i ,A[i] );

	ColorSpaceTransformation::transmatrix = new Matrix(m);
	ColorSpaceTransformation::inversetransmatrix = new Matrix( m.getInverseMatrix() );

	delete [] A;
	delete [] ww;
}
//進行正轉換
Vector 
RGB2XYZ::transform ( const Vector& v ) const {
	int dim = v.getDimension();
	double* d = v.getData();
	
	
	double* result = transmatrix->multiplyX( d ,dim );
	delete [] d;
	if ( result != NULL ) {
		Vector v1( dim ,result );
		
		delete [] result;
		return v1;
	}else return NULL;

}
Vector 
RGB2XYZ::transform ( const Vector& v , const string& t) const {
	return transform(v);
}
/*
  進行逆轉換 
  傳入：要轉換的Vector 以及轉換是否成功的bit
  傳出：轉換結果如果無法進行逆轉換 將會回傳default matrix
*/
#include <iostream>

using std::cerr;
Vector 
RGB2XYZ::inverseTransform( const Vector& v ,bool& issuccess ) const {
	int dim = v.getDimension();
	
	issuccess = false;
	
	if ( inversetransmatrix != NULL ) {
		double* d = v.getData();
		double* result = inversetransmatrix -> multiplyX( d ,dim );//Array
		delete [] d;
		
		if ( result != NULL ) {
			Vector v1( dim ,result );
			issuccess = true;
			
			delete [] result;
			return v1;
		}else 
			return Vector();



	}else {
		cerr <<"ColorSpaceTransformation::inverseTransform() error：Inverse transformation cannot perform\n";
		return Vector();
	}
}
Vector 
RGB2XYZ::inverseTransform ( const Vector& v ,bool& issuccess , const string& t ) const {
	return inverseTransform( v , issuccess);
}