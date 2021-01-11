#ifndef LINEAR_R3_H
#define LINEAR_R3_H

#include <math.h>
#include <assert.h>
#include <iostream>
//#include "MathMisc.h"
using namespace std;

class VectorR3;				// Space Vector (length 3)
class VectorR4;				// Space Vector (length 4)

// Most for internal use:
class Matrix3x3;
class Matrix3x4;

// **************************************
// VectorR3 class                       *
// * * * * * * * * * * * * * * * * * * **

class VectorR3 {

public:
	double x, y, z;		// The x & y & z coordinates.

	static const VectorR3 Zero;
	static const VectorR3 UnitX;
	static const VectorR3 UnitY;
	static const VectorR3 UnitZ;
	static const VectorR3 NegUnitX;
	static const VectorR3 NegUnitY;
	static const VectorR3 NegUnitZ;

public:
	VectorR3( ) : x(0.0), y(0.0), z(0.0) {}
	VectorR3( double xVal, double yVal, double zVal )
		: x(xVal), y(yVal), z(zVal) {}

	VectorR3& Set( double xx, double yy, double zz ) 
				{ x=xx; y=yy; z=zz; return *this; }
	VectorR3& SetFromHg( const VectorR4& );	// Convert homogeneous VectorR4 to VectorR3
	VectorR3& SetZero() { x=0.0; y=0.0; z=0.0;  return *this;}
	VectorR3& Load( const double* v );
	VectorR3& Load( const float* v );
	void Dump( double* v ) const;
	void Dump( float* v ) const;

	inline double operator[]( int i );

	VectorR3& operator= ( const VectorR3& v ) 
		{ x=v.x; y=v.y; z=v.z; return(*this);}
	VectorR3& operator+= ( const VectorR3& v ) 
		{ x+=v.x; y+=v.y; z+=v.z; return(*this); } 
	VectorR3& operator-= ( const VectorR3& v ) 
		{ x-=v.x; y-=v.y; z-=v.z; return(*this); }
	VectorR3& operator*= ( double m ) 
		{ x*=m; y*=m; z*=m; return(*this); }
	VectorR3& operator/= ( double m ) 
			{ register double mInv = 1.0/m; 
			  x*=mInv; y*=mInv; z*=mInv; 
			  return(*this); }
	VectorR3 operator- () const { return ( VectorR3(-x, -y, -z) ); }
	VectorR3& operator*= (const VectorR3& v);	// Cross Product
	VectorR3& ArrayProd(const VectorR3&);		// Component-wise product

	VectorR3& AddScaled( const VectorR3& u, double s );

	bool IsZero() const { return ( x==0.0 && y==0.0 && z==0.0 ); }
	double Norm() const { return ( (double)sqrt( x*x + y*y + z*z ) ); }
	double NormSq() const { return ( x*x + y*y + z*z ); }
	double MaxAbs() const;
	double Dist( const VectorR3& u ) const;	// Distance from u
	double DistSq( const VectorR3& u ) const;	// Distance from u squared
	VectorR3& Negate() { x = -x; y = -y; z = -z; return *this;}	
	VectorR3& Normalize () { *this /= Norm(); return *this;}	// No error checking
	inline VectorR3& MakeUnit();		// Normalize() with error checking
	inline VectorR3& ReNormalize();
	bool IsUnit( ) const
		{ register double norm = Norm();
		  return ( 1.000001>=norm && norm>=0.999999 ); }
	bool IsUnit( double tolerance ) const
		{ register double norm = Norm();
		  return ( 1.0+tolerance>=norm && norm>=1.0-tolerance ); }
	bool NearZero(double tolerance) const { return( MaxAbs()<=tolerance );}
							// tolerance should be non-negative

	double YaxisDistSq() const { return (x*x+z*z); }
	double YaxisDist() const { return sqrt(x*x+z*z); }

	VectorR3& Rotate( double theta, const VectorR3& u); // rotate around u.
	VectorR3& RotateUnitInDirection ( const VectorR3& dir);	// rotate in direction dir

	friend ostream& operator<< ( ostream& os, const VectorR3& u );

};

inline VectorR3 operator+( const VectorR3& u, const VectorR3& v );
inline VectorR3 operator-( const VectorR3& u, const VectorR3& v ); 
inline VectorR3 operator*( const VectorR3& u, double m); 
inline VectorR3 operator*( double m, const VectorR3& u); 
inline VectorR3 operator/( const VectorR3& u, double m); 
inline int operator==( const VectorR3& u, const VectorR3& v ); 

inline double operator^ (const VectorR3& u, const VectorR3& v ); // Dot Product
inline VectorR3 operator* (const VectorR3& u, const VectorR3& v);	 // Cross Product
inline VectorR3 ArrayProd ( const VectorR3& u, const VectorR3& v );

inline double Mag(const VectorR3& u) { return u.Norm(); }
inline double Dist(const VectorR3& u, const VectorR3& v) { return u.Dist(v); }
inline double DistSq(const VectorR3& u, const VectorR3& v) { return u.DistSq(v); }
inline double NormalizeError (const VectorR3& u);
 
extern const VectorR3 UnitVecIR3;
extern const VectorR3 UnitVecJR3;
extern const VectorR3 UnitVecKR3;

// 
// Advanced vector and position functions (prototypes)
//

//VectorR3 Interpolate( const VectorR3& start, const VectorR3& end, double a);

// *****************************************
// Matrix3x3 class                         *
// * * * * * * * * * * * * * * * * * * * * *

class Matrix3x3 {
public:

	double m11, m12, m13, m21, m22, m23, m31, m32, m33;	
									
	// Implements a 3x3 matrix: m_i_j - row-i and column-j entry

	static const Matrix3x3 Identity;

public:
	inline Matrix3x3();
	inline Matrix3x3(const VectorR3&, const VectorR3&, const VectorR3&); // Sets by columns!
	inline Matrix3x3(double, double, double, double, double, double,
					 double, double, double );	// Sets by columns

	inline void SetIdentity ();		// Set to the identity map
	inline void Set ( const Matrix3x3& );	// Set to the matrix.
	inline void Set3x3 ( const Matrix3x4& );	// Set to the 3x3 part of the matrix.
	inline void Set( const VectorR3&, const VectorR3&, const VectorR3& );
	inline void Set( double, double, double,
					 double, double, double,
					 double, double, double );
	inline void SetByRows( double, double, double, double, double, double,
							double, double, double );
	inline void SetByRows( const VectorR3&, const VectorR3&, const VectorR3& );
	
	inline void SetColumn1 ( double, double, double );
	inline void SetColumn2 ( double, double, double );
	inline void SetColumn3 ( double, double, double );
	inline void SetColumn1 ( const VectorR3& );
	inline void SetColumn2 ( const VectorR3& );
	inline void SetColumn3 ( const VectorR3& );
	inline VectorR3 Column1() const;
	inline VectorR3 Column2() const;
	inline VectorR3 Column3() const;

	inline void SetRow1 ( double, double, double );
	inline void SetRow2 ( double, double, double );
	inline void SetRow3 ( double, double, double );
	inline void SetRow1 ( const VectorR3& );
	inline void SetRow2 ( const VectorR3& );
	inline void SetRow3 ( const VectorR3& );
	inline VectorR3 Row1() const;
	inline VectorR3 Row2() const;
	inline VectorR3 Row3() const;

	inline void SetDiagonal( double, double, double );
	inline void SetDiagonal( const VectorR3& );
	inline double Diagonal( int );

	inline void MakeTranspose();					// Transposes it.
	Matrix3x3& ReNormalize();
	VectorR3 Solve(const VectorR3&) const;	// Returns solution

	inline void Transform( VectorR3* ) const;
	inline void Transform( const VectorR3& src, VectorR3* dest) const;

protected:
	void OperatorTimesEquals( const Matrix3x3& ); // Internal use only
	void SetZero ();							  // Set to the zero map

};

inline VectorR3 operator* ( const Matrix3x3&, const VectorR3& );

ostream& operator<< ( ostream& os, const Matrix3x3& A );


// *****************************************
// Matrix3x4 class                         *
// * * * * * * * * * * * * * * * * * * * * *

class Matrix3x4
{
public:
	double m11, m12, m13, m21, m22, m23, m31, m32, m33;	
	double m14;
	double m24;
	double m34;

	static const Matrix3x4 Identity;

public:
	// Constructors set by columns!
	Matrix3x4() {}
	Matrix3x4(const VectorR3&, const VectorR3&, const VectorR3&, const VectorR3& ); 
	Matrix3x4(double, double, double, double, double, double,
					 double, double, double, double, double, double );	// Sets by columns
	Matrix3x4( const Matrix3x3&, const VectorR3& );

	void SetIdentity ();		// Set to the identity map
	void Set ( const Matrix3x4& );	// Set to the matrix.
	void Set3x3 ( const Matrix3x3& );	// Set linear part to the matrix.
	void Set ( const Matrix3x3&, const VectorR3& );	// Set to the matrix plus 4th column
	void Set( const VectorR3&, const VectorR3&, const VectorR3&, const VectorR3& );
	void Set( double, double, double,
		 	  double, double, double,
			  double, double, double,
			  double, double, double );	  // Sets by columns
	void Set3x3( double, double, double,
		 		 double, double, double,
				 double, double, double );	  // Sets by columns
	void SetByRows( double, double, double, double, double, double,
					double, double, double, double, double, double );

	void SetColumn1 ( double, double, double );
	void SetColumn2 ( double, double, double );
	void SetColumn3 ( double, double, double );
	void SetColumn4 ( double, double, double );
	void SetColumn1 ( const VectorR3& );
	void SetColumn2 ( const VectorR3& );
	void SetColumn3 ( const VectorR3& );
	void SetColumn4 ( const VectorR3& );
	VectorR3 Column1() const;
	VectorR3 Column2() const;
	VectorR3 Column3() const;
	VectorR3 Column4() const;
	void SetRow1 ( double x, double y, double z, double w );
	void SetRow2 ( double x, double y, double z, double w );
	void SetRow3 ( double x, double y, double z, double w );
	void SetRow4 ( double x, double y, double z, double w );

	Matrix3x4& ApplyTranslationLeft( const VectorR3& u );
	Matrix3x4& ApplyTranslationRight( const VectorR3& u );
	Matrix3x4& ApplyYRotationLeft( double theta );
	Matrix3x4& ApplyYRotationLeft( double costheta, double sintheta );

	Matrix3x4& ReNormalize();
	VectorR3 Solve(const VectorR3&) const;	// Returns solution

	inline void Transform( VectorR3* ) const;
	inline void Transform3x3( VectorR3* ) const;
	inline void Transform( const VectorR3& src, VectorR3*  dest ) const;
	inline void Transform3x3( const VectorR3& src, VectorR3*  dest ) const;
	inline void Transform3x3Transpose( VectorR3*  dest ) const;
	inline void Transform3x3Transpose( const VectorR3& src, VectorR3*  dest ) const;

protected:
	void SetZero ();			// Set to the zero map
	void OperatorTimesEquals( const Matrix3x3& ); // Internal use only
	void OperatorTimesEquals( const Matrix3x4& ); // Internal use only
};

inline VectorR3 operator* ( const Matrix3x4&, const VectorR3& );

ostream& operator<< ( ostream& os, const Matrix3x4& A );


// ***************************************************************
// * 3-space vector and matrix utilities (prototypes)			 *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// Returns the solid angle between vectors v and w.
inline double SolidAngle( const VectorR3& v, const VectorR3& w);

// Returns a righthanded orthonormal basis to complement unit vector x
void GetOrtho( const VectorR3& x,  VectorR3& y, VectorR3& z);
// Returns a vector v orthonormal to unit vector x
void GetOrtho( const VectorR3& x,  VectorR3& y );

// ***************************************************************
// * Stream Output Routines	(Prototypes)						 *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

ostream& operator<< ( ostream& os, const VectorR3& u );


// *****************************************************
// * VectorR3 class - inlined functions				   *
// * * * * * * * * * * * * * * * * * * * * * * * * * * *

inline VectorR3& VectorR3::Load( const double* v ) 
{
	x = *v; 
	y = *(v+1);
	z = *(v+2);
	return *this;
}

inline VectorR3& VectorR3::Load( const float* v ) 
{
	x = *v; 
	y = *(v+1);
	z = *(v+2);
	return *this;
}

inline 	void VectorR3::Dump( double* v ) const
{
	*v = x; 
	*(v+1) = y;
	*(v+2) = z;
}

inline 	void VectorR3::Dump( float* v ) const
{
	*v = (float)x; 
	*(v+1) = (float)y;
	*(v+2) = (float)z;
}

inline double VectorR3::operator[]( int i )
{
	switch (i) {
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		assert(0);
		return 0.0;
	}
}

inline VectorR3& VectorR3::MakeUnit ()			// Convert to unit vector (or leave zero).
{
	double nSq = NormSq();
	if (nSq != 0.0) {
		*this /= sqrt(nSq);
	}
	return *this;
}

inline VectorR3 operator+( const VectorR3& u, const VectorR3& v ) 
{ 
	return VectorR3(u.x+v.x, u.y+v.y, u.z+v.z); 
}
inline VectorR3 operator-( const VectorR3& u, const VectorR3& v ) 
{ 
	return VectorR3(u.x-v.x, u.y-v.y, u.z-v.z); 
}
inline VectorR3 operator*( const VectorR3& u, register double m) 
{ 
	return VectorR3( u.x*m, u.y*m, u.z*m); 
}
inline VectorR3 operator*( register double m, const VectorR3& u) 
{ 
	return VectorR3( u.x*m, u.y*m, u.z*m); 
}
inline VectorR3 operator/( const VectorR3& u, double m) 
{ 
	register double mInv = 1.0/m;
	return VectorR3( u.x*mInv, u.y*mInv, u.z*mInv); 
}

inline int operator==( const VectorR3& u, const VectorR3& v ) 
{
	return ( u.x==v.x && u.y==v.y && u.z==v.z );
}

inline double operator^ ( const VectorR3& u, const VectorR3& v ) // Dot Product
{ 
	return ( u.x*v.x + u.y*v.y + u.z*v.z ); 
}

inline VectorR3 operator* (const VectorR3& u, const VectorR3& v)	// Cross Product
{
	return (VectorR3(	u.y*v.z - u.z*v.y,
					u.z*v.x - u.x*v.z,
					u.x*v.y - u.y*v.x  ) );
}

inline VectorR3 ArrayProd ( const VectorR3& u, const VectorR3& v )
{
	return ( VectorR3( u.x*v.x, u.y*v.y, u.z*v.z ) );
}

inline VectorR3& VectorR3::operator*= (const VectorR3& v)		// Cross Product
{
	double tx=x, ty=y;
	x =  y*v.z -  z*v.y;
	y =  z*v.x - tx*v.z;
	z = tx*v.y - ty*v.x;
	return ( *this );
}

inline VectorR3& VectorR3::ArrayProd (const VectorR3& v)		// Component-wise Product
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return ( *this );
}

inline VectorR3& VectorR3::AddScaled( const VectorR3& u, double s ) 
{
	x += s*u.x;
	y += s*u.y;
	z += s*u.z;
	return(*this);
}

inline VectorR3& VectorR3::ReNormalize()			// Convert near unit back to unit
{
	double nSq = NormSq();
	register double mFact = 1.0-0.5*(nSq-1.0);	// Multiplicative factor
	*this *= mFact;
	return *this;
}

inline double NormalizeError (const VectorR3& u)
{
	register double discrepancy;
	discrepancy = u.x*u.x + u.y*u.y + u.z*u.z - 1.0;
	if ( discrepancy < 0.0 ) {
		discrepancy = -discrepancy;
	}
	return discrepancy;
}

inline double VectorR3::Dist( const VectorR3& u ) const 	// Distance from u
{
	return sqrt( DistSq(u) );
}

inline double VectorR3::DistSq( const VectorR3& u ) const	// Distance from u
{
	return ( (x-u.x)*(x-u.x) + (y-u.y)*(y-u.y) + (z-u.z)*(z-u.z) );
}


// ******************************************************
// * Matrix3x3  class - inlined functions				*
// * * * * * * * * * * * * * * * * * * * * * * * * * * **

inline Matrix3x3::Matrix3x3() {}

inline Matrix3x3::Matrix3x3( const VectorR3& u, const VectorR3& v, 
							 const VectorR3& s )
{
	m11 = u.x;		// Column 1
	m21 = u.y;
	m31 = u.z;
	m12 = v.x;		// Column 2
	m22 = v.y;
	m32 = v.z;
	m13 = s.x;		// Column 3
	m23 = s.y;
	m33 = s.z;
}

inline Matrix3x3::Matrix3x3( double a11, double a21, double a31,
							 double a12, double a22, double a32,
							 double a13, double a23, double a33)
					// Values specified in column order!!!
{
	m11 = a11;		// Row 1
	m12 = a12;
	m13 = a13;
	m21 = a21;		// Row 2
	m22 = a22;
	m23 = a23;
	m31 = a31;		// Row 3
	m32 = a32;
	m33 = a33;
}
	
inline void Matrix3x3::SetIdentity ( )
{
	m11 = m22 = m33 = 1.0;
	m12 = m13 = m21 = m23 = m31 = m32 = 0.0;
}

inline void Matrix3x3::SetZero( ) 
{
	m11 = m12 = m13 = m21 = m22 = m23 = m31 = m32 = m33 = 0.0;
}

inline void Matrix3x3::Set ( const Matrix3x3& A )	// Set to the matrix.
{
	m11 = A.m11;
	m21 = A.m21;
	m31 = A.m31;
	m12 = A.m12;
	m22 = A.m22;
	m32 = A.m32;
	m13 = A.m13;
	m23 = A.m23;
	m33 = A.m33;
}

inline void Matrix3x3::Set3x3 ( const Matrix3x4& A )	// Set to the 3x3 part of the matrix.
{
	m11 = A.m11;
	m21 = A.m21;
	m31 = A.m31;
	m12 = A.m12;
	m22 = A.m22;
	m32 = A.m32;
	m13 = A.m13;
	m23 = A.m23;
	m33 = A.m33;
}

inline void Matrix3x3::Set( const VectorR3& u, const VectorR3& v, 
							 const VectorR3& w)
{
	m11 = u.x;		// Column 1
	m21 = u.y;
	m31 = u.z;
	m12 = v.x;		// Column 2
	m22 = v.y;
	m32 = v.z;
	m13 = w.x;		// Column 3
	m23 = w.y;
	m33 = w.z;
}

inline void Matrix3x3::Set( double a11, double a21, double a31, 
							 double a12, double a22, double a32,
							 double a13, double a23, double a33)
					// Values specified in column order!!!
{
	m11 = a11;		// Row 1
	m12 = a12;
	m13 = a13;
	m21 = a21;		// Row 2
	m22 = a22;
	m23 = a23;
	m31 = a31;		// Row 3
	m32 = a32;
	m33 = a33;
}

inline void Matrix3x3::SetByRows( double a11, double a12, double a13, 
							 double a21, double a22, double a23,
							 double a31, double a32, double a33)
					// Values specified in row order!!!
{
	m11 = a11;		// Row 1
	m12 = a12;
	m13 = a13;
	m21 = a21;		// Row 2
	m22 = a22;
	m23 = a23;
	m31 = a31;		// Row 3
	m32 = a32;
	m33 = a33;
}

inline void Matrix3x3::SetByRows( const VectorR3& u, const VectorR3& v, 
									const VectorR3& s )
{
	m11 = u.x;		// Row 1
	m12 = u.y;
	m13 = u.z;
	m21 = v.x;		// Row 2
	m22 = v.y;
	m23 = v.z;
	m31 = s.x;		// Row 3
	m32 = s.y;
	m33 = s.z;
}

inline void Matrix3x3::SetColumn1 ( double x, double y, double z)
{
	m11 = x; m21 = y; m31= z;
}

inline void Matrix3x3::SetColumn2 ( double x, double y, double z)
{
	m12 = x; m22 = y; m32= z;
}

inline void Matrix3x3::SetColumn3 ( double x, double y, double z)
{
	m13 = x; m23 = y; m33= z;
}

inline void Matrix3x3::SetColumn1 ( const VectorR3& u )
{
	m11 = u.x; m21 = u.y; m31 = u.z;
}

inline void Matrix3x3::SetColumn2 ( const VectorR3& u )
{
	m12 = u.x; m22 = u.y; m32 = u.z;
}

inline void Matrix3x3::SetColumn3 ( const VectorR3& u )
{
	m13 = u.x; m23 = u.y; m33 = u.z;
}

inline void Matrix3x3::SetRow1 ( double x, double y, double z )
{
	m11 = x;
	m12 = y;
	m13 = z;
}

inline void Matrix3x3::SetRow2 ( double x, double y, double z )
{
	m21 = x;
	m22 = y;
	m23 = z;
}

inline void Matrix3x3::SetRow3 ( double x, double y, double z )
{
	m31 = x;
	m32 = y;
	m33 = z;
}



inline VectorR3 Matrix3x3::Column1() const
{
	return ( VectorR3(m11, m21, m31) );
}

inline VectorR3 Matrix3x3::Column2() const
{
	return ( VectorR3(m12, m22, m32) );
}

inline VectorR3 Matrix3x3::Column3() const
{
	return ( VectorR3(m13, m23, m33) );
}

inline VectorR3 Matrix3x3::Row1() const
{
	return ( VectorR3(m11, m12, m13) );
}

inline VectorR3 Matrix3x3::Row2() const
{
	return ( VectorR3(m21, m22, m23) );
}

inline VectorR3 Matrix3x3::Row3() const
{
	return ( VectorR3(m31, m32, m33) );
}

inline void Matrix3x3::SetDiagonal( double x, double y, double z )
{
	m11 = x;
	m22 = y;
	m33 = z;
}

inline void Matrix3x3::SetDiagonal( const VectorR3& u )
{
	SetDiagonal ( u.x, u.y, u.z );
}

inline double Matrix3x3::Diagonal( int i ) 
{
	switch (i) {
	case 0:
		return m11;
	case 1:
		return m22;
	case 2:
		return m33;
	default:
		assert(0);
		return 0.0;
	}
}

inline void Matrix3x3::MakeTranspose()	// Transposes it.
{
	register double temp;
	temp = m12;
	m12 = m21;
	m21=temp;
	temp = m13;
	m13 = m31;
	m31 = temp;
	temp = m23;
	m23 = m32;
	m32 = temp;
}

inline VectorR3 operator* ( const Matrix3x3& A, const VectorR3& u)
{
	return( VectorR3( A.m11*u.x + A.m12*u.y + A.m13*u.z,
					  A.m21*u.x + A.m22*u.y + A.m23*u.z,
					  A.m31*u.x + A.m32*u.y + A.m33*u.z ) ); 
}

inline void Matrix3x3::Transform( VectorR3* u ) const {
	double newX, newY;
	newX = m11*u->x + m12*u->y + m13*u->z;
	newY = m21*u->x + m22*u->y + m23*u->z;
	u->z = m31*u->x + m32*u->y + m33*u->z;
	u->x = newX;
	u->y = newY;
}

inline void Matrix3x3::Transform( const VectorR3& src, VectorR3* dest ) const {
	dest->x = m11*src.x + m12*src.y + m13*src.z;
	dest->y = m21*src.x + m22*src.y + m23*src.z;
	dest->z = m31*src.x + m32*src.y + m33*src.z;
}


// ******************************************************
// * Matrix3x4  class - inlined functions				*
// * * * * * * * * * * * * * * * * * * * * * * * * * * **

inline Matrix3x4::Matrix3x4(const VectorR3& u, const VectorR3& v, 
							const VectorR3& s, const VectorR3& t)
{
	m11 = u.x;		// Column 1
	m21 = u.y;
	m31 = u.z;
	m12 = v.x;		// Column 2
	m22 = v.y;
	m32 = v.z;
	m13 = s.x;		// Column 3
	m23 = s.y;
	m33 = s.z;
	m14 = t.x;
	m24 = t.y;
	m34 = t.z;
}

inline Matrix3x4::Matrix3x4(double a11, double a21, double a31, 
				  double a12, double a22, double a32,
				  double a13, double a23, double a33, 
				  double a14, double a24, double a34)
{									// Values in COLUMN order!
	m11 = a11;		// Row 1
	m12 = a12;
	m13 = a13;
	m14 = a14;
	m21 = a21;		// Row 2
	m22 = a22;
	m23 = a23;
	m24 = a24;
	m31 = a31;		// Row 3
	m32 = a32;
	m33 = a33;
	m34 = a34;
}

inline Matrix3x4::Matrix3x4( const Matrix3x3& A, const VectorR3& u )
{
	Set(A, u);
}

inline void Matrix3x4::SetIdentity ()		// Set to the identity map
{
	m11 = m22 = m33 = 1.0;
	m12 = m13 = m21 = m23 = m31 = m32 = 0.0;
	m14 = m24 = m34 = 0.0;
}

inline void Matrix3x4::SetZero ()			// Set to the zero map
{
	m11 = m22 = m33 = 0.0;
	m12 = m13 = m21 = m23 = m31 = m32 = 0.0;
	m14 = m24 = m34 = 0.0;
}

inline void Matrix3x4::Set ( const Matrix3x4& A )	// Set to the matrix.
{
	m11 = A.m11;
	m21 = A.m21;
	m31 = A.m31;
	m12 = A.m12;
	m22 = A.m22;
	m32 = A.m32;
	m13 = A.m13;
	m23 = A.m23;
	m33 = A.m33;
	m14 = A.m14;
	m24 = A.m24;
	m34 = A.m34;
}

inline void Matrix3x4::Set ( const Matrix3x3& A, const VectorR3& t )	// Set to the matrix plus 4th column
{
	m11 = A.m11;
	m21 = A.m21;
	m31 = A.m31;
	m12 = A.m12;
	m22 = A.m22;
	m32 = A.m32;
	m13 = A.m13;
	m23 = A.m23;
	m33 = A.m33;
	m14 = t.x;		// Column 4
	m24 = t.y;
	m34 = t.z;
}

// Set linear part to the matrix 
inline void Matrix3x4::Set3x3 ( const Matrix3x3& A )
{
	m11 = A.m11;
	m21 = A.m21;
	m31 = A.m31;
	m12 = A.m12;
	m22 = A.m22;
	m32 = A.m32;
	m13 = A.m13;
	m23 = A.m23;
	m33 = A.m33;
}

inline void Matrix3x4::Set( const VectorR3& u, const VectorR3& v, 
						    const VectorR3& w, const VectorR3& t )
{
	m11 = u.x;		// Column 1
	m21 = u.y;
	m31 = u.z;
	m12 = v.x;		// Column 2
	m22 = v.y;
	m32 = v.z;
	m13 = w.x;		// Column 3
	m23 = w.y;
	m33 = w.z;
	m14 = t.x;		// Column 4
	m24 = t.y;
	m34 = t.z;
}

inline void Matrix3x4::Set( double a11, double a21, double a31, 
							 double a12, double a22, double a32,
							 double a13, double a23, double a33,
							 double a14, double a24, double a34 )
					// Values specified in column order!!!
{
	m11 = a11;		// Row 1
	m12 = a12;
	m13 = a13;
	m14 = a14;
	m21 = a21;		// Row 2
	m22 = a22;
	m23 = a23;
	m24 = a24;
	m31 = a31;		// Row 3
	m32 = a32;
	m33 = a33;
	m34 = a34;
}

inline void Matrix3x4::Set3x3( double a11, double a21, double a31, 
							 double a12, double a22, double a32,
							 double a13, double a23, double a33 )
					// Values specified in column order!!!
{
	m11 = a11;		// Row 1
	m12 = a12;
	m13 = a13;
	m21 = a21;		// Row 2
	m22 = a22;
	m23 = a23;
	m31 = a31;		// Row 3
	m32 = a32;
	m33 = a33;
}

inline void Matrix3x4::SetByRows( double a11, double a12, double a13, double a14,
							 double a21, double a22, double a23, double a24,
							 double a31, double a32, double a33, double a34 )
					// Values specified in row order!!!
{
	m11 = a11;		// Row 1
	m12 = a12;
	m13 = a13;
	m14 = a14;
	m21 = a21;		// Row 2
	m22 = a22;
	m23 = a23;
	m24 = a24;
	m31 = a31;		// Row 3
	m32 = a32;
	m33 = a33;
	m34 = a34;
}

inline void Matrix3x4::SetColumn1 ( double x, double y, double z)
{
	m11 = x; m21 = y; m31= z;
}

inline void Matrix3x4::SetColumn2 ( double x, double y, double z)
{
	m12 = x; m22 = y; m32= z;
}

inline void Matrix3x4::SetColumn3 ( double x, double y, double z)
{
	m13 = x; m23 = y; m33= z;
}

inline void Matrix3x4::SetColumn4 ( double x, double y, double z )
{
	m14 = x; m24 = y; m34= z;
}

inline void Matrix3x4::SetColumn1 ( const VectorR3& u )
{
	m11 = u.x; m21 = u.y; m31 = u.z;
}

inline void Matrix3x4::SetColumn2 ( const VectorR3& u )
{
	m12 = u.x; m22 = u.y; m32 = u.z;
}

inline void Matrix3x4::SetColumn3 ( const VectorR3& u )
{
	m13 = u.x; m23 = u.y; m33 = u.z;
}

inline void Matrix3x4::SetColumn4 ( const VectorR3& u )
{
	m14 = u.x;	m24 = u.y;	m34 = u.z;
}

inline VectorR3 Matrix3x4::Column1() const
{
	return ( VectorR3(m11, m21, m31) );
}

inline VectorR3 Matrix3x4::Column2() const
{
	return ( VectorR3(m12, m22, m32) );
}

inline VectorR3 Matrix3x4::Column3() const
{
	return ( VectorR3(m13, m23, m33) );
}

inline VectorR3 Matrix3x4::Column4() const
{
	return ( VectorR3(m14, m24, m34) );
}

inline void Matrix3x4::SetRow1 ( double x, double y, double z, double w )
{
	m11 = x;
	m12 = y;
	m13 = z;
	m14 = w;
}

inline void Matrix3x4::SetRow2 ( double x, double y, double z, double w )
{
	m21 = x;
	m22 = y;
	m23 = z;
	m24 = w;
}

inline void Matrix3x4::SetRow3 ( double x, double y, double z, double w )
{
	m31 = x;
	m32 = y;
	m33 = z;
	m34 = w;
}

// Left multiply with a translation (so the translation is applied afterwards).
inline Matrix3x4& Matrix3x4::ApplyTranslationLeft( const VectorR3& u )
{
	m14 += u.x;
	m24 += u.y;
	m34 += u.z;
	return *this;
}

// Right multiply with a translation (so the translation is applied first).
inline Matrix3x4& Matrix3x4::ApplyTranslationRight( const VectorR3& u )
{
	double new14 = m14 + m11*u.x + m12*u.y + m13*u.z;
	double new24 = m24 + m21*u.x + m22*u.y + m23*u.z;
	m34 = m34 + m31*u.x + m32*u.y + m33*u.z;
	m14 = new14;
	m24 = new24;
	return *this;
}

// Left-multiply with a rotation around the y-axis.
inline Matrix3x4& Matrix3x4::ApplyYRotationLeft( double theta ) 
{
	double costheta = cos(theta);
	double sintheta = sin(theta);
	return ApplyYRotationLeft( costheta, sintheta );
}

inline Matrix3x4& Matrix3x4::ApplyYRotationLeft( double costheta, double sintheta ) 
{
	double tmp;
	tmp = costheta*m11+sintheta*m31;
	m31 = costheta*m31-sintheta*m11;
	m11 = tmp;

	tmp = costheta*m12+sintheta*m32;
	m32 = costheta*m32-sintheta*m12;
	m12 = tmp;

	tmp = costheta*m13+sintheta*m33;
	m33 = costheta*m33-sintheta*m13;
	m13 = tmp;

	tmp = costheta*m14+sintheta*m34;
	m34 = costheta*m34-sintheta*m14;
	m14 = tmp;

	return *this;
}

inline VectorR3 Matrix3x4::Solve(const VectorR3& u) const	// Returns solution
{
	Matrix3x3 A;
	A.Set3x3(*this);
	return ( A.Solve( VectorR3(m14-u.x, m24-u.y, m34-u.z) ) );
}

inline void Matrix3x4::Transform( VectorR3* u ) const {
	double newX, newY;
	newX = m11*u->x + m12*u->y + m13*u->z + m14;
	newY = m21*u->x + m22*u->y + m23*u->z + m24;
	u->z = m31*u->x + m32*u->y + m33*u->z + m34;
	u->x = newX;
	u->y = newY;
}

inline void Matrix3x4::Transform3x3( VectorR3* u ) const {
	double newX, newY;
	newX = m11*u->x + m12*u->y + m13*u->z;
	newY = m21*u->x + m22*u->y + m23*u->z;
	u->z = m31*u->x + m32*u->y + m33*u->z;
	u->x = newX;
	u->y = newY;
}

inline void Matrix3x4::Transform( const VectorR3& src, VectorR3* dest ) const {
	dest->x = m11*src.x + m12*src.y + m13*src.z + m14;
	dest->y = m21*src.x + m22*src.y + m23*src.z + m24;
	dest->z = m31*src.x + m32*src.y + m33*src.z + m34;
}

inline void Matrix3x4::Transform3x3( const VectorR3& src, VectorR3* dest ) const {
	dest->x = m11*src.x + m12*src.y + m13*src.z;
	dest->y = m21*src.x + m22*src.y + m23*src.z;
	dest->z = m31*src.x + m32*src.y + m33*src.z;
}

inline void Matrix3x4::Transform3x3Transpose( VectorR3* u ) const {
	double newX, newY;
	newX = m11*u->x + m21*u->y + m31*u->z;
	newY = m12*u->x + m22*u->y + m32*u->z;
	u->z = m13*u->x + m23*u->y + m33*u->z;
	u->x = newX;
	u->y = newY;
}

inline void Matrix3x4::Transform3x3Transpose( const VectorR3& src, VectorR3* dest ) const {
	dest->x = m11*src.x + m21*src.y + m31*src.z;
	dest->y = m12*src.x + m22*src.y + m32*src.z;
	dest->z = m13*src.x + m23*src.y + m33*src.z;
}

inline VectorR3 operator* ( const Matrix3x4& A, const VectorR3& u )
{
	return( VectorR3( A.m11*u.x + A.m12*u.y + A.m13*u.z + A.m14,
					  A.m21*u.x + A.m22*u.y + A.m23*u.z + A.m24,
					  A.m31*u.x + A.m32*u.y + A.m33*u.z + A.m34) ); 
}

#endif