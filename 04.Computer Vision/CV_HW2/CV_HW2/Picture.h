/******************************************
FILE
    Picture.h
PURPOSE
    宣告Picture class用來進行以後處理用
	

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/

#ifndef _IMAGE_
#define _IMAGE_

#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>

#pragma comment(lib, "gdiplus.lib")

using std::string;
using std::vector;
using namespace Gdiplus;

typedef vector<Color> WColor;

//圖檔
class Picture {
private:
	Bitmap* image;//影像檔
	string fname;
	string dir;
public:
	static void initGDIPlus();//初始化GDI+
	/*Constructor*/
	/*
	  傳入：圖檔檔名 光線位置
	*/
	Picture(const string imgfn );
	/*Copy Constuctor*/
	Picture( const Picture& pic );
	/*Destructor*/
	~Picture();

	/*畫圖確定有寫進去用*/
	void drawPicture(const string fn);
	
	/*
	  Get data member function
	*/
	inline int getHeight() const { return image->GetHeight(); }
	inline int getWidth() const{ return image->GetWidth(); }
	Color getColor(int x ,int y ) const;
	/*
	  取得一個(x ,y)周圍的資訊
	  傳入：座標 以及要取多少
	  傳出：一個vector
	*/
	WColor* getWindowPixels(int x ,int y ,int wheight ,int wwidth) const;
	/*
	  取得一行的資料
	  傳入：要取的行號為何 windows size多大
	  傳出：一個有windows size個vector之color的array
	*/
	vector<WColor*>* getOneLineColors( int l ,int wheight ,int wwidth) const;
	string getDirName() const { return dir; }

	/*
	  Set data member
	*/
	void setColor(int x ,int y ,const Color &c );

	
};

#endif