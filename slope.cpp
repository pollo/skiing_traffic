/*
 *
 * Slope methods definition
 *
 */

using namespace std;

#include "slope.h"
#include "parameters.h"
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <limits>
#include <cassert>
#include <cstdlib>
#include <unistd.h>

#define EPS 0.000000000001

Slope::Slope(const GisBackend& gb,
             std::ofstream &output_file) :
  gb(gb), output_file(output_file), time_until_new_skier(0), next_skier_id(1),
  log(settings::log_freq)
{
  PhysicalForce *pf;
  SocialForce *sf;

  output_file << "id x y z vel time " << endl;

  pf = new DownhillForce();
  physical_forces.insert(pf);
  pf = new AirDragForce();
  physical_forces.insert(pf);
  pf = new KineticFrictionForce();
  physical_forces.insert(pf);
  pf = new CentripetalForce();
  physical_forces.insert(pf);

  sf = new LeftForce();
  social_forces.insert(sf);
  sf = new RightForce();
  social_forces.insert(sf);
  sf = new DestinationForce();
  social_forces.insert(sf);
  sf = new SkiersForce(*this);
  social_forces.insert(sf);

  //srand((unsigned)time(NULL));
}

Slope::~Slope()
{
  for (set<Skier*>::iterator skier = skiers.begin();
       skier != skiers.end(); ++skier)
    delete *skier;
  for (set<PhysicalForce*>::iterator physicalforce = physical_forces.begin();
       physicalforce != physical_forces.end(); ++physicalforce)
    delete *physicalforce;
  for (set<SocialForce*>::iterator socialforce = social_forces.begin();
       socialforce != social_forces.end(); ++socialforce)
    delete *socialforce;
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
//z coordinates are not used
double Slope::get_slope_from_p1_to_p2(const Point& p1, const Point& p2) const
{
  double x, y;
  double slope, aspect;

  aspect = get_aspect(p1);
  slope = get_slope(p1);

  if (isnan(aspect) || isnan(slope))
    return numeric_limits<double>::quiet_NaN();

  x = p2.x - p1.x;
  y = p2.y - p1.y;

  //if the two points are the same return 0
  if (abs(x) < EPS && abs(y) < EPS)
    return 0;

  //compute gradient vector
  double gradient_len = tan(slope);
  Vector gradient(gradient_len * cos(aspect + M_PI),
                  gradient_len * sin(aspect + M_PI));
  //compute direction
  Vector direction(x,y,0);
  direction.normalize();
  //the directional derivative is the scalar product beetwen gradient and dir
  double result = atan(gradient.x * direction.x + gradient.y * direction.y);

  return result;
}

void Slope::start_skiers(double dtime)
{
  if (time_until_new_skier <= 0)
  {
    double time_between_skiers = 3600.0 / (double)settings::persons_hour;
    Point p;
    gb.get_start_point(&p.x,&p.y);
    Skier *s = new Skier(next_skier_id,*this,p);
    next_skier_id++;
    skiers.insert(s);
    time_until_new_skier = (double)rand()/(double)RAND_MAX
                           *2*time_between_skiers;
  }
  time_until_new_skier -= dtime;
}

void Slope::update_skiers(double dtime)
{
  for (set<Skier*>::iterator skier = skiers.begin();
       skier != skiers.end(); ++skier)
  {
    (*skier)->update(dtime);
    if ((*skier)->is_inside_stop_area())
    {
      delete *skier;
      skiers.erase(skier);
    }
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
    output_file << p.z <<" ";
    output_file << (*skier)->get_velocity().norm()<<" ";
    output_file << time << " " <<endl;
  }
}

void Slope::update(double dtime)
{
  time += dtime;
  start_skiers(dtime);
  update_skiers(dtime);
  if (log-- == 0)
  {
    log = settings::log_freq;
    log_skiers_situation();
  }
  usleep(settings::slow_simulation);
}

double Slope::get_slope(const Point& p) const
{
  return gb.get_slope(p.x, p.y) * settings::degree_to_radians;
}

double Slope::get_aspect(const Point& p) const
{
  return gb.get_aspect(p.x, p.y) * settings::degree_to_radians;
}

//distance from left edge reporting also intersection point
double Slope::distance_from_left(const Point& p, Point* pl) const
{
  double dist = gb.distance_from_left(p.x, p.y, &(pl->x), &(pl->y));
  pl->z = get_elevation(*pl);
  return dist;
}

//returns distance from left edge
double Slope::distance_from_left(const Point& p) const
{
  return gb.distance_from_left(p.x, p.y, NULL, NULL);
}

//distance from right edge reporting also intersection point
double Slope::distance_from_right(const Point& p, Point* pl) const
{
  double dist = gb.distance_from_right(p.x, p.y, &(pl->x), &(pl->y));
  pl->z = get_elevation(*pl);
  return dist;
}

//returns distance from right edge
double Slope::distance_from_right(const Point& p) const
{
  return gb.distance_from_right(p.x, p.y, NULL, NULL);
}


bool Slope::is_inside_slope(const Point& p) const
{
  return gb.is_inside_slope(p.x, p.y);
}

bool Slope::is_inside_stop_area(const Point& p) const
{
  return gb.is_inside_stop_area(p.x, p.y);
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
                         const Point& end,
                         Point* intersection,
                         double* angle) const
{
  bool res;
  res = gb.reflect_line(start.x, start.y,
                  end.x, end.y,
                  &(intersection->x), &(intersection->y),
                  angle);
  *angle *= settings::degree_to_radians;
  return res;
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
