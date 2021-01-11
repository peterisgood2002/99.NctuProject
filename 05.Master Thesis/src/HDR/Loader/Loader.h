/******************************************
FILE
    Loader.h
PURPOSE
    宣告讀入High Dynamic Range Image之Loader的class
	此類別是一個Virtual class 做為一個interface用

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/

#ifndef _LOADER_
#define _LOADER_

class Loader {
public:
	virtual ~Loader() = 0;
public:
	/*取得影像高度和寬度*/
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	double getMaxL() const;
	double getMinL() const;

public:
	/*virtual function*/
	virtual double getRed( int  x ,int y) const = 0;
	virtual double getGreen( int  x ,int y ) const = 0;
	virtual double getBlue( int  x ,int y ) const = 0;
	virtual double getX( int  x ,int y) const = 0;
	virtual double getY( int  x ,int y ) const = 0;
	virtual double getZ( int  x ,int y ) const = 0;
	virtual double getBright( int x ,int y ) const = 0;
	virtual double getLuminance( int x ,int y ) const = 0;
	virtual double getAvgL() const = 0;

	

protected:
	/*
	  測試是否超過範圍
	  傳入：(x ,y)
	  傳出：是否測試成功
	  丟出：到底是X超過還是Y超過的訊息出來
	*/
	bool testDimension( int x , int y ) const;


protected:
	/*影像高度與寬度*/
	int width;//x軸方向的最大值
	int height;//y軸方向的最大值


};
#endif