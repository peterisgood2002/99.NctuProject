/******************************************
FILE
    RGB2LAB.cpp
PURPOSE
    進行RGB -> LAB與LAB -> RGB轉換

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include "RGB2LAB.h"
#include <cmath>

const string RGB2LAB::type = "RGB2LAB"; 
const int RGB2LAB::dimension = 3;

RGB2LAB::RGB2LAB() :RGB2XYZ() {
	refwhite = new double[ dimension ];

	for ( int i = 0; i < dimension; i++ )
		refwhite[i] = 1.0 / dimension;
}
RGB2LAB::RGB2LAB( const Matrix&  rgb2xyz ) : RGB2XYZ(rgb2xyz) {
}	
RGB2LAB::RGB2LAB( float* * const chrom , const float* w ) : RGB2XYZ(chrom ,w ) {
	refwhite = new double[ dimension ];

	for ( int i = 0; i < dimension; i++ )
		refwhite[i] = w[i];

}
//Copy Constuctor
RGB2LAB::RGB2LAB( const RGB2LAB& cs) {
	refwhite = new double[ dimension ];
	for ( int i = 0; i < dimension; i++ )
		refwhite[i] = cs.refwhite[i];
}
//operator assignment
RGB2LAB& 
RGB2LAB::operator= (const RGB2LAB& cs ) {
	if (this == &cs) return *this;
	
	for ( int i = 0; i < dimension; i++ )
		refwhite[i] = cs.refwhite[i];

	return *this;
}
//Destructor
RGB2LAB::~RGB2LAB() {
	delete [] refwhite;
}

Vector 
RGB2LAB::transform ( const Vector& v ) const {
	Vector xyz = RGB2XYZ::transform(v);
	
	return XYZ2LAB(xyz);
}
Vector 
RGB2LAB::transform ( const Vector& v , const string& t ) const {
	if ( t == type ) 
		return transform(v);
	else return RGB2XYZ::transform(v);
}
/*
  進行逆轉換 
  傳入：要轉換的Vector 以及轉換是否成功的bit
  傳出：轉換結果如果無法進行逆轉換 將會回傳default matrix
*/
Vector 
RGB2LAB::inverseTransform( const Vector& v ,bool& issuccess) const {
	Vector xyz = LAB2XYZ(v);

	return RGB2XYZ::inverseTransform( xyz ,issuccess );
	
}
Vector 
RGB2LAB::inverseTransform ( const Vector& v ,bool& issuccess , const string& t ) const {
	if ( t == type )
		return inverseTransform( v ,issuccess );
	else {
		return RGB2XYZ::inverseTransform( v ,issuccess );
	}
}
//從XYZ-->LAB
//進行正轉換
const double RGB2LAB::delta = 6.0 / 29;
double 
RGB2LAB::f( double t ) const{
	if ( t > pow( delta , 3 ) )
		return pow(t , 1.0/3 );
	
	else 
		return 1.0 / 3 * pow( 1/ delta ,2 ) * t + 4.0/29;
}
Vector 
RGB2LAB::XYZ2LAB( const Vector& xyz ) const {
	double lab[ dimension ] = {0};
	
	/*
		Xn Yn Zn : the CIE XYZ tristimulus values of the reference white point
		L =	116 * f(Y/Yn)^1/3 - 16

	*/
	lab[0] = 116* f( xyz[1] / refwhite[1] ) - 16;

	/*
	  A* = 500 * [f( X/Xn ) - f ( Y/Yn ) ]
	  B* = 200 * [f( Y/Yn ) - f ( Z/Zn ) ]

			t ^ 1/3					if t > 0.008856	
	  f = 
	        1/3 *(29/6)^2 * t + 4/29	if Y/Yn <= 0.008856	
	*/
	lab[1] = 500 * ( f( xyz[0]/refwhite[0] ) - f( xyz[1]/refwhite[1] ) );
	lab[2] = 200 * ( f( xyz[1]/refwhite[1] ) - f( xyz[2]/refwhite[2] ) );

	return Vector( 3 , lab );
}
Vector 
RGB2LAB::LAB2XYZ( const Vector& v ) const {
	/*
	  fy = ( L* + 16 ) / 116
	  fx = fy + A* / 500
	  fz = fy - b* / 200

	  delta = 6 / 29
	  if ( fy >  delta) Y = Yn*fy^3
	  else Y = ( fy - 16/116) * 3 * (delta)^2 * Yn
		
	  if ( fx > delta ) X = Xn* fx ^ 3
	  else X = ( fx - 16/116) * 3 * (delta)^2 * Xn

	  if ( fz > delta ) Z = Zn* fz ^ 3
	  else Z = ( fz - 16/116) * 3 * (delta)^2 * Zn
	*/

	double fy = ( v[0] + 16 ) / 116;
	double fx = fy + v[1] / 500;
	double fz = fy - v[2] / 200;

	double xyz[dimension] = {0};
	if ( fy > delta ) xyz[1] = refwhite[1] * pow(fy , 3 );
	else xyz[1]= ( fy - 4.0/29 ) * 3 * (delta) * (delta) * refwhite[1];
	if ( fx > delta ) xyz[0] = refwhite[0] * pow(fx , 3 );
	else xyz[0]= ( fx - 4.0/29 ) * 3 * (delta) * (delta) * refwhite[0];

	if ( fz > delta ) xyz[2] = refwhite[2] * pow(fz , 3 );
	else xyz[2]= ( fz - 4.0/29 ) * 3 * (delta) * (delta) * refwhite[2];

	return Vector( 3 , xyz );
}
//string 
//RGB2LAB::getType() {
//	return type;
//}