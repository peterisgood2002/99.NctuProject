/******************************************
FILE
    Matrix.cpp
PURPOSE
    進行基本Matrix相關運算

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include <exception>
#include <iostream>
#include <cmath>
#include "Matrix.h"


using std::exception;
using std::cerr;

/*
  得到一個2D double array
  傳入：row col 初始資料預設為0
  傳出：2D double array
*/
float* 
*create2DArray( int row ,int col ,float initdata) {
	float* *d = new float* [row];
	for ( int i = 0; i < row; i++ ) 
		d[i] = new float[col];
	
	
	for ( int i = 0; i < row; i++ ) 
		for ( int j = 0; j < col; j++ ) 
			d[i][j] = initdata;
		
	return d;
}
/*
  釋放記憶體
  傳入：一個2D array
*/
template < typename type>
void dispose(type* * data ,int row ,int col) {
	for ( int i = 0; i < row; i++ ) 
		delete [] data[i];

	delete [] data;
}


//Constructor
Matrix::Matrix(int row , int col ,float d ) {
	if ( row <= 0 || col <= 0 ) 
		throw exception("Matrix cannot have a negative row or col");
	this->col = col;
	this->row = row;
	data = create2DArray( row , col ,d);

}
Matrix::Matrix( float* * const d ,int r ,int c) {
	if ( r <= 0 || c <= 0 ) 
		throw exception("Matrix cannot have a negative row or col");

	row = r;
	col = c;
	
	data = create2DArray( r , c );

	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			data[i][j] = d[i][j];
		}
	}
}
/*The Big Three*/
/**
 *Copy Constructor
 *Parameter：another Matrix to copy
 */
Matrix::Matrix (const Matrix& m ) {
	row = m.row;
	col = m.col;
	data = create2DArray( row , col );

	for ( int i = 0; i < row; i++ ) 
		for ( int j = 0; j < col; j++ )
			data[i][j] = m.data[i][j];
}

/*Destructor*/

Matrix::~Matrix() {
	dispose( data ,row ,col);
}
/**
 *Overloading the assignment operator
 *Parameter：Matrix you wanna assign
*/
Matrix& 
Matrix::operator=( const Matrix& m) {

	if (this == &m ) return *this;

    dispose( data ,row ,col );//delete original array

	row = m.row;
	col = m.col;
	data = create2DArray( row , col );

	for ( int i = 0; i < row; i++ ) 
		for ( int j = 0; j < col; j++ )
			data[i][j] = m.data[i][j];
	
	return *this;
}

//Arithemetic Operators
/**
 *Unary negation operator
 */
Matrix 
Matrix::operator-() const {
	float* * d = create2DArray( row , col );
	for ( int i = 0 ;i < row; i++ ) 
		for ( int j = 0; j < col; j++ )
			d[i][j] = - data[i][j];

	Matrix tmp( d ,row ,col );

	dispose(data ,row ,col);

	return tmp;
}
/**
 *Overloading the + and += operator
 *Parameter：another Matrix to add
 *Return：
    if the dimensions are not the same it'll return the zero Matrix
 */
Matrix 
Matrix::operator+( const Matrix& m2 ) const {
	if ( row == m2.row && col == m2.col ) {
		float* * d = create2DArray( m2.row ,m2.col );
		 
		for ( int i = 0; i < row; i++ ) 
			for ( int j = 0; j < col; j++ ) 
				d[i][j] = data[i][j] + m2.data[i][j];
			
			
		Matrix m (d ,row ,m2.col );
		dispose(d ,m2.row ,m2.col );
		return m;
	}else {
		cerr << "addition between differenct dimension you will get zero Matrix\n";
		return Matrix( m2.row ,m2.col );
	}
}
/**
 *Return：if the dimensions are not the same
          it'll return the orginal Matrix
 */
Matrix& 
Matrix::operator+= ( const Matrix& m2 ) {
	if ( row == m2.row && col == m2.col ) {
		 
		for ( int i = 0; i < row; i++ ) 
			for ( int j = 0; j < col; j++ ) 
				data[i][j] += m2.data[i][j];
			
	}else {
		cerr << "addition between differenct dimension you will get addend Matrix\n";
		
	}

	return *this;
}
/**
 *Overloading the - and  -= operator
 *Parameter：another Matrix to substract
 *Return：
    if the dimensions are not the same it'll return the zero Matrix
 */
Matrix 
Matrix::operator-( const Matrix& m2 ) const {
	if ( row == m2.row && col == m2.col ) {
		float* * d = create2DArray( m2.row ,m2.col );
		 
		for ( int i = 0; i < row; i++ ) 
			for ( int j = 0; j < col; j++ ) 
				d[i][j] = data[i][j] - m2.data[i][j];
		Matrix m (d ,row ,col );

		dispose(d ,m2.row ,m2.col );
		return m;
	}
	
	cerr << "subtract between differenct dimension you will get zero Matrix\n";
	return Matrix( m2.row ,m2.col );
	
}
Matrix& 
Matrix::operator-= ( const Matrix& m2 ) {
	if ( row == m2.row && col == m2.col ) {
		 
		for ( int i = 0; i < row; i++ ) 
			for ( int j = 0; j < col; j++ ) 
				data[i][j] -= m2.data[i][j];
			
	}
	else cerr << "subtract between differenct dimension you will get subtrahend Matrix\n";

	return *this;
}
/**
 *Matrix Multiplication：
     it'll overload operator* 
 *Parameter：Matrix m2 to multiply
 *Return：
		if this.col and m2.row are not the same it'll return the zero Matrix
 */
Matrix 
Matrix::operator*( const Matrix& m2 ) const {
	if ( col == m2.row ) {
		float* * d = create2DArray( row ,m2.col );
		 
		for ( int i = 0; i < row; i++ ) 
			for ( int j = 0; j < m2.col; j++ ) 
				for ( int k = 0; k < col; k++ ) 
					d[i][j] += data[i][k] * m2.data[k][j];
			
		Matrix m (d ,row ,m2.col );
		dispose(d ,m2.row ,m2.col );
		return m;
	}
	
	cerr << "subtract between differenct dimension you will get zero Matrix\n";
	return Matrix( row ,m2.col );
}
Matrix& 
Matrix::operator*= ( const Matrix& m2 ) {
	
	if ( col == m2.row ) {
		float* * d = create2DArray( row ,m2.col );

		for ( int i = 0; i < row; i++ ) 
			for ( int j = 0; j < m2.col; j++ ) {
				double total = 0;
				for ( int k = 0; k < col; k++ ) 
					total += data[i][k] * m2.data[k][j];

				d[i][j] = total;
			}

		for ( int i = 0; i < row; i++ )
			for ( int j = 0; j < m2.col; j++)
				data[i][j] = d[i][j];
	}
	else cerr << "multiplication cannot be operated you will get original matrix\n";
	return *this;
}

//Equality operators
/**
 *Overloading == operator
 *Parameter：another Matrix wanting comparison
 *Return：
    if the data and dimension are the same return true
    else return false
 */
bool 
Matrix::operator== (const Matrix& m2) const {
	
	if ( row == m2.row && col == m2.col ) {
		for ( int i = 0; i < row; i++ ) 
			for ( int j = 0; j < col; j++ )
				if ( data[i][j] != m2.data[i][j] )
					return false;

		return true;

	}
	return false;

}
/**
 *Overloading != operator
 *Parameter：another Matrix wanting comparison
 *Return：
    if the data and dimension are not the same return true
    else return false
 */
bool 
Matrix::operator!= (const Matrix& v2) const {
	if ( *this == v2 ) return false;

	return true;
}





/**
 *Overloading the global operator<<
*/
ostream& operator<< (ostream& out ,const Matrix& v ) {
	out << "Matrix：\n";

	for ( int i = 0; i < v.row; i++ ) {
		for ( int j = 0; j < v.col; j++ ) {
			out << v.data[i][j] <<" ";
		}

		out <<"\n";
	}

	return out;
}

/*
 設定Col的值
 //設定單一個col或row
*/
void 
Matrix::setCol( const float* col ,int size ,int c ) {
	if ( size == row ) {
		for ( int i = 0; i < size; i++ )
			data[i][c] = col[i];
	}

	return;

}
void 
Matrix::setRow( const float* row ,int size ,int r ) {
	if ( size == col ) {
		for ( int i = 0; i < size; i++ )
			data[r][i] = row[i];
	}

	return;
}
//設定資料
void 
Matrix::setData(int i ,int j ,double value ) {
	if ( i < 0 || i >= row ) {
		cerr << "the parameter i excess the row dimension\n";
		return;
	}
	if ( j < 0 || j >= col ) {
		cerr << "the parameter j excess the row dimension\n";
		return;
	}
	data[i][j] = value;
}
/**
 *Scale all element of Matrix by value
*/
void 
Matrix::scale( float value ) {
	for ( int i = 0; i < row; i++ )
		for ( int j = 0; j < col; j++ ) 
			data[i][j] *= value;
}
/*
 *Scale one collumn or row 
  if i is bigger than dimension it won't do anything
*/
void 
Matrix::scaleCol( int i ,float value ) {
	if ( i >= 0 && i < col ) {
		for ( int j = 0; j < row; j++ )
			data[j][i] *= value;
	}
	else cerr <<"there is an error to scale col. do nothing\n";
}
void 
Matrix::scaleRow( int i ,float value ) {
	if ( i >= 0 && i < col ) {
		for ( int j = 0; j < row; j++ )
			data[i][j] *= value;
	}
	else cerr <<"there is an error to scale col. do nothing\n";
}
/*
 取得相關資料
*/ 
float* 
Matrix::getOneCol( int i ) const {
	if ( i >= 0 && i < col ) {
		float* d = new float[row];
		for ( int j = 0; j < row; j++ ) 
			d[j] = data[j][i];

		return d;
	}else {
		cerr << "pay attention you got a NULL COL\n";
		return NULL;
	}
	
}
float* 
Matrix::getOneRow( int i ) const {
	if ( i >= 0 && i < row ) {
		float* d = new float[col];
		for ( int j = 0; j < col; j++ ) 
			d[j] = data[i][j];

		return d;
	}
	else {
		cerr << "pay attention you got a NULL ROW\n";
		return NULL;
	}
}
float* *
Matrix::getDatas() const {
	float* * d = create2DArray(row ,col );

	for ( int i = 0; i < row; i++ ) 
		for ( int j = 0; j < col; j++ )
			d[i][j] = data[i][j];

	return d;
}

//矩陣基本運算
/**
 *determinant value
 *Return：determinant value
 *Throws：if Matrix is not a square matrix it'll throw exception to stop the program
*/
double 
Matrix::determinant() const{
	if ( row == col ) {
		if ( row == 1 ) return data[0][0];

		//m * m
		double total = 0;
		for ( int i = 0; i < row; i++ ) //統一對第一行進行降階
				total += data[i][0] * getCofactor( i ,0 );

		return total;
	}
	
	throw exception("the matrix is not a square matrix\n");

}

/*
  *get the getMinor of M( i , j )
   definiton is as follows http://en.wikipedia.org/wiki/Minor_%28matrix%29
  *Parameter：x coordinate and y- coordinate
  *Return a getMinor of M( i ,j )
*/
Matrix 
Matrix::getMinor(int i ,int j) const{
	float* *d = create2DArray( row-1 , col-1 );

	
	for ( int ti = 0 ,k = 0; ti < row; ti++ ) {
		if ( ti == i ) continue;
		
		for ( int tj = 0 ,l = 0; tj < col ;tj++ ) {
			if ( tj == j ) continue;
			
			d[k][l++] = data[ti][tj];
			
		}
		k++;
	}

	Matrix m(d ,row-1 ,col-1 );

	dispose(d , row-1 ,col-1 );

	return m;

}
/**
 *get the cofactor of M(i ,j)
 *Parameter：M(i ,j) 
 *Return： a cofactor of M( i ,j )
*/
double 
Matrix::getCofactor(int i ,int j ) const{
	Matrix m = getMinor( i , j );
	return  pow(-1.0 , i+j )* m.determinant();
}
//transpose of matrix
Matrix 
Matrix::transpose() const{
	float* * d = create2DArray( row , col );

	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			d[i][j] = data[j][i];
		}
	}

	Matrix m(d ,row ,col );

	dispose(d , row ,col );

	return m;

}
//是不是可以有反矩陣
bool 
Matrix::isInvertible() const{
	return ( determinant() != 0 )? true : false;
}
/**
 *get Inverse Matrix
 *Return：Inverse Matrix of this
          if it is singular it'll return default matrix
*/
Matrix 
Matrix::getInverseMatrix() const{
	if ( !isInvertible() ) {
		cerr <<"it is singular it'll return default matrix\n";
		return Matrix();
	}

	double det = determinant();

	float* * d = create2DArray( row , col );

	for ( int i = 0; i < row; i++ ) 
		for ( int j = 0; j < col; j++ ) 
			d[i][j] = getCofactor( j ,i ) / det;
		
	Matrix m(d ,row ,col );

	dispose(d , row ,col );

	return m;
	
}

/**
 functions will perform following operation
    Y = M * x
*/
/**
 *Parameter：array to muliply
 *Return:array to store result
*/
double* 
Matrix::multiplyX( const double* x ,int size) const{
	double* result = NULL;

	if ( size == col ) {
		result = new double[size];
		for ( int i = 0; i < size; i++ )
			result[i] = 0;
		
		for ( int i = 0; i < size; i++ ) {
			float* _1row = getOneRow(i);
			
			for ( int j = 0; j < size; j++ ) 
				result[i] += _1row[j] * x[j];
			
			delete [] _1row;
			
		}
	}

	return result;
}
