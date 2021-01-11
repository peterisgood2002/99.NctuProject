/******************************************
FILE
    CNumberMatrix.cpp
PURPOSE
    此檔案將會包括一些一個ComplexNumber 2DArray的相關函式 以便於管理以及添加新事物


NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include "ComplexNumber.h"
#include <cassert>

/*Constructor*/
CNumberMatrix::CNumberMatrix( int r ,int c ,float* * const d) {
	row = r; col = c;

	data = new CNumberArray[row];
	for ( int i = 0; i < row; i++ ) {
		data[i].resetSize(c);
		if ( d != NULL ) {
			for ( int j = 0; j < col; j++ ) {
				data[i].setData(j , d[i][j] , 0 );
			}
		}
	}
}
/*Consider the d array as one row */
CNumberMatrix::CNumberMatrix( const CNumberArray* d ,int r) {
	
	data = new CNumberArray[r];
	
	col = d[0].getSize();
	row = r;
	for ( int i = 0; i < r; i++ ) {
		assert( d[i].getSize() == col );
		data[i] = d[i];
	}


}
CNumberMatrix::CNumberMatrix( const CNumberMatrix& c ) {
	row = c.row; col = c.col;

	data = new CNumberArray[row];
	for ( int i = 0; i < row; i++ ) {
		data[i].resetSize( c.col );

		for ( int j = 0; j < col; j++ )
			data[i].setData(j ,c.data[i][j] );
	}
}
CNumberMatrix& 
CNumberMatrix::operator= ( const CNumberMatrix& c ) {
	delete [] data;

	row = c.row; col = c.col;

	data = new CNumberArray[row];
	for ( int i = 0; i < row; i++ ) {
		data[i].resetSize( c.col );

		for ( int j = 0; j < col; j++ )
			data[i].setData(j ,c.data[i][j] );
	}

	return *this;
}
CNumberMatrix::~CNumberMatrix(){
	delete [] data;
}
/*=======================
Get informations
=========================*/
int
CNumberMatrix::getRowSize() const{
	return row;
}
int
CNumberMatrix::getColSize() const{
	return col;
}
CNumberArray 
CNumberMatrix::getRow( int i ) const{
	assert( i < row && i >= 0 );

	return data[i];
}
CNumberArray 
CNumberMatrix::getCol( int i ) const{
	assert( i < col && i >= 0 );

	CNumberArray result( row );
	
	for ( int j = 0; j < row; j++ )
		result[j] = data[j][i];

	return result;

}

/*Set the row[i] into c. it is just a replacement*/
/*=============================
Set some operation
==============================*/
void 
CNumberMatrix::setRow( int i ,const CNumberArray& r ) {
	if ( r.getSize() > col )
		col = r.getSize();

	data[i] = r;
}
/*If the c.size don't equal the rows it'll extend the row to match it*/
void 
CNumberMatrix::setCol( int i ,const CNumberArray& c ) {
	assert( i < col && i >= 0 );
	if ( row < c.getSize() ) {
		//將舊的資料放入 並且將本來沒有資料的地方設0
		CNumberArray* tmp = data;
		int r = row;

		row = c.getSize();
		data = new CNumberArray[ row ];
		int j = 0;
		for ( j = 0; j < r; j++ ) 
			data[j] = tmp[j];
		
		for ( j; j < row; j++ )
			data[j].resetSize(col);	

		
		delete [] tmp;
	}

	//放入新的資料
	for ( int j = 0; j < row; j++ ) {
		if ( j >= c.getSize() )
			data[j][i] = 0;
		else data[j][i] = c[j];
	}

}

/*Reset row and col
 *Parameter:
  *the array new row or col
   if the size is bigger than original 
   it'll extend the original array and append zeros
   if the size is smaller than original 
   it'll truncate it to new size

*/
void 
CNumberMatrix::resetRow( int r ) {
	assert( r > 0 );

	if ( r >= row ) {
		CNumberArray* tmp = data;
		
		data = new CNumberArray[ r ];

		for ( int i = 0; i < row; i++ ) {
			data[i] = tmp[i];
		}
		delete [] tmp;
		
	}
	
	row = r;

}
void 
CNumberMatrix::resetCol( int c ) {
	assert ( c > 0 );
	
	for ( int i = 0; i < row; i++ )
		data[i].resetSize(c);
	

	col = c;
}
void 
CNumberMatrix::resetSize( int r ,int c ) {
	assert( c > 0 && r > 0 );

	resetRow(r);
	resetCol(c);
}
/*=============================
Operator overloading
===============================*/
CNumberArray& 
CNumberMatrix::operator[] ( int i ) const {
	assert( i >= 0 && i < row );

	return data[i];
}
/**
 *Pixel-wis Multiplication：
     it'll overload operator* 
 *Parameter：Matrix m2 to multiply
 *Return：
	if this.col and m2.row are not the same it'll return the zero Matrix
 */
CNumberMatrix 
CNumberMatrix::operator*( const CNumberMatrix& m2 ) const {
	assert( row == m2.row && col == m2.col );

	CNumberArray* result = new CNumberArray[row];

	for ( int i = 0; i < row; i++ ) {
		result[i] = data[i] * m2.data[i];
	}

	return CNumberMatrix( result , row );
}
CNumberMatrix& 
CNumberMatrix::operator*= ( const CNumberMatrix& m2 ) {
	assert( row == m2.row && col == m2.col );

	for ( int i = 0; i < row; i++ )
		data[i] *= m2.data[i];

	return *this;
}
//Debug用
ostream& 
operator<< ( ostream& out ,const CNumberMatrix& r ) {
	for ( int i = 0; i < r.getRowSize(); i++ ) {
		out << r[i];
		out <<"\n";
	}

	out<<"===============\n";

	return out;
}