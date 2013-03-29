/*
 * File:   downhill_force.cpp
 *
 * Defines the downhill force acting on a skier
 *
 */

#include "downhill_force.h"
#include "parameters.h"
#include <cmath>

Vector DownhillForce::apply(const Skier& skier)
{
  Vector force;
  const double g = settings::g;
  double angle = skier.get_current_inclination_angle();
  //in the following formula the angle is considered positive even if the
  //direction of motion is downhill
  angle *= -1;
  //fp = mg (sin γ) er
  force = skier.get_mass() * g * sin(angle) * skier.get_direction();
  return force;
}
