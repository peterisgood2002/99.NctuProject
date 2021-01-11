/******************************************
FILE
    HDRLoader.h
PURPOSE
    宣告讀入HDR格式的class

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/

#ifndef _HDRLOADER_
#define _HDRLOADER_

#include <string>
#include <fstream>
#include "../ColorSpace/ColorSpaceTransformation.h"
#include "Loader.h"

using std::ifstream;
using std::string;

//struct RealColor {
//public:
//	float R;
//	float G;
//	float B;
//	float E;
//};

class HDRLoader : public Loader {
	
public:
	/*Constuctor*/
	HDRLoader();
	HDRLoader( const string& fname );
	//the "Big Three"
	/*Copy Constructor*/
	HDRLoader( const HDRLoader& loader);

	/**Overloading the assignment operator*/
	HDRLoader& operator= (const HDRLoader& v );

public:
	/*Destructor*/
	~HDRLoader();

public:
	/*
	 讀入HDR 影像 RGBE的影像格式
	 傳入：HDR影像的file name
	 傳出：讀取是否成功
	 Exception：若讀入檔案失敗 將會傳出Exception
	*/
	bool readImage( const string& fname );
	
	
	/*
	 根據Greg Ward在Graphics Gems的REAL Pixel這一篇取得(x ,y )的RGBE資料
	 因為她存的是整數 但是其實他是以科學記號表示法表示的
	 如果取錯值 他會回傳NOTANUMBER 並丟出錯誤訊息

	  --------------->x
	  |
	  |
	  |
	  |
	  v
	  
	  y
	*/
	double getRed( int  x ,int y) const;
	double getGreen( int  x ,int y ) const;
	double getBlue( int  x ,int y ) const;
	double getX( int  x ,int y) const;
	double getY( int  x ,int y ) const;
	double getZ( int  x ,int y ) const;
	double getL( int x ,int y ) const;
	double getA( int x ,int y ) const;
	double getB( int x ,int y ) const;
	double getBright( int x ,int y ) const;
	double getLuminance( int x ,int y ) const;
	double getAvgL() const;
	

	/*
	 設定資料
	 如果設定不成功 將會甚麼事都不做
	*/
	void scaleLuminanceData(int x ,int y ,float scale );
	/*
	 儲存圖片
	*/
	void saveImage(const string& fname);
	

protected:
	//初始化相關資料
	void init();

private:
	static const float DEFAULT;
	static const float NOTANUMBER;
	static const string RGB;
	static const string XYZ;
	enum C{ RED = 0 ,GREEN ,BLUE ,X = 0 ,Y ,Z ,L = 0 ,A ,B };

private:
	/*Header資訊*/
	struct Header {
	public:
		string format;//32-bit_rle_rgbe || 32-bit_rle_xyze
		float exposure;//檔案中已經先被乘到每一個pixel上面的值
		float colorcorr;
		float pixelratio;//a decimal fraction of the height of each pixel to its width
		ColorSpaceTransformation* rgb2lab;
	} headinfo;

	/*基礎的RGBE 之所以不用class是因為這樣處理速度比較快*/
	float* *C1;
	float* *C2;
	float* *C3;
	float* *E;
	
	///*影像高度與寬度*/
	//int width;//x軸方向的最大值
	//int height;//y軸方向的最大值

	

private:
	
	//讀取HDR file的Header部分 並且設定相關資訊
	void readHeader( ifstream& input);
	/*
	 讀取Resolution大小 並且決定X軸及Y軸走向
	 傳入：處理輸入檔案類別 X軸與Y軸起始位置以及增加走向
	 傳出：此函式會修改x y ix iy變數以方便指定走向
	*/
	void readResolution( ifstream& input ,int& x ,int& y ,int& ix ,int& iy);

	/*
	  讀取Pixel資料
	  傳入：處理輸入檔案類別 X軸與Y軸起始位置以及增加走向
	*/
	void readScanline( ifstream& input ,int x ,int y ,int ix ,int iy );

	//根據channel和value設定pixel值
	void setPixel(int i ,int j ,int channel ,float value);

	/*
	  根據Channel以及要的資訊取值
	  傳入：要取(x ,y )哪一個Channel以及是以哪一種ColorSpace
			channel有三種可能性分別是1 2 3 其他狀況將會回傳NOTANUMBER
		    colorspace只有兩種可能性 一種是rgb一種是xyz
	  傳回：該channel的值 如果沒有取得成功將會回傳NOTANUMBER
	*/
	double getChannel( int x ,int y ,int c ,const string colorspace ) const;
	/*
	  取得LAB的值
	  傳入：要取(x ,y )轉換後的LAB中的哪一個部分
	  傳回：該值
	*/
	double getLAB(int x ,int y ,int c) const;
private:
	//進行Hard Copy用
	void hardCopy( const HDRLoader& loader );
	
	
};

#endif