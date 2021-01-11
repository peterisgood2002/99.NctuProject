/******************************************
FILE
    RGB2XYZ.h
PURPOSE
    �i��RGB -> XY�PXY -> RGB�ഫ
	�Բ��ഫ�Ь�REF����Color space

	�аO��RGB�ഫ�D�n�O3 * 3���x�}

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

#ifndef _RGB2XYZ_
#define _RGB2XYZ_

#include "ColorSpaceTransformation.h"

#include <string>

using std::string;
class RGB2XYZ :public ColorSpaceTransformation {

public:
	/*Constructor*/
	RGB2XYZ();
	RGB2XYZ( const Matrix&  m );
	
	RGB2XYZ( float* * const chrom , const float* w );
	//~RGB2XYZ();

	/*
	 �p���ഫ�x�}��
	*/
	void calTransMatrix(float* * const chrom ,const float* w );

	//�i�楿�ഫ
	Vector transform ( const Vector& v  ) const;
	Vector transform ( const Vector& v , const string& t ) const;//�i��RGB2XYZ�ഫ
	/*
	  �i��f�ഫ 
	  �ǤJ�G�n�ഫ��Vector �H���ഫ�O�_���\��bit
	  �ǥX�G�ഫ���G�p�G�L�k�i��f�ഫ �N�|�^��default matrix
	*/
	Vector inverseTransform( const Vector& v ,bool& issuccess ) const;
	Vector inverseTransform ( const Vector& v ,bool& issuccess , const string& t ) const;//�i��RGB2XYZ�f�ഫ

public:
	static const string type;
	
private:
	
	static const int dimension;//����
	

};
#endif