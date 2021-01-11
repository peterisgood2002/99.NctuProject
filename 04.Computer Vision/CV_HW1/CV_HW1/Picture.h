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
#pragma comment(lib, "gdiplus.lib")

using std::string;
using namespace Gdiplus;

//光線位置
struct Light {
public:
	double x;
	double y;
	double z;
};

//圖檔
class Picture {
private:
	Bitmap* image;//影像檔
	Light lightc;//光線在哪裡
	string fname;
	string dir;
public:
	static void initGDIPlus();//初始化GDI+
	/*Constructor*/
	/*
	  傳入：圖檔檔名 光線位置
	*/
	Picture(const string imgfn ,const Light& lc);
	
	/*Destructor*/
	~Picture();

	/*畫圖確定有寫進去用*/
	void drawPicture(const string fn);

	/*
	  Get data member function
	*/
	inline int getHeight() const { return image->GetHeight(); }
	inline int getWidth() const{ return image->GetWidth(); }
	inline Light getLight() const{return lightc;}
	Color getColor(int x ,int y ) const;
	string getDirName() const { return dir; }
};

#endif