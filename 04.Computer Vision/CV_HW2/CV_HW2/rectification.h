/******************************************
FILE
    retificationh.h
PURPOSE
    定義如何將圖形搬正的函式庫

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/

#ifndef _RECTIF_
#define _RECTIF_

#include "Picture.h"
//#include <vector>

//using std::vector;

/*
 取得旋轉的矩陣 R: 3 * 3
 傳入：旋轉軸 旋轉角度
 傳出：旋轉矩陣
*/
double* * getRotation3By3( const double* axis ,double theta  );
/*
  根據一個向量找出他與x軸夾角並求出Rotation matrix 2
  傳入：向量
  傳出：rotation matrix
*/
double* *getRotation2(const double* v );
/*
 image epipole line轉正
 傳入：
     影像 
	 fundamental matrix 
	 translation vector 
	 rotation matrix
 傳出：轉正的影像
*/
vector<Picture> getRectifyImage( vector<Picture>& pics , double* * f ,const double* t ,double* * r );

#endif
