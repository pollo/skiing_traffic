#include "vector.h"
#include "parameters.h"
#include <cmath>
#include <iostream>

//the vector is rotated around the z axix for angle radians
void Vector::rotate(double angle)
{
  double x1, y1;
  x1 = cos(angle) * x - sin(angle) * y;
  y1 = sin(angle) * x + cos(angle) * y;
  x = x1;
  y = y1;
}

//the vector is rotated around the axis that lies on the x y plane and
//wich is perpendicular to the vector for angle radians
void Vector::rotate_axis_xy(double angle)
{
  //compute axis_versor
  Vector av(x,y,0);
  double x1, y1, z1;
  av.rotate(M_PI/2.0);
  av.normalize();
  //apply rotation (see formula to rotate in three dimensions)
  x1 = (av.x*av.x + (1 - av.x*av.x) *  cos(angle)) * x;
  x1 += ((1-cos(angle))*av.x*av.y) * y;
  x1 += (sin(angle) * av.y) * z;

  y1 = ((1-cos(angle))*av.y*av.x) * x;
  y1 += (av.y*av.y + (1 - av.y*av.y) * cos(angle)) * y;
  y1 += (-sin(angle) * av.x) * z;

  z1 = (-sin(angle)*av.y) * x;
  z1 += (sin(angle) * av.x) * y;
  z1 += (cos(angle)) * z;

  x = x1;
  y = y1;
  z = z1;
}

void Vector::normalize()
{
  *this /= this->norm();
}

double Vector::norm() const
{
  return sqrt(x*x+y*y+z*z);
}

double Vector::inclination_angle() const
{
  return atan(z / sqrt(x*x+y*y));
}

std::ostream& operator<<(std::ostream& os,const Vector v)
{
  os.precision(13);
  os << "( " << v.x << std::endl << "  " << v.y << std::endl << "  " << v.z << " )\n";
  return os;
}
