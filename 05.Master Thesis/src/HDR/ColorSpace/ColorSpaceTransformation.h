/******************************************
FILE
    ColorSpaceTransformation.h
PURPOSE
    ColorSpace�ഫ���һݭn���F�� 
	�o�O�@��abstract class
	�ΨӴ��ѩҦ�ColorSpace�����ӭn�����@�Ǩ禡

	����JAVA��interface �u���Ф@�ǥi�H�Ϊ�function�Ӥw

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

#ifndef _ColorSpaceTransformation_
#define _ColorSpaceTransformation_

#include "../Essential/Vector.h"
#include "../Essential/Matrix.h"
//#include "Vector.h"

#include <string>


using std::string;

//class Vector;
class ColorSpaceTransformation {

public:
	/*Constructor*/
	ColorSpaceTransformation ();
	ColorSpaceTransformation ( const Matrix&  m );
	ColorSpaceTransformation ( float* * const d , int row ,int col );
	//�]���o�Opure class�ҥH�èS��copy constructor
	/*operator assignment*/
	ColorSpaceTransformation& operator= (const ColorSpaceTransformation& cs );
	/*Destructor*/
	virtual ~ColorSpaceTransformation();
	/*set Matrix*/
	void setTransformationMatrix( const Matrix&  m );
	void setTransformationMatrix( float* * const d , int row ,int col );

	/*
	 �i�楿�ഫ
	 */
	virtual Vector transform ( const Vector& v  ) const = 0; 
	virtual Vector transform ( const Vector& v , const string& t ) const = 0;//�ھ�t�i���ഫ
	/*
	  �i��f�ഫ 
	  �ǤJ�G�n�ഫ��Vector �H���ഫ�O�_���\��bit
	  �ǥX�G�ഫ���G�p�G�L�k�i��f�ഫ �N�|�^��default matrix
	*/
	virtual Vector inverseTransform( const Vector& v ,bool& issuccess) const = 0;
	virtual Vector inverseTransform ( const Vector& v ,bool& issuccess , const string& t ) const = 0;//�ھ�t�i���ഫ

	//���o�ഫ�x�}
	Matrix* getTransformMatrix() const;
	Matrix* getInverseTransformationMatrix() const;

protected:
	Matrix* transmatrix;
	Matrix* inversetransmatrix;

};

#endif