/******************************************
FILE
    ArrayManipulation.h
PURPOSE
    定義操作Array的相關函式

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/

#ifndef _ARRAYMANI_
#define _ARRAYMANI_

#include "Struct.h"

/*
  得到一個3D array
  傳入：row col
  傳出：3D array
*/
Vector3* *create3DArray( int row ,int col );
/*
  得到一個2D double array內定值為0
  傳入：row col 初始資料預設為0
  傳出：2D double array
*/
double* *create2DArray( int row ,int col ,double initdata = 0);
/*
  釋放記憶體
  傳入：一個2D array
*/
void dispose(double* * data ,int row ,int col);
/*
  將Array從2D轉成1D 此為Column major 以便和Matlab合體
  傳入：2D double array row col值
  傳出：1D double array
*/
double* _2DDouble2M1D( double** _2d ,int row ,int col );
/*
  將Array從1D轉成2D 此為Column major 以便和Matlab合體
  傳入：1D double array 2D double array的row col值
  傳出：2D double array
*/
double* *_M1DDouble22D(  double* M1d ,int row ,int col );


#endif