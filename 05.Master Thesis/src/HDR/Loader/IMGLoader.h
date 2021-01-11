/******************************************
FILE
    IMGLoader.h
PURPOSE
    讀取Image

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/

#ifndef _IMGLOADER_
#define _IMGLOADER_

#include "Loader.h"
#include "../ColorSpace/RGB2XYZ.h"
#include <string>

using std::string;

class IMGLoader : public Loader {
public:
	IMGLoader( const string& fname );
	/*Copy Constructor*/
	IMGLoader( const IMGLoader& loader);
	/**Overloading the assignment operator*/
	IMGLoader& operator= (const IMGLoader& loader );

public:
	~IMGLoader();
	
public:
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
	RGB2XYZ* rgb2xyz;
	float* *R;
	float* *G;
	float* *B;

private:
	static const float NOTANUMBER;

private:
	void init();
	void setRGB( const string& fname );
	Vector transformRGB2XYZ( int x ,int y) const;
	//進行Hard Copy用
	void hardCopy( const IMGLoader& loader );


};

/*Inline function*/
double 
IMGLoader::getRed( int  x ,int y) const {
	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	return R[y][x];;
}
double 
IMGLoader::getGreen( int  x ,int y ) const {
	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	return G[y][x];
}
double 
IMGLoader::getBlue( int  x ,int y ) const {
	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	return B[y][x];
}
#endif