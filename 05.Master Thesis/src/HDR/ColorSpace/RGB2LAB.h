/******************************************
FILE
    RGB2LAB.h
PURPOSE
    進行RGB -> LAB與LAB -> RGB轉換
	詳細轉換請看REF中的Color space

	請記住RGB轉換主要是3 * 3的矩陣

NOTE
	

AUTHOR
    9757553 顏志晟
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

	//進行正轉換
	Vector transform ( const Vector& v ) const;
	Vector transform ( const Vector& v , const string& t ) const;//進行RGB2XYZ轉換
	/*
	  進行逆轉換 
	  傳入：要轉換的Vector 以及轉換是否成功的bit
	  傳出：轉換結果如果無法進行逆轉換 將會回傳default matrix
	*/
	Vector inverseTransform( const Vector& v ,bool& issuccess ) const;
	Vector inverseTransform ( const Vector& v ,bool& issuccess , const string& t) const;//進行RGB2XYZ逆轉換

	//從XYZ-->LAB
	Vector XYZ2LAB( const Vector& v ) const;
	Vector LAB2XYZ( const Vector& v ) const;

public:
	static const string type;

private:
	
	static const int dimension;//維度
	static const double delta;
	double* refwhite;

private:
	double f( double t ) const;
};

#endif