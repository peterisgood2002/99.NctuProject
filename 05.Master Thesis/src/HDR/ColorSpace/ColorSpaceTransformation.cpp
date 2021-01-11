/******************************************
FILE
    ColorSpaceTransformation.cpp
PURPOSE
    ColorSpace�ഫ���һݭn���F�� 
	�o�O�@��abstract class
	�ΨӴ��ѩҦ�ColorSpace�����ӭn�����@�Ǩ禡

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/
#include "ColorSpaceTransformation.h"
/*Constructor*/
ColorSpaceTransformation::ColorSpaceTransformation () 
{
	//����n�I�s���ɭԦA�� ���]�w��NULL
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
