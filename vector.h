/*
 * File:   vector.h
 *
 * The class Vector
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

struct Vector {
  Vector() {};
  Vector(double x, double y) : x(x), y(y), z(0) {};
  Vector(double x, double y, double z) : x(x), y(y), z(z) {};

  Vector& operator+=(const Vector& v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  Vector& operator/=(double a)
  {
    x /= a;
    y /= a;
    z /= a;
    return *this;
  }

  Vector& operator*=(double a)
  {
    x *= a;
    y *= a;
    z *= a;
    return *this;
  }

  //the vector is rotated around the z axix for angle radians
  void rotate(double angle)
  {
    x = cos(angle) * x - sin(angle) * y;
    y = sin(angle) * x + cos(angle) * y;
  }

  double norm() const
  {
    return sqrt(x*x+y*y+z*z);
  }

  double x, y, z;
};

inline Vector operator+(Vector v1, const Vector& v2)
{
  v1 += v2;
  return v1;
}


inline Vector operator*(Vector v, double a)
{
  v *= a;
  return v;
}

inline Vector operator/(Vector v, double a)
{
  v /= a;
  return v;
}

typedef Vector Point;

#endif /* VECTOR_H */
