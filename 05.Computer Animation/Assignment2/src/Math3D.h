#ifndef MATH3D_H
#define MATH3D_H

#include <math.h>

const double PI = 3.14159265358979323846f;		// Pi
#define	DEGTORAD(x)	( ((x) * PI) / 180.0f )
#define degtorad PI / 180.0f
#define	RADTODEG(x)	( ((x) * 180.0f) / PI )
#define radtodeg 180.0f / PI

class Vector3
{

public:
	// constructor
	Vector3() : x(0), y(0), z(0) {};					
	Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {};
	Vector3(double tx, double ty, double tz) : x(tx), y(ty), z(tz) {};
	Vector3(double* p) : x(p[0]), y(p[1]), z(p[2]) {};

	void flip(); // reverse this vector
	void set(Vector3& v);
	void set(double x, double y, double z);
	void setDiff(Vector3& a, Vector3& b);
	Vector3 normalize();

	Vector3 cross(Vector3& b); //return this cross b
	double dot(Vector3& b); // return this dotted with b

	Vector3 operator +(const Vector3& rhs);
	Vector3 operator -(const Vector3& rhs);
	Vector3 operator *(const double scalar) const;
	Vector3 operator /(const double scalar) const;
	Vector3& operator =(const Vector3 & rhs);
	bool operator ==(const Vector3 & rhs);
	bool operator !=(const Vector3 & rhs);
	static Vector3 interpolate(Vector3& v0, Vector3& v1, double t);

	inline double norm(){return x*x+y*y+z*z;}
	inline double length(){return sqrt(norm());}

public:
	double x,y,z;
};

#endif