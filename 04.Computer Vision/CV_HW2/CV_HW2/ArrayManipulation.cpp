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
  傳入：row col 初始資料預設為0
  傳出：2D double array
*/
double* 
*create2DArray( int row ,int col ,double initdata) {
	double* *d = new double* [row];
	for ( int i = 0; i < row; i++ ) 
		d[i] = new double[col];
	
	
	for ( int i = 0; i < row; i++ ) 
		for ( int j = 0; j < col; j++ ) 
			d[i][j] = initdata;
		
	return d;
}
/*
  釋放記憶體
  傳入：一個2D array
*/
void dispose(double* * data ,int row ,int col) {
	for ( int i = 0; i < row; i++ ) 
		delete data[i];
	delete data;
}
/*
  將Array從2D轉成1D 此為Column major 以便和Matlab合體
  傳入：2D double array row col值
  傳出：1D double array
*/
double*
_2DDouble2M1D(  double** _2d ,int row ,int col ) {
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
/*
  將Array從1D轉成2D 此為Column major 以便和Matlab合體
  傳入：1D double array 2D double array的row col值
  傳出：2D double array
*/
double* 
*_M1DDouble22D( double* M1d ,int row ,int col ) {
	double* * data = create2DArray(row ,col );
	int index = 0;
	for ( int i = 0; i < col; i++ ) {
		for ( int j = 0; j < row; j++ ) 
			data[j][i] = M1d[index++];
	}

	return data;
}