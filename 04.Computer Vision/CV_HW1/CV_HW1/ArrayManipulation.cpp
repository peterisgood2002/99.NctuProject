/******************************************
FILE
    ArrayManipulation.cpp
PURPOSE
    定義操作Array的相關函式

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include "ArrayManipulation.h"
#include <iostream>

/*
  得到一個3D array
  傳入：row col
  傳出：3D array
*/
Vector3* 
*create3DArray( int row ,int col ) {
	Vector3* *d = new Vector3* [row];
	for ( int i = 0; i < row; i++ ) 
		d[i] = new Vector3[col];
	
	
	for ( int i = 0; i < row; i++ ) 
		for ( int j = 0; j < col; j++ ) {
			d[i][j].x = 0;
			d[i][j].y = 0;
			d[i][j].z = 0;
		}
	return d;
}

/*
  得到一個2D double array
  傳入：row col
  傳出：2D double array
*/
double* 
*create2DArray( int row ,int col ) {
	double* *d = new double* [row];
	for ( int i = 0; i < row; i++ ) 
		d[i] = new double[col];
	
	
	for ( int i = 0; i < row; i++ ) 
		for ( int j = 0; j < col; j++ ) 
			d[i][j] = 0;
		
	return d;
}

/*
  將Array從2D轉成1D 此為Column major 以便和Matlab合體
  傳入：2D double array row col值
  傳出：1D double array
*/
double*
_2DDouble2M1D( double** _2d ,int row ,int col ) {
	double* _1d = new double[ row * col ];
	int i = 0;
	for ( int j = 0; j < col; j++ ) {
		for ( int k = 0; k < row; k++ ) {
			try {
				_1d[i++] = _2d[k][j];
			} catch (char* s ) {
				std::cerr << s;
				exit(-1);
			}
		}
	
	}

	return _1d;
}