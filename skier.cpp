/*
 * File:   skier.cpp
 *
 * Skier implementation
 *
 */

using namespace std;

#include "skier.h"
#include "parameters.h"
#include "physicalforce.h"
#include "slope.h"
#include <set>
#include <cmath>
#include <cassert>
#include <iostream>
#include <algorithm>

#define EPS 0.00000000001
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
  id(id), slope(sl), position(position), mass(settings::average_mass)
{
  double slope, aspect;
  const double d2g = settings::degree_to_radians;
  slope = get_current_slope();
  aspect = get_current_aspect();
  //the skier is not allowed to fly
  (this->position).z = get_current_elevation();
  //initial direction is along the fall line
  direction.x = cos(slope * d2g) * cos(aspect * d2g);
  direction.y = cos(slope * d2g) * sin(aspect * d2g);
  direction.z = sin(slope * d2g) * -1;
  //check vector direction to be on the slope plane
  assert(abs(-slope - get_current_inclination_angle()) < EPS);
  //initial velocity is 0
  velocity.x = 0;
  velocity.y = 0;
  velocity.z = 0;
  //initial acceleration is 0
  acceleration.x = 0;
  acceleration.y = 0;
  acceleration.z = 0;
}

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
  //computes acceleration before moving
  update_acceleration();
  //moves skier for dtime or until the cell border is reached
  cell_border_reached = update_position(dtime, &time_to_reach_border);
  //if the cell border is reached update velocity only for the time used
  if (cell_border_reached)
    update_velocity(time_to_reach_border);
  update_direction();
  if (cell_border_reached)
    update(dtime-time_to_reach_border);
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
    border_reached = false;
  }
  else
  {
    double xtime, ytime, time;
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
    cout << "x " << position.x << endl;
    cout << "y " << position.y << endl;
    d = position + velocity * time;
    cout << "x " << d.x << endl;
    cout << "y " << d.y << endl;
    cout << "position " << d.z << endl;
    cout << get_current_elevation();
    assert(abs(d.z - get_current_elevation()) < EPS);
    //adding K needed to cross the cell
    d.x += K;
    d.y += K;
    *time_to_reach_border = time;
    border_reached = true;
  }
  if (!slope.is_inside_slope(d))
  {
    Point intersection;
    Point difference;
    double angle;
    slope.reflect_line(position,d,&intersection,&angle);
    angle = angle * settings::degree_to_radians;
    //vector from position to intersection
    difference = position - intersection;
    //vector from intersection to destination
    difference.rotate(angle);
    position = intersection + difference;
    position.z = get_current_elevation();
    assert(slope.is_inside_slope(position));
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
    force += (*iter)->apply(*this);
  }
  acceleration = force / mass;
}

void Skier::update_velocity(double dtime)
{
  velocity = velocity + acceleration * dtime;
}

void Skier::update_direction()
{
  direction = velocity / velocity.norm();
}

double Skier::get_current_elevation() const
{
  return slope.get_elevation(position);
}

double Skier::get_current_slope() const
{
  return slope.get_slope(position);
}

double Skier::get_current_aspect() const
{
  return slope.get_aspect(position);
}
