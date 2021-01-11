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
#pragma comment(lib, "gdiplus.lib")

using std::string;
using namespace Gdiplus;

//���u��m
struct Light {
public:
	double x;
	double y;
	double z;
};

//����
class Picture {
private:
	Bitmap* image;//�v����
	Light lightc;//���u�b����
	string fname;
	string dir;
public:
	static void initGDIPlus();//��l��GDI+
	/*Constructor*/
	/*
	  �ǤJ�G�����ɦW ���u��m
	*/
	Picture(const string imgfn ,const Light& lc);
	
	/*Destructor*/
	~Picture();

	/*�e�ϽT�w���g�i�h��*/
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