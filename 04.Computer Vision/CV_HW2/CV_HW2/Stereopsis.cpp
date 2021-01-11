/******************************************
FILE
    Stereopsis.cc
PURPOSE
    struct from motion需要的相關資訊

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include "Stereopsis.h"
#include "DPAlgo.h"
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <cmath>
#include <iostream>

using std::cout;
using std::vector;
using Gdiplus::Color;
/*
  根據兩張圖取得3D座標 計算的方法如下
  利用DP找出對應點後假設是(u ,v) (U ,V)
  d = | u - U |
  再利用三角化法
  z = -f(B/d)
  P = -f(B/d) * p
  傳入：
      兩張圖 分別代表左圖和右圖 
	  Optical center位移多少
	  要對應的windows size多大
	  以哪一個圖為基準預設為以左圖(i = 0)為準 i = 1為右圖
  傳出：根據哪一個圖傳出3D座標
*/
Vertex* * 
get3DValue(Picture& pl , Picture& pr ,double T, int wwidth ,int wheight ,int rorc ,double f){
	int width = pl.getWidth();
	int height = pl.getHeight();
	
	if ( rorc != 0 ) {
		width = pr.getWidth();
		height = pr.getHeight();
	}

	//初始化Vertex
	Vertex* * v = create3DArray ( height ,width);
	for ( int i =0; i < height; i++ ) {
		printf("取出第%d列\n",i);
		vector<WColor*>* lc = pl.getOneLineColors(i ,wwidth ,wheight);
		vector<WColor*>* rc = pr.getOneLineColors(i ,wwidth ,wheight);
		cout<<"取出完畢\n";

		double* * dpresult = dpAlgorithm(*lc ,*rc );
		printf("計算第%d列之Correspondence\n",i);
		int* cor  = 0;
		if ( rorc == 0 ) cor = getCorrespondence(dpresult ,(int)lc->size() ,(int)rc->size() ,ROW);//以左邊為準
		else cor = getCorrespondence(dpresult ,(int)lc->size() ,(int)rc->size() ,COL);//以左邊為準
		cout<<"計算Correspondence結束\n";
		
		/*for ( size_t x = 0; x < lc->size(); x++ ) 
			cout<< cor[x] <<" ";*/
		/*算出3D座標值*/

		for ( int j = 0; j < (int)lc->size(); j++ ) {
			if ( cor[j] != (int)NOTANUMBER ){
				int diff = (int)abs( cor[j] - j );
				if ( diff != 0 ) {
					double z = f * T / diff;
					v[i][j].x = z * j / f;
					v[i][j].y = z * i / f;
					v[i][j].z = z * 1;
				} else {
					if ( j > 0 ) {
						v[i][j].x = v[i][j-1].x;
						v[i][j].y = v[i][j-1].y;
						v[i][j].z = v[i][j-1].z;
					}
				}
			}
			else {
				if ( j > 0 ) {
						v[i][j].x = v[i][j-1].x;
						v[i][j].y = v[i][j-1].y;
						v[i][j].z = v[i][j-1].z;
				}
			}
			

		}
		
		delete cor;
		for ( size_t z = 0; z < lc->size(); z++ ) {
			WColor* tmp = (*lc)[z];
			delete tmp;
			tmp = (*rc)[z];
			delete dpresult[z];
		}

		delete lc;
		delete rc;
		delete dpresult;
		cout<<"==================\n";
	}

	return v;
}
