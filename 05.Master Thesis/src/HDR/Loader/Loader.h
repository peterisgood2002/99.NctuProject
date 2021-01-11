/******************************************
FILE
    Loader.h
PURPOSE
    �ŧiŪ�JHigh Dynamic Range Image��Loader��class
	�����O�O�@��Virtual class �����@��interface��

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

#ifndef _LOADER_
#define _LOADER_

class Loader {
public:
	virtual ~Loader() = 0;
public:
	/*���o�v�����שM�e��*/
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
	  ���լO�_�W�L�d��
	  �ǤJ�G(x ,y)
	  �ǥX�G�O�_���զ��\
	  ��X�G�쩳�OX�W�L�٬OY�W�L���T���X��
	*/
	bool testDimension( int x , int y ) const;


protected:
	/*�v�����׻P�e��*/
	int width;//x�b��V���̤j��
	int height;//y�b��V���̤j��


};
#endif