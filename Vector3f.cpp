#include "VectorMatrix.h"
Vector3f Vector3f::operator+(Vector3f&v) 
{
	return Vector3f(x + v.x, y + v.y, z + v.z);
};
float Vector3f::operator*(Vector3f&v) 
{
	return x*v.x + y*v.y + z*v.z;
};
Vector3f Vector3f::operator*(float s)
{
	return Vector3f(x*s, y*s, z*s);
}
Vector3f Vector3f::operator-(Vector3f&v)
{
	return Vector3f(x - v.x, y - v.y, z - v.z);
}
Vector3f Vector3f::operator^(Vector3f&v)
{
	return Vector3f(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
};
Vector3f Vector3f::operator=(Vector3f&v) 
{
	return Vector3f(x = v.x, y = v.y, z = v.z);
};
float Vector3f::Length() 
{
	return sqrt(x*x+y*y+z*z);
};
void Vector3f::Normalize() 
{
	float len = Length();
	x /= len;
	y /= len;
	z /= len;
};