/******************************************
FILE
    PhotoStereo.h
PURPOSE
    宣告Photometric Stereo的相關函式

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include <vector>
#include "Picture.h"
#include "ArrayManipulation.h"
using std::vector;


/*
  建立一個HeightMap
  傳入：要建立HeightMap的相關img
  傳出：Heightmap
*/
Vector3** buildNormalMap( const vector<Picture>& imgs );
/*
  對(x ,y)進行Photometric stereo算出未normalize之normal
  傳入：( x ,y ) 還有圖檔
  傳出：未normalize之normal
*/
Normal calNormal( int x ,int y ,const vector<Picture>& imgs );
/*
  將Normal normalize
  傳入：一個尚未Normalize的normal
  傳回：改變normal並傳回Normalize的結果
*/
Normal* *normalizeN( Normal* * map ,int row ,int col);
/*
  找到(-df/dx ,-df/dy ,1 )
  傳入：一個尚未Normalize的normal row col
  傳回：改變normal並傳回Normalize的結果
*/
Normal* *findF( Normal* * map ,int row ,int col);
/*
  建立Height Map
  傳入：Normal Map
  傳出：Height Map
*/
double* *buildHeightMap( Normal* *nmap ,int row ,int col );
/*
  進行Correlation
  ----------->x
  |
  |
  |
  |
  y
  傳入：height map filter 要filter的位置
  傳出：coorelation的結果

*/
double* *smoothHMap( double* * hmap ,int row ,int col );
