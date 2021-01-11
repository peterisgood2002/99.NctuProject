/******************************************
FILE
    Picture.h
PURPOSE
    �ŧiPicture class�ΨӶi��H��B�z��
	

NOTE
	

AUTHOR
    9757553 �C����
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

//����
class Picture {
private:
	Bitmap* image;//�v����
	string fname;
	string dir;
public:
	static void initGDIPlus();//��l��GDI+
	/*Constructor*/
	/*
	  �ǤJ�G�����ɦW ���u��m
	*/
	Picture(const string imgfn );
	/*Copy Constuctor*/
	Picture( const Picture& pic );
	/*Destructor*/
	~Picture();

	/*�e�ϽT�w���g�i�h��*/
	void drawPicture(const string fn);
	
	/*
	  Get data member function
	*/
	inline int getHeight() const { return image->GetHeight(); }
	inline int getWidth() const{ return image->GetWidth(); }
	Color getColor(int x ,int y ) const;
	/*
	  ���o�@��(x ,y)�P�򪺸�T
	  �ǤJ�G�y�� �H�έn���h��
	  �ǥX�G�@��vector
	*/
	WColor* getWindowPixels(int x ,int y ,int wheight ,int wwidth) const;
	/*
	  ���o�@�檺���
	  �ǤJ�G�n�����渹���� windows size�h�j
	  �ǥX�G�@�Ӧ�windows size��vector��color��array
	*/
	vector<WColor*>* getOneLineColors( int l ,int wheight ,int wwidth) const;
	string getDirName() const { return dir; }

	/*
	  Set data member
	*/
	void setColor(int x ,int y ,const Color &c );

	
};

#endif