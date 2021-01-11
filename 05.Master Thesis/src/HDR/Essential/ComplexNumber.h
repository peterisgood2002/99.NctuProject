/******************************************
FILE
    ComplexNumber.h
PURPOSE
    define the relative class of complex number
	it includes the matrix and array of complex number
	


NOTE
	

AUTHOR
    9757553 ÃC§ÓÑÔ
*******************************************/

#ifndef _COMPLEX_
#define _COMPLEX_

#include <iostream>

using std::ostream;

/*==========================
 *ComplexNumber«Å§i
   represent a complex number
============================*/
class ComplexNumber{

public:
	ComplexNumber( double real = 0 , double imagery = 0 );
	void setReal( double r );
	void setImagery( double i );
	void setData( double r ,double i );
	double getReal() const;
	double getImagery() const;
	double getLength() const;

public:
	/*operator overloading*/
	/*perform s * R = (s*real + i s* imagery*/
	ComplexNumber operator* ( double scale ) const; 
	ComplexNumber& operator*= ( double scale );
	ComplexNumber operator* ( const ComplexNumber& c2 ) const;
	ComplexNumber& operator*= ( const ComplexNumber& c2 );
	ComplexNumber operator/ ( double scale ) const;
	ComplexNumber& operator/= ( double scale );
	ComplexNumber operator+ ( const ComplexNumber& r1 ) const;
	ComplexNumber& operator+= ( const ComplexNumber& r1);
	ComplexNumber operator- ( const ComplexNumber& r1 ) const;
	ComplexNumber& operator-= ( const ComplexNumber& r1);
	


private:
	double real;
	double imagery;
};
/*purpose for Debug*/
ostream& operator<< ( ostream& out ,const ComplexNumber& r );

/*=============================
 *CNumberArray«Å§i
  represent an array of ComplexNumber
==============================*/
class CNumberArray {

public:
	CNumberArray( int size = 3 ,const double* data = NULL);
	CNumberArray( const ComplexNumber* d ,int size );
	/*the big three*/
	CNumberArray( const CNumberArray& c );
	CNumberArray& operator= ( const CNumberArray& c );
	~CNumberArray();

public:
	/*set information*/
	/*
	 according to index set the data 
	*/
	void setData( int index ,double r = 0,double imagery = 0 );
	void setData( int index , const ComplexNumber& c );
	/*
	 reset the size 
	 *Parameter:
	  *the array new size
		if the size is bigger than original 
		it'll extend the original array and append zeros
		if the size is smaller than original 
		it'll truncate it to new size
	*/
	void resetSize( int nsize );
public:
	ComplexNumber getData(int i) const;
	int getSize() const;

public:
	ComplexNumber& operator[] (int i) const;
	/**
     *Pixel-wise Multiplication¡G
         it'll overload operator* 
     *Parameter¡GMatrix m2 to multiply
	 *Return¡G
		if this.col and m2.row are not the same it'll return the zero Matrix
     */
    CNumberArray operator*( const CNumberArray& a2 ) const;
	CNumberArray& operator*= ( const CNumberArray& a2 );
	/**
     *Overloading the + and += operator
     *Parameter¡Ganother Vector to add
     *Return¡G
        if the dimensions are not the same it'll return the zero Vector
     */
    CNumberArray operator+( const CNumberArray& a2 ) const;
    CNumberArray& operator+= ( const CNumberArray& a2 );
	/**
     *Overloading the + and += operator
     *Parameter¡Ganother Vector to add
     *Return¡G
        if the dimensions are not the same it'll return the zero Vector
     */
    CNumberArray operator-( const CNumberArray& a2 ) const;
    CNumberArray& operator-= ( const CNumberArray& a2 );

public:
	friend CNumberArray operator* ( const ComplexNumber& c , const CNumberArray& a2 );
	friend CNumberArray operator* ( const CNumberArray& a2 , const ComplexNumber& c );
private:
	ComplexNumber* data;
	int size;
	
};
/*it'll implement c * a2 and a2 * c*/
CNumberArray operator* ( const ComplexNumber& c , const CNumberArray& a2 );
CNumberArray operator* ( const CNumberArray& a2 , const ComplexNumber& c );
ostream& operator<< ( ostream& out ,const CNumberArray& r );

/*=============================
 *CNumberArray«Å§i
  represent an 2D array of ComplexNumber
==============================*/
class CNumberMatrix {

public:
	CNumberMatrix( int row ,int col ,float* * const d = NULL);
	/*
	 *Consider the d array as one row 
	  Be sure that every size of d array should be the same or it'll abort the programs
	 *Parameter¡Garray with the same size of each element
	*/
	CNumberMatrix( const CNumberArray* d ,int r);
	/*the big three*/
	CNumberMatrix( const CNumberMatrix& c );
	CNumberMatrix& operator= ( const CNumberMatrix& c );
	~CNumberMatrix();

public:
	int getRowSize() const;
	int getColSize() const;
	CNumberArray getRow( int i ) const;
	CNumberArray getCol( int i ) const;
	

public:
	/*Set the row[i] into c. it is just a replacement*/
	void setRow( int index ,const CNumberArray& r );
	/*If the c.size is larger than the rows it'll extend the row to match it
	  If the c.size is bigger than the rows it'll set the rest to zero
	*/
	void setCol( int index ,const CNumberArray& c );
	/*Reset row and col
	 *Parameter:
	  *the array new row or col
		if the size is bigger than original 
		it'll extend the original array and append zeros
		if the size is smaller than original 
		it'll truncate it to new size
	
	*/
	void resetRow( int r );
	void resetCol( int c );
	void resetSize( int r ,int c );


public:
	CNumberArray& operator[] ( int i ) const;
	/**
     *Pixel-Wise Multiplication¡G
         it'll overload operator* 
     *Parameter¡GMatrix m2 to multiply
	 *Return¡G
		if this.col and m2.row are not the same it'll return the zero Matrix
     */
    CNumberMatrix operator*( const CNumberMatrix& m2 ) const;
	CNumberMatrix& operator*= ( const CNumberMatrix& m2 );

private:
	CNumberArray* data;
	int row;
	int col;

};
ostream& operator<< ( ostream& out ,const CNumberMatrix& m );

#endif