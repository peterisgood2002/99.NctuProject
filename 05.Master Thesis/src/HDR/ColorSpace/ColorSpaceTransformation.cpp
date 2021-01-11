/******************************************
FILE
    ColorSpaceTransformation.cpp
PURPOSE
    ColorSpace轉換的所需要的東西 
	這是一個abstract class
	用來提供所有ColorSpace都應該要有的一些函式

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include "ColorSpaceTransformation.h"
/*Constructor*/
ColorSpaceTransformation::ColorSpaceTransformation () 
{
	//等到要呼叫的時候再說 先設定為NULL
	transmatrix = NULL;
	inversetransmatrix = NULL;
}
ColorSpaceTransformation::ColorSpaceTransformation ( const Matrix&  m )
{
	setTransformationMatrix(m);

}
ColorSpaceTransformation::ColorSpaceTransformation ( float* * const d , int row ,int col ) 
{
	setTransformationMatrix( d ,row ,col );
}
/*operator assignment*/
ColorSpaceTransformation& 
ColorSpaceTransformation::operator= (const ColorSpaceTransformation& cs ) {
	delete transmatrix;
	delete inversetransmatrix;

	transmatrix = new Matrix( *cs.transmatrix );
	inversetransmatrix = new Matrix( *cs.inversetransmatrix );

	return *this;
}
/*Destructor*/
ColorSpaceTransformation::~ColorSpaceTransformation() {
	delete transmatrix;
	delete inversetransmatrix;
}



/*set Matrix*/
void
ColorSpaceTransformation::setTransformationMatrix( const Matrix&  m ) {
	transmatrix = new Matrix(m);

	inversetransmatrix = new Matrix( transmatrix->getInverseMatrix() );
}
void
ColorSpaceTransformation::setTransformationMatrix( float* * const d , int row ,int col ) {
	transmatrix = new Matrix(d , row ,col );

	inversetransmatrix = new Matrix( transmatrix->getInverseMatrix() );
}


Matrix* 
ColorSpaceTransformation::getTransformMatrix() const {
	return new Matrix( *transmatrix );
}

Matrix*
ColorSpaceTransformation::getInverseTransformationMatrix() const{
	return new Matrix( *inversetransmatrix );
}
