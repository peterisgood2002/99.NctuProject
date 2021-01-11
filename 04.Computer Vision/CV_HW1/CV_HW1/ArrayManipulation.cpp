/******************************************
FILE
    ArrayManipulation.cpp
PURPOSE
    �w�q�ާ@Array�������禡

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/
#include "ArrayManipulation.h"
#include <iostream>

/*
  �o��@��3D array
  �ǤJ�Grow col
  �ǥX�G3D array
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
  �o��@��2D double array
  �ǤJ�Grow col
  �ǥX�G2D double array
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
  �NArray�q2D�ন1D ����Column major �H�K�MMatlab�X��
  �ǤJ�G2D double array row col��
  �ǥX�G1D double array
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