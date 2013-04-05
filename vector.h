/*
 * File:   vector.h
 *
 * The class Vector
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

struct Vector {
  Vector() {};
  Vector(double x, double y) : x(x), y(y), z(0) {};
  Vector(double x, double y, double z) : x(x), y(y), z(z) {};

  //the vector is rotated around the z axix for angle radians
  void rotate(double angle);
  //the vector is rotated around the axis that lies on the x y plane and
  //wich is perpendicular to the vector for angle radians
  void rotate_axis_xy(double angle);
  void normalize();
  double norm() const;
  //returns the inclination angle of the vector from the horizontal
  //in radians
  double inclination_angle() const;
  //returns the angle of the vector with the x axis on the horizonal plane
  //in radians
  double angle_on_xyplane() const;

  Vector& operator+=(const Vector& v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  Vector& operator-=(const Vector& v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
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

  double x, y, z;
};

inline Vector operator+(Vector v1, const Vector& v2)
{
  v1 += v2;
  return v1;
}

inline Vector operator-(Vector v1, const Vector& v2)
{
  v1 -= v2;
  return v1;
}

//scalar product
inline double operator*(const Vector& v,const Vector& u)
{
  return v.x*u.x+v.y*u.y+v.z*u.z;
}

inline Vector operator*(Vector v, double a)
{
  v *= a;
  return v;
}

inline Vector operator*(double a, Vector v)
{
  return v*a;
}

inline Vector operator/(Vector v, double a)
{
  v /= a;
  return v;
}

std::ostream& operator<<(std::ostream& os,const Vector v);

typedef Vector Point;

#endif /* VECTOR_H */
