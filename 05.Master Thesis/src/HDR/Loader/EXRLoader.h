/******************************************
FILE
    EXRLoader
PURPOSE
    宣告讀入EXR格式的class

NOTE
	都是因為High dynamic Range之file format沒有統一才讓我那麼麻煩

AUTHOR
    9757553 顏志晟
*******************************************/

#ifndef _EXRLOADER_
#define _EXRLOADER_

#include <string>
#include "Loader.h"
#include "../ColorSpace/RGB2XYZ.h"

#include <ImfRgbaFile.h>
#include <ImfHeader.h>

using std::string;
using Imf::RgbaInputFile;
using Imf::Header;

class EXRLoader : public Loader {

public:
	/*Constructor*/
	EXRLoader( const string& fname );
	//the "Big Three"
	/*Copy Constructor*/
	EXRLoader( const EXRLoader& loader);

	/**Overloading the assignment operator*/
	EXRLoader& operator= (const EXRLoader& v );

public:
	~EXRLoader();

public:
	/*
	 讀入HDR 影像 RGBE的影像格式
	 傳入：HDR影像的file name
	 傳出：讀取是否成功
	 Exception：若讀入檔案失敗 將會傳出Exception
	*/
	bool readImage(const string& fname);

public:
	/*Virtual Function*/
	inline double getRed( int  x ,int y) const;
	inline double getGreen( int  x ,int y ) const;
	inline double getBlue( int  x ,int y ) const;
	double getX( int  x ,int y) const;
	double getY( int  x ,int y ) const;
	double getZ( int  x ,int y ) const;
	double getBright( int x ,int y ) const;
	double getLuminance( int x ,int y ) const;
	double getAvgL() const;
	


private:
	void init();
	
	/*讀取Header相關資訊*/
	void readHeader( const RgbaInputFile& input );
	/*讀取Pixel資料*/
	void readPixel( RgbaInputFile& input );

	/*轉換color space*/
	Vector transformRGB2XYZ( int x ,int y) const;

private:
	static const float DEFAULT;
	static const float NOTANUMBER;

private:
	//Header input;//請注意 之所以用指標的關係 是因為Header的建構子很怪 所以只好這樣做
	RGB2XYZ* rgb2xyz;
	float* *R;
	float* *G;
	float* *B;
	

private:
	//進行Hard Copy用
	void hardCopy( const EXRLoader& loader );

};

/*Inline function*/
double 
EXRLoader::getRed( int  x ,int y) const {
	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	return R[y][x];;
}
double 
EXRLoader::getGreen( int  x ,int y ) const {
	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	return G[y][x];
}
double 
EXRLoader::getBlue( int  x ,int y ) const {
	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	return B[y][x];
}
#endif