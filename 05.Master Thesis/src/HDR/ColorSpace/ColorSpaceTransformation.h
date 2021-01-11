/******************************************
FILE
    ColorSpaceTransformation.h
PURPOSE
    ColorSpace轉換的所需要的東西 
	這是一個abstract class
	用來提供所有ColorSpace都應該要有的一些函式

	類似JAVA的interface 只介紹一些可以用的function而已

NOTE
	

AUTHOR
    9757553 顏志晟
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
	//因為這是pure class所以並沒有copy constructor
	/*operator assignment*/
	ColorSpaceTransformation& operator= (const ColorSpaceTransformation& cs );
	/*Destructor*/
	virtual ~ColorSpaceTransformation();
	/*set Matrix*/
	void setTransformationMatrix( const Matrix&  m );
	void setTransformationMatrix( float* * const d , int row ,int col );

	/*
	 進行正轉換
	 */
	virtual Vector transform ( const Vector& v  ) const = 0; 
	virtual Vector transform ( const Vector& v , const string& t ) const = 0;//根據t進行轉換
	/*
	  進行逆轉換 
	  傳入：要轉換的Vector 以及轉換是否成功的bit
	  傳出：轉換結果如果無法進行逆轉換 將會回傳default matrix
	*/
	virtual Vector inverseTransform( const Vector& v ,bool& issuccess) const = 0;
	virtual Vector inverseTransform ( const Vector& v ,bool& issuccess , const string& t ) const = 0;//根據t進行轉換

	//取得轉換矩陣
	Matrix* getTransformMatrix() const;
	Matrix* getInverseTransformationMatrix() const;

protected:
	Matrix* transmatrix;
	Matrix* inversetransmatrix;

};

#endif