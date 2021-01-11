/******************************************
FILE
    readData.h
PURPOSE
    從檔案中讀取相關資料

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/

#ifndef _READDATA_
#define _READDATA_

#include <vector>
#include <string>
#include "Picture.h"
#include "Struct.h"
using std::vector;
using std::string;



/*
  取得兩張圖的點對應 以方便找尋F
  傳入：correspondence 的檔案位置
  傳出：相對應的點座標集合
*/
vector<CorPoint> getCorrespondPoint(const string cfile); 
/*
  讀取資料夾下的圖檔
  傳入：資料夾名稱
  傳出：圖檔集合
*/
vector<Picture>  getPicture( const string dname );
#endif