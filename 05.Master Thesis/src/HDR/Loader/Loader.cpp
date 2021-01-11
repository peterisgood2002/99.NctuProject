/******************************************
FILE
    Loader.cpp
PURPOSE
    定義讀入High Dynamic Range Image之Loader的class
	此類別是一個Virtual class 做為一個interface用

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include "Loader.h"

#include <iostream>
#include <exception>

using std::cerr;
using std::exception;

Loader::~Loader() {}

/*取得最大最小的Luminance*/
#using <mscorlib.dll>
using System::Double;

double 
Loader::getMaxL() const {
	double result = Double::MinValue;

	for ( int i = 0; i < width; i++ ) {
		for ( int j = 0; j < height; j++ ) {
			double val = getLuminance( i ,j );
			if ( val > result )
				result = val;
		}
	}

	return result;
}
double 
Loader::getMinL() const {
	double result = Double::MaxValue;

	for ( int i = 0; i < width; i++ ) {
		for ( int j = 0; j < height; j++ ) {
			double val = getLuminance( i ,j );
			if ( val < result )
				result = val;
		}
	}

	return result;
}
//測試是否超過範圍
bool 
Loader::testDimension( int x , int y ) const {
	if ( x >= 0 && x < width && y >= 0 && y < height )
		return true;
	else {
		
		if ( x < 0 && x >= width )
			throw exception( "Loader::testDimension() 存取錯誤：x軸方向\n" );
		else 
			throw exception( "Loader::testDimension() 存取錯誤：y軸方向\n" );

		return false;	
		
	}
}
