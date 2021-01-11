/******************************************
FILE
    Matrix.h
PURPOSE
    �i���Matrix�����B��
	if necessary it'll throw exception and turn off the program

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

#ifndef _MATRIX_
#define _MATRIX_

#include <iostream>


using std::ostream;

/*
  �o��@��2D double array
  �ǤJ�Grow col ��l��ƹw�]��0
  �ǥX�G2D double array
*/
float* *create2DArray( int row ,int col ,float initdata = 2 );
/*
  ����O����
  �ǤJ�G�@��2D array
*/
template < typename type>
void dispose(type* * data ,int row ,int col);


class Matrix {

public:
	/*constructor*/
	Matrix( int row = 3, int col = 3 ,float d = 2);
	Matrix( float* * const d ,int row ,int col);
	//the "Big Three"
    /**
     *Copy Constructor
     *Parameter�Ganother Matrix to copy
     */
    Matrix (const Matrix& m );

    /**
     *Destructor
     */
    ~Matrix();

    /**
     *Overloading the assignment operator
     *Parameter�GMatrix you wanna assign
     */
    Matrix& operator= (const Matrix& m );


public:
	//Arithemetic Operators
    /**
     *Unary negation operator
     */
    Matrix operator-() const;
    /**
     *Overloading the + and += operator
     *Parameter�Ganother Matrix to add
     *Return�G
        if the dimensions are not the same it'll return the zero Matrix
     */
    Matrix operator+( const Matrix& m2 ) const;
    /**
     *Return�Gif the dimensions are not the same
              it'll return the orginal Matrix
     */
    Matrix& operator+= ( const Matrix& m2 );
    /**
     *Overloading the - and  -= operator
     *Parameter�Ganother Matrix to substract
     *Return�G
        if the dimensions are not the same it'll return the zero Matrix
     */
    Matrix operator-( const Matrix& m2 ) const;
    Matrix& operator-= ( const Matrix& m2 );
    /**
     *Matrix Multiplication�G
         it'll overload operator* 
     *Parameter�GMatrix m2 to multiply
	 *Return�G
		if this.col and m2.row are not the same it'll return the zero Matrix
     */
    Matrix operator*( const Matrix& m2 ) const;
	Matrix& operator*= ( const Matrix& m2 );

	//Equality operators
    /**
     *Overloading == operator
     *Parameter�Ganother Matrix wanting comparison
     *Return�G
        if the data and dimension are the same return true
        else return false
     */
    bool operator== (const Matrix& m2) const;
    /**
     *Overloading != operator
     *Parameter�Ganother Matrix wanting comparison
     *Return�G
        if the data and dimension are not the same return true
        else return false
     */
    bool operator!= (const Matrix& m2) const;
	//Operaotr<<
    friend ostream& operator<< (ostream& out ,const Matrix& v );

public:
	/*
	 �]�wCol����
	 �]�w��@��col��row
	 �p�G�j�p���P�N�|�]�w�����\
	*/
	void setCol( const float* col ,int size ,int c );
	void setRow( const float* row ,int size ,int r );
	//�]�w��� data[i][j] = value
	void setData(int i ,int j ,double value );

public:
	/**
	 *Scale all element of Matrix by value
	*/
	void scale( float value );
	/*
	 *Scale one collumn or row 
	  if i is bigger than dimension it won't do anything
	*/
	void scaleCol( int i ,float value );
	void scaleRow( int i ,float value );

public:
	/*
	 ���o�������
	*/
	inline int getColCount() const;
	inline int getRowCount() const;
	float* getOneCol(int i ) const;
	float* getOneRow(int i ) const;
	//get all data
	float* *getDatas() const;
	/*
	 *get data[i][j]
	 *Return�Gif (i ,j) excess the range it'll promt some information and return 0
    */
	inline float getData( int i ,int j ) const;
	/*Get the total amount of the matrix*/
	inline int getElementAmount() const;
	

	//�x�}�򥻹B��
	/* *determinant value
	   *Return�Gdeterminant value
	   *Throws�Gif Matrix is not a square matrix it'll throw exception to stop the program
	*/
	double determinant() const;
	/*
	  *get the getMinor of M( i , j )
	  *Parameter�GM(i ,j) 
	  *Return a getMinor of M( i ,j )
	*/
	Matrix getMinor(int i ,int j) const;
	/**
     *get the cofactor of M(i ,j)
	 *Parameter�GM(i ,j) 
	 *Return�G a cofactor of M( i ,j )
	*/
	double getCofactor(int i ,int j ) const;
	//transpose of matrix
	Matrix transpose() const;
	//�O���O�i�H���ϯx�}
	bool isInvertible() const;
	/**
	 *get Inverse Matrix
	 *Return�GInverse Matrix of this 
	          if it is singular it'll return null
	*/
	Matrix getInverseMatrix() const;
	/**
	 functions will perform following operation
	    Y = M * x
	*/
	/**
	 *Parameter�Garray to muliply
	 *Return: array to store result
	          if it cannot multply it'll return null
	*/
	double* multiplyX( const double* x ,int size) const;
	

	
    
private:
	float* * data;
	int col;
	int row;
};

/**
 *Overloading the global operator<<
*/
ostream& operator<< (ostream& out ,const Matrix& v );


/*=============================
     InLine Function Definition


	 Member functions can be inline, but it cannot be called before its definition due to its internal linkage. 
	 Therefore, inline member functions are usually defined right after class definition in the .h file
===============================*/
inline int 
Matrix::getColCount() const {
	return col;
}
inline int 
Matrix::getRowCount() const {
	return row;
}
/*
 *get data[i][j]
 *Return�Gif (i ,j) excess the range it'll promt some information and return 0
*/ 
using std::cerr;
inline float 
Matrix::getData( int i ,int j ) const {
	
	if ( i < 0 || i >= row ) {
		cerr << "Matrix::getData() error�Gthe parameter i excess the row dimension\n";
		return 0;
	}
	if ( j < 0 || j >= col ) {
		cerr << "Matrix::getData() error�Gthe parameter j excess the row dimension\n";
		return 0;
	}

	return data[i][j];
}
/*Get the total amount of the matrix*/
int 
Matrix::getElementAmount() const {
	return row * col;
}
#endif
