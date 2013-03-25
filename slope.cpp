/*
 * File:   slope.cpp
 *
 * Slope implementation
 *
 */

using namespace std;

#include "slope.h"
#include "parameters.h"
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <limits>

#define EPS 0.001

Slope::Slope(const GisBackend& gb,
             std::ofstream &output_file) : gb(gb), output_file(output_file)
{
  time_since_last_skier = 0;
  next_skier_id = 1;
  output_file << "id x y z " << endl;
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
    delev = dist * tan(slope_center_point * settings::degree_to_radians);

  return center.z + delev;
}

//returns the slope along the line p1-p2 in the direction from p1 to p2
//slope and aspect are taken from the p1 cell
double Slope::get_slope_from_p1_to_p2(const Point& p1, const Point& p2) const
{
  double x, y;
  double slope, aspect;
  double angle_line_points, angle_horizontal_linepoints;
  //the slope and the aspect are considered the same from p1 to p2
  //values of p1 are taken
  aspect = get_aspect(p1) * settings::degree_to_radians;
  slope = get_slope(p1) * settings::degree_to_radians;

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
  return asin( sin(slope) * sin(angle_horizontal_linepoints))/settings::degree_to_radians;
}

void Slope::start_skiers(double dtime)
{
  double time_between_skiers = 3600 / settings::persons_hour;
  while (time_since_last_skier >= time_between_skiers)
  {
    Point p;
    gb.get_start_point(&p.x,&p.y);
    Skier *s = new Skier(next_skier_id,*this,p);
    next_skier_id++;
    skiers.insert(s);
    time_since_last_skier -= time_between_skiers;
  }
  time_since_last_skier += dtime;
}

void Slope::update_skiers(double dtime)
{
  for (set<Skier*>::iterator skier = skiers.begin();
       skier != skiers.end(); ++skier)
  {
    (*skier)->update(dtime);
  }
}

void Slope::log_skiers_situation() const
{
  for (set<Skier*>::iterator skier = skiers.begin();
       skier != skiers.end(); ++skier)
  {
    const Point &p = (*skier)->get_position();
    output_file.precision(13);
    output_file << (*skier)->get_id() <<" "<< p.x <<" "<< p.y <<" ";
    output_file.precision(6);
    output_file << p.z <<endl;
  }
}

void Slope::update(double dtime)
{
  start_skiers(dtime);
  update_skiers(dtime);
  log_skiers_situation();
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

void Slope::get_cell_bound(const Point& p,
                           double *east, double *west,
                           double *north, double *sud) const
{
  gb.get_cell_bound(p.x,p.y,east,west,north,sud);
}

bool Slope::reflect_line(const Point& start,
                         const Poitn& end,
                         Point* intersection,
                         double* angle) const
{
  return gb.reflect_line(start.x, start.y,
                         end.x, end.y,
                         &(intersection->x), &(intersection->y),
                         angle);
}

const set<PhysicalForce*>& Slope::get_physical_forces() const
{
  return physical_forces;
}

const set<SocialForce*>& Slope::get_social_forces() const
{
  return social_forces;
}

const set<Skier*>& Slope::get_skiers() const
{
  return skiers;
}
