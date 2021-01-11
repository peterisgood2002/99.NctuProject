/******************************************
FILE
    HDRLoader.h
PURPOSE
    �ŧiŪ�JHDR�榡��class

NOTE
	

AUTHOR
    9757553 �C����
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
	 Ū�JHDR �v�� RGBE���v���榡
	 �ǤJ�GHDR�v����file name
	 �ǥX�GŪ���O�_���\
	 Exception�G�YŪ�J�ɮץ��� �N�|�ǥXException
	*/
	bool readImage( const string& fname );
	
	
	/*
	 �ھ�Greg Ward�bGraphics Gems��REAL Pixel�o�@�g���o(x ,y )��RGBE���
	 �]���o�s���O��� ���O���L�O�H��ǰO����ܪk��ܪ�
	 �p�G������ �L�|�^��NOTANUMBER �å�X���~�T��

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
	 �]�w���
	 �p�G�]�w�����\ �N�|�ƻ�Ƴ�����
	*/
	void scaleLuminanceData(int x ,int y ,float scale );
	/*
	 �x�s�Ϥ�
	*/
	void saveImage(const string& fname);
	

protected:
	//��l�Ƭ������
	void init();

private:
	static const float DEFAULT;
	static const float NOTANUMBER;
	static const string RGB;
	static const string XYZ;
	enum C{ RED = 0 ,GREEN ,BLUE ,X = 0 ,Y ,Z ,L = 0 ,A ,B };

private:
	/*Header��T*/
	struct Header {
	public:
		string format;//32-bit_rle_rgbe || 32-bit_rle_xyze
		float exposure;//�ɮפ��w�g���Q����C�@��pixel�W������
		float colorcorr;
		float pixelratio;//a decimal fraction of the height of each pixel to its width
		ColorSpaceTransformation* rgb2lab;
	} headinfo;

	/*��¦��RGBE ���ҥH����class�O�]���o�˳B�z�t�פ����*/
	float* *C1;
	float* *C2;
	float* *C3;
	float* *E;
	
	///*�v�����׻P�e��*/
	//int width;//x�b��V���̤j��
	//int height;//y�b��V���̤j��

	

private:
	
	//Ū��HDR file��Header���� �åB�]�w������T
	void readHeader( ifstream& input);
	/*
	 Ū��Resolution�j�p �åB�M�wX�b��Y�b���V
	 �ǤJ�G�B�z��J�ɮ����O X�b�PY�b�_�l��m�H�μW�[���V
	 �ǥX�G���禡�|�ק�x y ix iy�ܼƥH��K���w���V
	*/
	void readResolution( ifstream& input ,int& x ,int& y ,int& ix ,int& iy);

	/*
	  Ū��Pixel���
	  �ǤJ�G�B�z��J�ɮ����O X�b�PY�b�_�l��m�H�μW�[���V
	*/
	void readScanline( ifstream& input ,int x ,int y ,int ix ,int iy );

	//�ھ�channel�Mvalue�]�wpixel��
	void setPixel(int i ,int j ,int channel ,float value);

	/*
	  �ھ�Channel�H�έn����T����
	  �ǤJ�G�n��(x ,y )���@��Channel�H�άO�H���@��ColorSpace
			channel���T�إi��ʤ��O�O1 2 3 ��L���p�N�|�^��NOTANUMBER
		    colorspace�u����إi��� �@�جOrgb�@�جOxyz
	  �Ǧ^�G��channel���� �p�G�S�����o���\�N�|�^��NOTANUMBER
	*/
	double getChannel( int x ,int y ,int c ,const string colorspace ) const;
	/*
	  ���oLAB����
	  �ǤJ�G�n��(x ,y )�ഫ�᪺LAB�������@�ӳ���
	  �Ǧ^�G�ӭ�
	*/
	double getLAB(int x ,int y ,int c) const;
private:
	//�i��Hard Copy��
	void hardCopy( const HDRLoader& loader );
	
	
};

#endif