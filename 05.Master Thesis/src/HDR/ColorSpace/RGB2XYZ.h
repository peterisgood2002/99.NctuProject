/******************************************
FILE
    RGB2XYZ.h
PURPOSE
    進行RGB -> XY與XY -> RGB轉換
	詳細轉換請看REF中的Color space

	請記住RGB轉換主要是3 * 3的矩陣

NOTE
	

AUTHOR
    9757553 顏志晟
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
	 計算轉換矩陣用
	*/
	void calTransMatrix(float* * const chrom ,const float* w );

	//進行正轉換
	Vector transform ( const Vector& v  ) const;
	Vector transform ( const Vector& v , const string& t ) const;//進行RGB2XYZ轉換
	/*
	  進行逆轉換 
	  傳入：要轉換的Vector 以及轉換是否成功的bit
	  傳出：轉換結果如果無法進行逆轉換 將會回傳default matrix
	*/
	Vector inverseTransform( const Vector& v ,bool& issuccess ) const;
	Vector inverseTransform ( const Vector& v ,bool& issuccess , const string& t ) const;//進行RGB2XYZ逆轉換

public:
	static const string type;
	
private:
	
	static const int dimension;//維度
	

};
#endif