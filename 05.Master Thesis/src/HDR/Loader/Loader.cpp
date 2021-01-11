/******************************************
FILE
    Loader.cpp
PURPOSE
    �w�qŪ�JHigh Dynamic Range Image��Loader��class
	�����O�O�@��Virtual class �����@��interface��

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/
#include "Loader.h"

#include <iostream>
#include <exception>

using std::cerr;
using std::exception;

Loader::~Loader() {}

/*���o�̤j�̤p��Luminance*/
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
//���լO�_�W�L�d��
bool 
Loader::testDimension( int x , int y ) const {
	if ( x >= 0 && x < width && y >= 0 && y < height )
		return true;
	else {
		
		if ( x < 0 && x >= width )
			throw exception( "Loader::testDimension() �s�����~�Gx�b��V\n" );
		else 
			throw exception( "Loader::testDimension() �s�����~�Gy�b��V\n" );

		return false;	
		
	}
}
