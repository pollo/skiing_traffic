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

#define EPS 0.00000000001

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
//First, the 
void Skier::update(double dtime)
{
  update_acceleration();
  update_position(dtime);
  update_velocity(dtime);
  update_direction();
}

void Skier::update_position(double dtime)
{
  

}

void Skier::update_velocity(double dtime)
{
  velocity = acceleration
}

void Skier::update_direction()
{


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
