/*
 * File:   slope.cpp
 *
 * Slope implementation
 *
 */

using namespace std;

#include "slope.h"
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <limits>

#define EPS 0.001

Slope::Slope(const GisBackend& gb) : gb(gb)
{
}

//returns the elevation in the point p. The computation is based on the slope of
// the cell, the distance of the point from the center of the cell and the
// direction of the fall line
double Slope::get_elevation(const Point& p) const
{
  Point center;
  double slope_center_point;
  double x, y, dist, delev;
  get_cell_center(p,&center);

  x = p.x - center.x;
  y = p.y - center.y;

  dist = sqrt(x*x+y*y);
  slope_center_point = get_slope_from_p1_to_p2(center,p);
  //delta elevation from the center to the point
  if (isnan(slope_center_point))
    delev = 0;
  else
    delev = dist * tan(slope_center_point);

  return center.z + delev;
}

//returns the slope along the line p1-p2 in the direction from p1 to p2
//slope and aspect are taken from the p1 cell
double Slope::get_slope_from_p1_to_p2(const Point& p1, const Point& p2) const
{
  double x, y;
  double slope, aspect;
  double angle_line_points, angle_horizontal_linepoints;
  double degree_to_radians = M_PI/180.0;
  //the slope and the aspect are considered the same from p1 to p2
  //values of p1 are taken
  aspect = get_aspect(p1) * degree_to_radians;
  slope = get_slope(p1) * degree_to_radians;

  if (isnan(aspect) || isnan(slope))
    return numeric_limits<double>::quiet_NaN();

  x = p1.x - p2.x;
  y = p1.y - p2.y;

  //compute the angle beetween the x direction and the line from p1 to p2
  if (abs(x) < EPS)
    //avoid division by small x: the angle is pi/2 or -pi/2 depending on y
    angle_line_points = M_PI / 2.0 * (y>0 ? 1 : -1);
  else
    //atan is between pi/2 and -pi/2, if in 2 or 3 quadrant add pi to correct
    //tha angle
    angle_line_points = atan(y/x) + (x<0 ? M_PI : 0);

  //compute angle between horizontal and linepoints
  angle_horizontal_linepoints =  angle_line_points - aspect + M_PI / 2.0;;

  //compute the slope from p1 to p2
  return asin( sin(slope) * sin(angle_horizontal_linepoints));
}

double Slope::get_slope(const Point& p) const
{
  return gb.get_slope(p.x, p.y);
}

double Slope::get_aspect(const Point& p) const
{
  return gb.get_aspect(p.x, p.y);
}

double Slope::distance_from_left(const Point& p) const
{
  return gb.distance_from_left(p.x, p.y);
}

double Slope::distance_from_right(const Point& p) const
{
  return gb.distance_from_right(p.x, p.y);
}

bool Slope::is_inside_slope(const Point& p) const
{
  return gb.is_inside_slope(p.x, p.y);
}

void Slope::get_cell_center(const Point& p,
                            Point* c) const
{
  gb.get_cell_center(p.x,p.y,&c->x,&c->y);
  c->z = gb.get_elevation(p.x,p.y);
}

const set<PhysicalForce*>& Slope::get_physical_forces() const
{
  return physical_forces;
}

const set<SocialForce*>& Slope::get_social_forces() const
{
  return social_forces;
}

const set<Skier>& Slope::get_skiers() const
{
  return skiers;
}
