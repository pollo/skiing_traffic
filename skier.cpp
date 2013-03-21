/*
 * File:   skier.cpp
 *
 * Skier implementation
 *
 */

using namespace std;

#include "skier.h"
#include "parameters.h"
#include <cmath>

#define EPS 0.00000000001

Skier::Skier(const Slope& slope,
             const Vector &position,
             const Vector &direction,
             const Vector &velocity,
             const Vector &acceleration) :
  slope(slope), position(position), direction(direction),
  velocity(velocity), acceleration(acceleration),
  mass(settings::average_mass)
{
  //the skier is not allowed to fly
  postion.z = get_current_elevation();
  //TODO: check that direction lies on the slope plane
  //TODO: check that velocity lies on the slope plane
  //TODO: check that acceleration lies on the slope plane
}

Skier::Skier(const Slope& slope,
             const Point &position) :
  slope(slope), position(position), mass(settings::average_mass)
{
  double slope, aspect;
  slope = get_current_slope();
  aspect = get_current_aspect();
  //the skier is not allowed to fly
  postion.z = get_current_elevation();
  //initial direction is along the fall line
  direction.x = cos(slope) * cos(aspect);
  direction.y = cos(slope) * sen(aspect);
  direction.z = sen(slope) * -1;
  assert(abs(slope - get_current_inclination_angle) < EPS);
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

void Skier::update(double dtime)
{
  update_position(dtime);
  update_velocity(dtime);
  update_direction();
  update_acceleration();
}

void Skier::update_position(double dtime)
{


}

void Skier::update_velocity(double dtime)
{


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
    force += **iter;
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
