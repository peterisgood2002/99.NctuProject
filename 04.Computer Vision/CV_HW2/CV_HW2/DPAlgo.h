/******************************************
FILE
    DPAlgo.h
PURPOSE
    進行Dynamic programming algorithm的一些函式
	

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/

#ifndef _DPALGO_
#define _DPALGO_

#include <windows.h>
#include <gdiplus.h>
#include <vector>

using Gdiplus::Color;
using std::vector;

typedef vector<Color> WColor;

enum Direction { COL=-3 ,UP=-3 ,ROW=-2  ,LEFT=-2 ,D ,NOTDIR = 0};//表是他的最小值是從上面或是左邊或是對角線來的
const double NOTANUMBER = System::Double::MaxValue;
/*
 從Color中找尋最接近的點 以下列為例
 double l[4] = { 4 ,3 ,2 ,1};
 double r[4] = { 5 ,4 ,2 ,1};
 其輸出為
        5 4 2 1
      4
	  3
	  2
	  1
 每一個cell表示他的下一筆資料該怎麼跑
 傳入：左圖和右圖的epipolar line之color值
 傳出：Dynamic programming的結果 
*/
double* *dpAlgorithm(const vector<WColor*>& l , const vector<WColor*>& r ); 
/*
 透過DP algorithm結果 找出相對應的資料
 傳入：DP algo結果 
      要以row為準 找相對應資料還是col d = UP表以row d = LEFT表以col
 傳出：以d為準的相對應結果
*/
int* getCorrespondence(double* * dpresult ,int row ,int col ,Direction d);
/*
  負責計算cost用的function 如果有需要請自行override
  傳入：兩筆資料
  傳出：cost
*/
double costFunc(const double* l ,const double* r ,int wsize);
/*
 Dynamic programming主要實作處
 傳入：兩個陣列以及window size
 傳出：Dynamic programming結果
*/
double* *dpAlgorithm(const vector<double*>& left ,const vector<double*>& right ,int wsize);
/*Output data 用 偵錯用*/
void printData(double* * const data ,int r ,int c);

/*測試用*/
double* * dpAlgorithm(double* l ,int llength ,double* r ,int rlength );
#endif