
#include "stdafx.h"

//#include "MathMisc.h"
#include "LinearR3.h"
//#include "Spherical.h"

// ******************************************************
// * VectorR3 class - math library functions				*
// * * * * * * * * * * * * * * * * * * * * * * * * * * **

const double PI = 3.1415926535897932384626433832795028841972;

const VectorR3 UnitVecIR3(1.0, 0.0, 0.0);
const VectorR3 UnitVecJR3(0.0, 1.0, 0.0);
const VectorR3 UnitVecKR3(0.0, 0.0, 1.0);

const VectorR3 VectorR3::Zero(0.0, 0.0, 0.0);
const VectorR3 VectorR3::UnitX( 1.0, 0.0, 0.0);
const VectorR3 VectorR3::UnitY( 0.0, 1.0, 0.0);
const VectorR3 VectorR3::UnitZ( 0.0, 0.0, 1.0);
const VectorR3 VectorR3::NegUnitX(-1.0, 0.0, 0.0);
const VectorR3 VectorR3::NegUnitY( 0.0,-1.0, 0.0);
const VectorR3 VectorR3::NegUnitZ( 0.0, 0.0,-1.0);

const Matrix3x3 Matrix3x3::Identity(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
const Matrix3x4 Matrix3x4::Identity(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0);

double VectorR3::MaxAbs() const
{
	register double m;
	m = (x>0.0) ? x : -x;
	if ( y>m ) m=y;
	else if ( -y >m ) m = -y;
	if ( z>m ) m=z;
	else if ( -z>m ) m = -z;
	return m;
}


// *********************************************************************
// Rotation routines												   *
// *********************************************************************

// s.Rotate(theta, u) rotates s and returns s 
//        rotated theta degrees around unit vector w.
VectorR3& VectorR3::Rotate( double theta, const VectorR3& w) 
{
	double c = cos(theta);
	double s = sin(theta);
	double dotw = (x*w.x + y*w.y + z*w.z);
	double v0x = dotw*w.x;
	double v0y = dotw*w.y;		// v0 = provjection onto w
	double v0z = dotw*w.z;
	double v1x = x-v0x;
	double v1y = y-v0y;			// v1 = projection onto plane normal to w
	double v1z = z-v0z;
	double v2x = w.y*v1z - w.z*v1y;
	double v2y = w.z*v1x - w.x*v1z;	// v2 = w * v1 (cross product)
	double v2z = w.x*v1y - w.y*v1x;
	
	x = v0x + c*v1x + s*v2x;
	y = v0y + c*v1y + s*v2y;
	z = v0z	+ c*v1z + s*v2z;

	return ( *this );
}

// Rotate unit vector x in the direction of "dir": length of dir is rotation angle.
//		x must be a unit vector.  dir must be perpindicular to x.
VectorR3& VectorR3::RotateUnitInDirection ( const VectorR3& dir)
{	
	double theta = dir.NormSq();
	if ( theta==0.0 ) {
		return *this;
	}
	else {
		theta = sqrt(theta);
		double costheta = cos(theta);
		double sintheta = sin(theta);
		VectorR3 dirUnit = dir/theta;
		*this = costheta*(*this) + sintheta*dirUnit;
		return ( *this );
	}
}

// ******************************************************
// * Matrix3x3 class - math library functions			*
// * * * * * * * * * * * * * * * * * * * * * * * * * * **

Matrix3x3& Matrix3x3::ReNormalize()	// Re-normalizes nearly orthonormal matrix
{
	register double alpha = m11*m11+m21*m21+m31*m31;	// First column's norm squared
	register double beta  = m12*m12+m22*m22+m32*m32;	// Second column's norm squared
	register double gamma = m13*m13+m23*m23+m33*m33;	// Third column's norm squared
	alpha = 1.0 - 0.5*(alpha-1.0);				// Get mult. factor
	beta  = 1.0 - 0.5*(beta-1.0);
	gamma = 1.0 - 0.5*(gamma-1.0);
	m11 *= alpha;								// Renormalize first column
	m21 *= alpha;
	m31 *= alpha;
	m12 *= beta;								// Renormalize second column
	m22 *= beta;
	m32 *= beta;
	m13 *= gamma;
	m23 *= gamma;
	m33 *= gamma;
	alpha = m11*m12+m21*m22+m31*m32;		// First and second column dot product
	beta  = m11*m13+m21*m23+m31*m33;		// First and third column dot product
	gamma = m12*m13+m22*m23+m32*m33;		// Second and third column dot product
	alpha *= 0.5;
	beta *= 0.5;
	gamma *= 0.5;
	register double temp1, temp2;
	temp1 = m11-alpha*m12-beta*m13;			// Update row1
	temp2 = m12-alpha*m11-gamma*m13;
	m13 -= beta*m11+gamma*m12;
	m11 = temp1;
	m12 = temp2;
	temp1 = m21-alpha*m22-beta*m23;			// Update row2
	temp2 = m22-alpha*m21-gamma*m23;
	m23 -= beta*m21+gamma*m22;
	m21 = temp1;
	m22 = temp2;
	temp1 = m31-alpha*m32-beta*m33;			// Update row3
	temp2 = m32-alpha*m31-gamma*m33;
	m33 -= beta*m31+gamma*m32;
	m31 = temp1;
	m32 = temp2;
	return *this;
}

void Matrix3x3::OperatorTimesEquals(const Matrix3x3& B)	 // Matrix product
{
	double t1, t2;		// temporary values
	t1 =  m11*B.m11 + m12*B.m21 + m13*B.m31;
	t2 =  m11*B.m12 + m12*B.m22 + m13*B.m32;
	m13 = m11*B.m13 + m12*B.m23 + m13*B.m33;
	m11 = t1;
	m12 = t2;

	t1 =  m21*B.m11 + m22*B.m21 + m23*B.m31;
	t2 =  m21*B.m12 + m22*B.m22 + m23*B.m32;
	m23 = m21*B.m13 + m22*B.m23 + m23*B.m33;
	m21 = t1;
	m22 = t2;

	t1 =  m31*B.m11 + m32*B.m21 + m33*B.m31;
	t2 =  m31*B.m12 + m32*B.m22 + m33*B.m32;
	m33 = m31*B.m13 + m32*B.m23 + m33*B.m33;
	m31 = t1;
	m32 = t2;
	return;
}

VectorR3 Matrix3x3::Solve(const VectorR3& u) const	// Returns solution
{												// based on Cramer's rule
	double sd11 = m22*m33-m23*m32;
	double sd21 = m32*m13-m12*m33;
	double sd31 = m12*m23-m22*m13;
	double sd12 = m31*m23-m21*m33;
	double sd22 = m11*m33-m31*m13;
	double sd32 = m21*m13-m11*m23;
	double sd13 = m21*m32-m31*m22;
	double sd23 = m31*m12-m11*m32;
	double sd33 = m11*m22-m21*m12;

	register double detInv = 1.0/(m11*sd11 + m12*sd12 + m13*sd13);

	double rx = (u.x*sd11 + u.y*sd21 + u.z*sd31)*detInv;
	double ry = (u.x*sd12 + u.y*sd22 + u.z*sd32)*detInv;
	double rz = (u.x*sd13 + u.y*sd23 + u.z*sd33)*detInv;

	return ( VectorR3( rx, ry, rz ) );
}


// ******************************************************
// * Matrix3x4 class - math library functions			*
// * * * * * * * * * * * * * * * * * * * * * * * * * * **

Matrix3x4& Matrix3x4::ReNormalize()	// Re-normalizes nearly orthonormal matrix
{
	register double alpha = m11*m11+m21*m21+m31*m31;	// First column's norm squared
	register double beta  = m12*m12+m22*m22+m32*m32;	// Second column's norm squared
	register double gamma = m13*m13+m23*m23+m33*m33;	// Third column's norm squared
	alpha = 1.0 - 0.5*(alpha-1.0);				// Get mult. factor
	beta  = 1.0 - 0.5*(beta-1.0);
	gamma = 1.0 - 0.5*(gamma-1.0);
	m11 *= alpha;								// Renormalize first column
	m21 *= alpha;
	m31 *= alpha;
	m12 *= beta;								// Renormalize second column
	m22 *= beta;
	m32 *= beta;
	m13 *= gamma;
	m23 *= gamma;
	m33 *= gamma;
	alpha = m11*m12+m21*m22+m31*m32;		// First and second column dot product
	beta  = m11*m13+m21*m23+m31*m33;		// First and third column dot product
	gamma = m12*m13+m22*m23+m32*m33;		// Second and third column dot product
	alpha *= 0.5;
	beta *= 0.5;
	gamma *= 0.5;
	register double temp1, temp2;
	temp1 = m11-alpha*m12-beta*m13;			// Update row1
	temp2 = m12-alpha*m11-gamma*m13;
	m13 -= beta*m11+gamma*m12;
	m11 = temp1;
	m12 = temp2;
	temp1 = m21-alpha*m22-beta*m23;			// Update row2
	temp2 = m22-alpha*m21-gamma*m23;
	m23 -= beta*m21+gamma*m22;
	m21 = temp1;
	m22 = temp2;
	temp1 = m31-alpha*m32-beta*m33;			// Update row3
	temp2 = m32-alpha*m31-gamma*m33;
	m33 -= beta*m31+gamma*m32;
	m31 = temp1;
	m32 = temp2;
	return *this;
}

void Matrix3x4::OperatorTimesEquals (const Matrix3x4& B)	// Composition
{
	m14 += m11*B.m14 + m12*B.m24 + m13*B.m34;
	m24 += m21*B.m14 + m22*B.m24 + m23*B.m34;
	m34 += m31*B.m14 + m32*B.m24 + m33*B.m34;

	double t1, t2;		// temporary values
	t1 =  m11*B.m11 + m12*B.m21 + m13*B.m31;
	t2 =  m11*B.m12 + m12*B.m22 + m13*B.m32;
	m13 = m11*B.m13 + m12*B.m23 + m13*B.m33;
	m11 = t1;
	m12 = t2;

	t1 =  m21*B.m11 + m22*B.m21 + m23*B.m31;
	t2 =  m21*B.m12 + m22*B.m22 + m23*B.m32;
	m23 = m21*B.m13 + m22*B.m23 + m23*B.m33;
	m21 = t1;
	m22 = t2;

	t1 =  m31*B.m11 + m32*B.m21 + m33*B.m31;
	t2 =  m31*B.m12 + m32*B.m22 + m33*B.m32;
	m33 = m31*B.m13 + m32*B.m23 + m33*B.m33;
	m31 = t1;
	m32 = t2;
}

void Matrix3x4::OperatorTimesEquals (const Matrix3x3& B)	// Composition
{
	double t1, t2;		// temporary values
	t1 =  m11*B.m11 + m12*B.m21 + m13*B.m31;
	t2 =  m11*B.m12 + m12*B.m22 + m13*B.m32;
	m13 = m11*B.m13 + m12*B.m23 + m13*B.m33;
	m11 = t1;
	m12 = t2;

	t1 =  m21*B.m11 + m22*B.m21 + m23*B.m31;
	t2 =  m21*B.m12 + m22*B.m22 + m23*B.m32;
	m23 = m21*B.m13 + m22*B.m23 + m23*B.m33;
	m21 = t1;
	m22 = t2;

	t1 =  m31*B.m11 + m32*B.m21 + m33*B.m31;
	t2 =  m31*B.m12 + m32*B.m22 + m33*B.m32;
	m33 = m31*B.m13 + m32*B.m23 + m33*B.m33;
	m31 = t1;
	m32 = t2;
}

// ***************************************************************
//  Linear Algebra Utilities									 *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// Returns a righthanded orthonormal basis to complement vector u
void GetOrtho( const VectorR3& u,  VectorR3& v, VectorR3& w)
{
	if ( u.x > 0.5 || u.x<-0.5 || u.y > 0.5 || u.y<-0.5 ) {
		v.Set ( u.y, -u.x, 0.0 );
	}
	else {
		v.Set ( 0.0, u.z, -u.y);
	}
	v.Normalize();
	w = u;
	w *= v;
	w.Normalize();
	// w.NormalizeFast();
	return;
}

// Returns a vector v orthonormal to unit vector u
void GetOrtho( const VectorR3& u,  VectorR3& v )
{
	if ( u.x > 0.5 || u.x<-0.5 || u.y > 0.5 || u.y<-0.5 ) {
		v.Set ( u.y, -u.x, 0.0 );
	}
	else {
		v.Set ( 0.0, u.z, -u.y);
	}
	v.Normalize();
	return;
}

// ***************************************************************
//  Stream Output Routines										 *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

ostream& operator<< ( ostream& os, const VectorR3& u )
{
	return (os << "<" << u.x << "," << u.y << "," << u.z << ">");
}

ostream& operator<< ( ostream& os, const Matrix3x3& A )
{
	os << " <" << A.m11 << ", " << A.m12 << ", " << A.m13  << ">\n"
	   << " <" << A.m21 << ", " << A.m22 << ", " << A.m23  << ">\n"
	   << " <" << A.m31 << ", " << A.m32 << ", " << A.m33  << ">\n" ;
	return (os);
}

ostream& operator<< ( ostream& os, const Matrix3x4& A )
{
	os << " <" << A.m11 << ", " << A.m12 << ", " << A.m13 
			<< "; " << A.m14 << ">\n"
	   << " <" << A.m21 << ", " << A.m22 << ", " << A.m23 
			<< "; " << A.m24 << ">\n"
	   << " <" << A.m31 << ", " << A.m32 << ", " << A.m33 
			<< "; " << A.m34 << ">\n" ;
	return (os);
}

