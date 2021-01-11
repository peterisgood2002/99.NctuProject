#include <memory.h>
#include "stdafx.h"
#include "Math3D.h"

void Vector3::set(Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

void Vector3::set(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3::flip()
{
	x = -x;
	y = -y;
	z = -z;
}

void Vector3::setDiff(Vector3& a, Vector3& b)
{
	x = a.x - b.x;
	y = a.y - b.y;
	z = a.z - b.z;
}

Vector3 Vector3::normalize()
{
	if(norm() < 0.0000001)
	{
		set(1,1,1);
		normalize();
		return *this;
	}
	double len = length();

	x /= len;
	y /= len;
	z /= len;
	return *this;
}

Vector3 Vector3::cross(Vector3& b) //return this cross b
{
	Vector3 result(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
	result.normalize();
	return result;
}

double Vector3::dot(Vector3& b) // return this dotted with b
{
	return x * b.x + y * b.y + z * b.z;
}

Vector3 Vector3::operator +(const Vector3 & rhs)
{
	Vector3 r;

	r.x = x + rhs.x;
	r.y = y + rhs.y;
	r.z = z + rhs.z;

	return r;
}

Vector3 Vector3::operator -(const Vector3 & rhs)
{
	Vector3 r;

	r.x = x - rhs.x;
	r.y = y - rhs.y;
	r.z = z - rhs.z;

	return r;
}

Vector3 Vector3::operator *(double scalar) const
{
	Vector3 r;

	r.x = x * scalar;
	r.y = y * scalar;
	r.z = z * scalar;

	return r;
}

Vector3 Vector3::operator /(double scalar) const
{
	return (*this)*(1/scalar);
}

Vector3& Vector3::operator =(const Vector3 &rhs)
{
	// self assignment control
	if (this == &rhs) 
		return *this;

	x = rhs.x;
	y = rhs.y;
	z = rhs.z;

    return *this;  // Assignment operator returns left side.
}

bool Vector3::operator ==(const Vector3 & rhs)
{
	if(x == rhs.x && y == rhs.y && z == rhs.z)
		return true;
	else
		return false;
}

bool Vector3::operator !=(const Vector3 & rhs)
{
	if(x == rhs.x && y == rhs.y && z == rhs.z)
		return false;
	else
		return true;
}

Vector3 Vector3::interpolate(Vector3& v0, Vector3& v1, double t)
{
	Vector3 inter;
	inter.x = v0.x + (v1.x - v0.x) * t;
	inter.y = v0.y + (v1.y - v0.y) * t;
	inter.z = v0.z + (v1.z - v0.z) * t;
	return inter;
}