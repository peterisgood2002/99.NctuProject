/******************************************
FILE
    CNumberArray.cpp
PURPOSE
    此檔案將會包括一些一個ComplexNumber Array的相關函式 以便於管理以及添加新事物


NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include "ComplexNumber.h"
#include <cassert>

/*Constructor*/
CNumberArray::CNumberArray( int s ,const double* d ) {
	assert( s > 0 );

	size = s;
	data = new ComplexNumber[size];
	if ( d != NULL ) {
		for ( int i = 0; i < size; i++ )
			setData( i ,d[i] ,0 );
	}
}
/*Constructor*/
CNumberArray::CNumberArray( const ComplexNumber* d ,int s ) {
	assert( s > 0 && d != NULL );

	size = s;
	
	data = new ComplexNumber[ size ];

	for ( int i = 0; i < size; i++ ) 
		data[i] = d[i];
	
}
/*the big three*/
CNumberArray::CNumberArray( const CNumberArray& c ) {
	size = c.size;
	data = new ComplexNumber[ size ];
	
	for ( int i = 0; i < size; i++ )
		setData( i , c[i] );
}
CNumberArray& 
CNumberArray::operator= ( const CNumberArray& c ) {
	delete [] data;

	size = c.size;
	data = new ComplexNumber[ size ];
	
	for ( int i = 0; i < size; i++ )
		setData( i , c[i] );

	return *this;
	
}
CNumberArray::~CNumberArray() {
	delete [] data;
}
void 
CNumberArray::setData( int i ,double r ,double imagery ) {
	assert( i < size && i >= 0 );

	data[i].setData( r ,imagery );

}
void 
CNumberArray::setData(int i , const ComplexNumber& c  ) {
	assert( i < size && i >= 0 );

	data[i] = c;
}

/*
 reset the size 
 *Parameter:
  *the array new size
	if the size is bigger than original 
	it'll extend the original array and append zeros
	if the size is smaller than original 
	it'll truncate it to new size
*/
void 
CNumberArray::resetSize( int nsize ) {
	assert( nsize > 0 );

	if ( nsize >= size ) {
		ComplexNumber* tmp = data;
		
		data = new ComplexNumber[ nsize ];

		for ( int i = 0; i < size; i++ ) 
			data[i].setData( tmp[i].getReal() ,tmp[i].getImagery() );

		delete [] tmp;
	}

	size = nsize;
}
ComplexNumber 
CNumberArray::getData(int i) const{
	assert( i < size && i >= 0 );

	return data[i];
}
int 
CNumberArray::getSize() const{
	return size;
}
ComplexNumber& 
CNumberArray::operator[] (int i) const {
	assert( i < size );

	return data[i];
}
/**
 *Pixel-wise Multiplication：
     it'll overload operator* 
 *Parameter：Matrix m2 to multiply
 *Return：
	if this.col and m2.row are not the same it'll return the zero Matrix
 */
CNumberArray 
CNumberArray::operator*( const CNumberArray& a2 ) const {
	assert( size == a2.size );

	ComplexNumber* result  = new ComplexNumber[ size ];

	for ( int i = 0; i < size; i++ )
		result[i] = data[i] * a2.data[i];

	return CNumberArray( result ,size);
}
CNumberArray& 
CNumberArray::operator*= ( const CNumberArray& a2 ) {
	assert( size == a2.size );

	for ( int i = 0; i < size; i++ )
		data[i] *= a2.data[i];

	return *this;
}
/**
 *Overloading the + and += operator
 *Parameter：another Vector to add
 *Return：
    if the dimensions are not the same it'll return the zero Vector
 */
CNumberArray 
CNumberArray::operator+( const CNumberArray& a2 ) const {
	assert( size == a2.size );

	ComplexNumber* result  = new ComplexNumber[ size ];
	for ( int i = 0; i < size; i++ )
		result[i] = data[i] + a2.data[i];

	return CNumberArray( result ,size);

}
CNumberArray& 
CNumberArray::operator+= ( const CNumberArray& a2 ) {
	assert( size == a2.size );

	for ( int i = 0; i < size; i++ )
		data[i] += a2.data[i];

	return *this;
}
/**
 *Overloading the + and += operator
 *Parameter：another Vector to add
 *Return：
    if the dimensions are not the same it'll return the zero Vector
 */
CNumberArray 
CNumberArray::operator-( const CNumberArray& a2 ) const {
	assert( size == a2.size );

	for ( int i = 0; i < size; i++ )
		data[i] -= a2.data[i];

	return *this;
}
CNumberArray& 
CNumberArray::operator-= ( const CNumberArray& a2 ) {
	assert( size == a2.size );

	for ( int i = 0; i < size; i++ )
		data[i] -= a2.data[i];

	return *this;
}
/*it'll implement c * a2 and a2 * c*/
CNumberArray operator* ( const ComplexNumber& c , const CNumberArray& a2 ) {

	CNumberArray result( a2.data ,a2.size );
	for ( int i = 0; i < a2.size; i++ )
		result[i] *= c;

	return result;

}
CNumberArray operator* ( const CNumberArray& a2 , const ComplexNumber& c ) {

	CNumberArray result( a2.data ,a2.size );
	for ( int i = 0; i < a2.size; i++ )
		result[i] *= c;

	return result;
}
ostream& 
operator<< ( ostream& out ,const CNumberArray& r ) {
	for ( int i = 0; i < r.getSize(); i++ )
		out<< r[i] <<" ";

	return out;
}