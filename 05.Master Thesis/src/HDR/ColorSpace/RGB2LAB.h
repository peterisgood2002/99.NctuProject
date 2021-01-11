/******************************************
FILE
    RGB2LAB.h
PURPOSE
    �i��RGB -> LAB�PLAB -> RGB�ഫ
	�Բ��ഫ�Ь�REF����Color space

	�аO��RGB�ഫ�D�n�O3 * 3���x�}

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

#ifndef _RGB2LAB_
#define _RGB2LAB_

#include "RGB2XYZ.h"
#include <string>

using std::string;

class RGB2LAB : public RGB2XYZ{

public:
	RGB2LAB();
	RGB2LAB( const Matrix&  rgb2xyz );
	
	RGB2LAB( float* * const chrom , const float* w );

	//Copy Constuctor
	RGB2LAB( const RGB2LAB& cs);
	//operator assignment
	RGB2LAB& operator= (const RGB2LAB& cs );
	//Destructor
	~RGB2LAB();

	//�i�楿�ഫ
	Vector transform ( const Vector& v ) const;
	Vector transform ( const Vector& v , const string& t ) const;//�i��RGB2XYZ�ഫ
	/*
	  �i��f�ഫ 
	  �ǤJ�G�n�ഫ��Vector �H���ഫ�O�_���\��bit
	  �ǥX�G�ഫ���G�p�G�L�k�i��f�ഫ �N�|�^��default matrix
	*/
	Vector inverseTransform( const Vector& v ,bool& issuccess ) const;
	Vector inverseTransform ( const Vector& v ,bool& issuccess , const string& t) const;//�i��RGB2XYZ�f�ഫ

	//�qXYZ-->LAB
	Vector XYZ2LAB( const Vector& v ) const;
	Vector LAB2XYZ( const Vector& v ) const;

public:
	static const string type;

private:
	
	static const int dimension;//����
	static const double delta;
	double* refwhite;

private:
	double f( double t ) const;
};

#endif