/******************************************
FILE
    Stereopsis.h
PURPOSE
    定義struct from motion需要的相關資訊

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/

#ifndef _STEREOPSIS_
#define _STEREOPSIS_

#include "ArrayManipulation.h"
#include "Picture.h"
/*
  根據兩張圖取得3D座標
  傳入：
      兩張圖 分別代表左圖和右圖 
	  Optical center位移多少 
	  要對應的windows size多大
	  以哪一個圖為基準預設為以左圖(i = 0)為準 i = 1為右圖
	  焦距
  傳出：根據哪一個圖傳出3D座標
*/
Vertex* * get3DValue(Picture& pl , Picture& pr ,double T, int wwidth = 1 ,int wheight = 1 ,int rorc = 0 ,double f = 1);

#endif