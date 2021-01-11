/******************************************
FILE
    ImageReader.h
PURPOSE
    讀入Image的相關程式

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include <vector>
#include <string>
#include "Picture.h"


using std::string;
using std::vector;
/*
  讀取Image
  傳出一個Picture陣列
*/
vector<Picture> readIMG();
/*
  讀取Light座標之檔案
  傳入：讀取fname檔案
  傳出：將檔案存成string
*/
string readLightFile(const string fname);
/*
  讀取光線位置
  傳入：存有光線座標的字串 圖的名字
  傳出：光線位置
*/
Light readLP(const string lp ,const string imgn );

