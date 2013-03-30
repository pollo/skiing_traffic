/*
 * File:   skier.cpp
 *
 * Skier implementation
 *
 */

using namespace std;

#include "skier.h"
#include "parameters.h"
#include "physicalforces.h"
#include "slope.h"
#include <set>
#include <cmath>
#include <cassert>
#include <iostream>
#include <algorithm>

#define EPS 0.000000001
#define K 0.00001

Skier::Skier(int id,
             const Slope& slope,
             const Vector &position,
             const Vector &direction,
             const Vector &velocity,
             const Vector &acceleration):
  id(id), slope(slope), position(position), direction(direction),
  velocity(velocity), acceleration(acceleration),
  mass(settings::average_mass)
{
  //the skier is not allowed to fly
  (this->position.z) = get_current_elevation();
  //TODO: check that direction lies on the slope plane
  //TODO: check that velocity lies on the slope plane
  //TODO: check that acceleration lies on the slope plane
}

Skier::Skier(int id,
             const Slope& sl,
             const Point &position) :
  id(id), slope(sl), position(position), mass(settings::average_mass),
  turning_right(false), turning_left(false)
{
  double slope, aspect;
  slope = get_current_slope();
  aspect = get_current_aspect();
  //the skier is not allowed to fly
  (this->position).z = get_current_elevation();
  //initial direction is along the fall line
  direction.x = cos(slope) * cos(aspect);
  direction.y = cos(slope) * sin(aspect);
  direction.z = sin(slope) * -1;
  //cout << "direction inclination " << direction.inclination_angle() <<endl;
  //cout << "get_current_inclination_angle " << get_current_inclination_angle() << endl;
  //cout << slope << endl;
  //check vector direction to be on the slope plane
  assert(abs(-slope - get_current_inclination_angle()) < EPS);
  //initial velocity is 0
  //velocity.x = 0;
  //velocity.y = 0;
  //velocity.z = 0;
  //initial velocity is 1 (equal to direction)
  velocity = direction;
  //initial acceleration is 0
  acceleration.x = 0;
  acceleration.y = 0;
  acceleration.z = 0;
}

//returns the inclination angle along the direction
double Skier::get_current_inclination_angle() const
{
  return slope.get_slope_from_p1_to_p2(position,position+direction);
}

//Updates the position and the parameters of the skier for a time of dtime
//The acceleration that acted during this dtime is compute (start position
//is considered).
//The position of the skier is update moving the skier according to the
//velocity computed in the previous step.
//The new velocity is computed according to the old velocity and the effect
//of the acceleration. Finally the new direction is derived by the velocity.
void Skier::update(double dtime)
{
  bool cell_border_reached = false;
  double time_to_reach_border;
  //cout << "dtime " << dtime << endl;
  //moves skier for dtime or until the cell border is reached
  cell_border_reached = update_position(dtime, &time_to_reach_border);
  cerr.precision(13);
  cerr << position.x << " " << position.y <<" " << position.z << " (" << velocity.x
       <<","<< velocity.y <<","<< velocity.z<<") " << velocity.norm()<<" ("<<acceleration.x
       <<","<<acceleration.y<<","<<acceleration.z<<") "
       << (turning_left ? 1 : (turning_right ? 2 : 0)) << " " << endl;
  //cout << "inclination angle " << get_current_inclination_angle() << endl;
  //cout << "position" << endl;
  //cout << position;
  if (cell_border_reached)
  {
    //adjust direction to have inclination angle of the plane
    //because it will be used in the acceleration computation
    direction.rotate_axis_xy(direction.inclination_angle() -
                             get_current_inclination_angle());
  }
  //cout << "direction" << endl;
  //cout << direction;
  //cout << direction.inclination_angle() << endl;
  //computes acceleration
  update_acceleration();
  //cout << "acceleration" << endl;
  //cout << acceleration;
  //cout << acceleration.inclination_angle() << endl;
  //if the cell border is reached update velocity only for the time used
  if (cell_border_reached)
    update_velocity(time_to_reach_border, true);
  else
    update_velocity(dtime, false);
  update_direction();
  //cout << "velocity" << endl;
  //cout << velocity;
  //cout << velocity.inclination_angle() << endl;
  if (velocity.norm() > 0 )
    assert(abs(velocity.inclination_angle()-get_current_inclination_angle())<EPS);
  if (cell_border_reached)
    update(dtime-time_to_reach_border);

}

void Skier::reflect_edge(const Vector d)
{
    Point intersection;
    Point difference;
    double angle;
    //cerr << "x y z\n";
    //cerr << position.x << " " << position.y <<" " << position.z << " " << endl;
    //cerr << d.x << " " << d.y <<" " << d.z << " " << endl;
    slope.reflect_line(position,d,&intersection,&angle);
    //cout << "angle "<< angle/settings::degree_to_radians<<endl;
    //vector from position to intersection
    difference = position - intersection;
    //vector from intersection to destination
    difference.rotate(2*angle);
    position = intersection + difference;
    position.z = get_current_elevation();
    //update velocity and direction
    direction.rotate(2*angle);
    velocity.x *= -1;
    velocity.y *= -1;
    velocity.rotate(2*angle);
    //update x and y of direction in order to compute inclination angle
    update_direction();
    velocity.rotate_axis_xy(velocity.inclination_angle() -
                            get_current_inclination_angle());
    update_direction();
    //cerr << position.x << " " << position.y <<" " << position.z << " " << endl;
    assert(slope.is_inside_slope(position));
}

//Moves the skier for dtime according to the velocity.
//Returns wheter the skier reached the border of the cell or not
//If the border is reached the time used is stored in time_to_reach_border
//If the slope edge is reached the skier is "reflected"
bool Skier::update_position(double dtime, double* time_to_reach_border)
{
  bool border_reached;
  Point d;
  double east, west, north, sud;
  slope.get_cell_bound(position,&east,&west,&north,&sud);
  d = position + velocity * dtime;
  //if new position is inside the cell
  if ((d.x>=west && d.x<east) && (d.y>=sud && d.y<north))
  {
    //cout << "inside cell" << endl;
    double elev = slope.get_elevation(d);
    cout.precision(10);
    //cout << "--------------------" << endl;
    //cout << "aspect " << get_current_aspect()/settings::degree_to_radians <<endl;
    //cout << "slope " << get_current_slope()/settings::degree_to_radians <<endl;
    //cout << "destination" << endl << d;
    //cout << elev << endl;
    slope.get_elevation(d);
    //cout << "position" << endl << position;
    slope.get_elevation(position);
    //cout << "position to destination " << slope.get_slope_from_p1_to_p2(position, d)/settings::degree_to_radians  << endl;
    //cout << "--------------------" << endl;
    //cout << "error " << abs(d.z - elev) << endl;
    assert(abs(d.z - elev) < EPS);
    d.z = elev;
    border_reached = false;
  }
  else
  {
    //cout << "outside cell" << endl;
    double xtime, ytime, time;
    double elev;
    //time to reach x border
    if (velocity.x > 0)
      xtime = (east - position.x) / velocity.x;
    else
      xtime = (west - position.x) / velocity.x;
    //time to reach y border
    if (velocity.y > 0)
      ytime = (north - position.y) / velocity.y;
    else
      ytime = (sud - position.y) / velocity.y;
    time = min(xtime,ytime);
    //cout << "time to border " << time << endl;
    d = position + velocity * time;
    //adding K needed to cross the cell
    if (xtime<ytime)
      d.x += K * (velocity.x > 0 ? 1 : -1);
    else
      d.y += K * (velocity.y > 0 ? 1 : -1);
    *time_to_reach_border = time;
    border_reached = true;
    elev = slope.get_elevation(d);
    //cout << "height difference " << d.z << " " << elev << endl;
    //check that height difference from one cell to another is not
    //greater than 20 cm
    if (abs(d.z - elev) > 0.3)
    {
      static int i=1;
      cout.precision(13);
      cout << "Warning: From position x=" << position.x <<" y="<<position.y;
      cout <<"to x="<<d.x<<" y="<<d.y<<" there is a difference in elevation";
      cout <<" estimated and real elevation of "<<abs(d.z - elev)<<endl;
      //cerr << i << " " << d.x << " " << d.y <<" " << d.z << " " << endl;
      //cerr << i*100 << " " << d.x << " " << d.y <<" " << elev << " " << endl;
      i++;
    }
    //assert(abs(d.z - elev) < 0.50);
    //adjust elevation
    d.z = elev;
  }
  if (!slope.is_inside_slope(d))
  {
    reflect_edge(d);
  }
  else
    position = d;
  return border_reached;
}

void Skier::update_acceleration()
{
  Vector force(0,0,0);
  const set<PhysicalForce*>& forces = slope.get_physical_forces();
  for (set<PhysicalForce*>::const_iterator iter = forces.begin();
       iter != forces.end(); ++iter)
  {
//    Vector tmp =(*iter)->apply(*this);
//    assert(abs(slope.get_slope_from_p1_to_p2(position,position+tmp) -
//             tmp.inclination_angle()) < EPS);
//    cout << force;
    force += (*iter)->apply(*this);
  }
  acceleration = force / mass;
//  cout << slope.get_slope_from_p1_to_p2(position,position+acceleration)
//       << " " << acceleration.inclination_angle() << endl;
  assert(abs(slope.get_slope_from_p1_to_p2(position,position+acceleration) -
             acceleration.inclination_angle()) < EPS);
}

void Skier::update_velocity(double dtime, bool update_inclination)
{
  //if the cell is changed adjust old velocity inclination so that it still
  //lies on the plane of the slope
  if (update_inclination)
    velocity.rotate_axis_xy( velocity.inclination_angle() -
                             get_current_inclination_angle());

  velocity = velocity + acceleration * dtime;

  if (velocity.norm() < 1)
  {
    //assume that the minimum velocity is 1 m/s -> 3.6 Km/h, when the skier has
    //this velocity he is walking problably due to a counter slope
    velocity.normalize();
  }
}

void Skier::update_direction()
{
  if (velocity.norm() > 0)
    direction = velocity / velocity.norm();
}

bool Skier::fall_line_crossed() const
{
  double dir;
  double aspect = get_current_aspect();
  dir = atan(direction.y / direction.x) + (direction.x<0 ? M_PI : 0) +
    (direction.x>0 && direction.y<0 ? 2 * M_PI : 0);

  //cout << "dir " << dir/settings::degree_to_radians << " asp " << aspect/settings::degree_to_radians << endl;

  if (turning_left)
  {
    assert(!turning_right);
    if (aspect > 0 && aspect < M_PI)
      return (dir > aspect && dir < aspect + M_PI);
    else
      return !(dir < aspect && dir > aspect - M_PI);
  }
  else
  {
    assert(turning_right);

    if (aspect > 0 && aspect < M_PI)
      return !(dir > aspect && dir < aspect + M_PI);
    else
      return (dir < aspect && dir > aspect - M_PI);
  }
}

double Skier::get_current_elevation() const
{
  return slope.get_elevation(position);
}

//returns the slope of the cell
double Skier::get_current_slope() const
{
  return slope.get_slope(position);
}

double Skier::get_current_aspect() const
{
  return slope.get_aspect(position);
}
