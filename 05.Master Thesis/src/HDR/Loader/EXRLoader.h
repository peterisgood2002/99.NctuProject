/******************************************
FILE
    EXRLoader
PURPOSE
    �ŧiŪ�JEXR�榡��class

NOTE
	���O�]��High dynamic Range��file format�S���Τ@�~���ڨ���·�

AUTHOR
    9757553 �C����
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
	 Ū�JHDR �v�� RGBE���v���榡
	 �ǤJ�GHDR�v����file name
	 �ǥX�GŪ���O�_���\
	 Exception�G�YŪ�J�ɮץ��� �N�|�ǥXException
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
	
	/*Ū��Header������T*/
	void readHeader( const RgbaInputFile& input );
	/*Ū��Pixel���*/
	void readPixel( RgbaInputFile& input );

	/*�ഫcolor space*/
	Vector transformRGB2XYZ( int x ,int y) const;

private:
	static const float DEFAULT;
	static const float NOTANUMBER;

private:
	//Header input;//�Ъ`�N ���ҥH�Ϋ��Ъ����Y �O�]��Header���غc�l�ܩ� �ҥH�u�n�o�˰�
	RGB2XYZ* rgb2xyz;
	float* *R;
	float* *G;
	float* *B;
	

private:
	//�i��Hard Copy��
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