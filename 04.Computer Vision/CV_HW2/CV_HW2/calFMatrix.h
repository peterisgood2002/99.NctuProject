/******************************************
FILE
    calFMatrix.h
PURPOSE
    定義如何計算fundamental matrix相關資訊
	F = T * R
	並求出T R

NOTE
	1.透過下面公式計算F
      p'：image 2 的某點
      p：image 1 的某點
      p' 垂直於T * p'
      因此在沒有內部參數的情況下
      0 = p' (T * p') = p' [T * (Rp + T) ] = p' (T * R)   p = p' E  p
      在有內部參數的情況下
      0 = p'  (K' E K)  p = p' F  p
    2.正規化correspondent point可以取得更加的值
      距離不會超過sqrt(2) 也就是
      scale = sqrt(2) / Avgdis
          [     scale           0     -avg*scale ]
	   S =[       0           scale   -avg*scale ]
	      [       0             0		   1     ]

AUTHOR
    9757553 顏志晟
*******************************************/

#ifndef _CALF_
#define _CALF_

#include <vector>
#include <string>
#include "Picture.h"
#include "Struct.h"

using std::vector;
using std::string;

/*
  取得CorPoint中各項的平均值
  傳入：所有的點對應集合
  傳出：一個CorPoint儲存各對應之平均值
*/
CorPoint* getAverage( const vector<CorPoint>& cp );
/*
 正規化correspondent point好進行運算
 距離不會超過sqrt(2) 也就是
 scale = 影像高度或寬度
 
 傳入：點座標 相對應點所有的平均值 以及圖檔的高度寬度
 傳出：normalize過後的點座標 以及Normalize過後的矩陣 nl nr
*/
vector<CorPoint> normalizeCP( const vector<CorPoint>& cp  ,double imgw ,double imgh ,double* * nl ,double* * nr ,CorPoint* avg = NULL);

/*
  透過下面公式計算F
  p'：image 2 的某點
  p：image 1 的某點
  p' 垂直於T * p'
  因此在沒有內部參數的情況下
  0 = p' (T * p') = p' [T * (Rp + T) ] = p' (T * R)   p = p' E  p
  在有內部參數的情況下
  0 = p'  (K' E K)  p = p' F  p
  傳入：correspondent point
  傳出：Fundamental matrix
*/
double* *calF(const vector<CorPoint>& cp );
/*
 計算K' F K
 傳入：左邊的normalize matrix Fundamental matrix 右邊的normalize
 傳出：算出的值
*/
double* *calKFK( double* *nl , double* * f , double* *nr ,int row ,int col);
/*
  根據fundamental matrix將T解出 透過對應點解出方向
  公式為：
    sum(T * Xi') * (F Xi) < 0 ==> T =  -T
  傳入：fundamental matrix
  傳出：translation vector
*/
double* calT(double* * f ,const vector<CorPoint>& cp ,int row ,int col);
/*
  根據fundamental matrix和T來解 R
  公式：
    R = W = [ E1 * T + E2 * T        E2 * T + E3 * E1      E3 * T + E1 * E2 ]
	有noise ==> || R - W|| 最小(目前並沒有做)
  傳入：fundamental matrix translation vector
  傳出：R
*/
double* *calR(double* * f ,const double* t ,bool noise = false);
/*
  根據fundamental matrix 在圖上畫出一條Epipolar line
  傳入：圖 檔名 fundamental matrix 對應點 以及這一張圖是左圖還是右圖 
*/
void drawEpiLine(const Picture& p ,const string fname ,double* * f ,vector<CorPoint>& cp ,bool isL = true);
#endif