/*
 *
 * Defines the physical forces acting on a skier
 *
 */

#include "physicalforces.h"
#include "parameters.h"
#include <cmath>
#include <cassert>
#include <cstdlib>

using namespace std;

#define EPS 0.0000001

//F_p
Vector downhill_force(const Skier& skier)
{
  Vector force;
  const double g = settings::g;
  double angle = skier.get_current_inclination_angle();
  //in the following formula the angle is considered positive even if the
  //direction of motion is downhill
  angle *= -1;
  //fp = mg (sin γ)
  force =  skier.get_mass() * g * sin(angle) * skier.get_direction();
  assert(abs(abs(force.inclination_angle()) -
             abs(skier.get_current_inclination_angle())) < EPS);
  return force;
}

//(1-\eta) F_p
Vector DownhillForce::apply(const Skier& skier)
{
  if (skier.turning())
    return (1-skier.get_skidding_factor()) * downhill_force(skier);
  else
    return downhill_force(skier);
}

//F_p
Vector DownhillForce::apply_without_skidding(const Skier& skier)
{
  return downhill_force(skier);
}

//F_C
Vector CentripetalForce::apply(const Skier& skier)
{
  if (!skier.turning())
  {
    return Vector(0,0,0);
  }
  else
  {
    DownhillForce fp;
    const double rsc = skier.get_turning_radius();
    double m = skier.get_mass();
    double r = (skier.get_velocity()).norm();
    Vector f_lat = fall_line_force(skier) - fp.apply_without_skidding(skier);
    f_lat.normalize();
    double direction = (skier.fall_line_crossed() ? -1 : 1);
    return m / rsc * r*r * f_lat * direction;
  }
}

//F_{air}
Vector AirDragForce::apply(const Skier& skier)
{
  const double cd = settings::air_drag_coefficient;
  const double A = settings::frontal_area;
  const double p = settings::air_density;
  double r = (skier.get_velocity()).norm();
  return -0.5 * cd * A * p * r*r * skier.get_direction();
}

//F_{ground}
Vector KineticFrictionForce::apply(const Skier& skier)
{
  const double u = settings::kinetic_friction_coefficient;
  double f_eff = effective_force(skier).norm();
  return -u * f_eff * skier.get_direction();
}

//F_N
Vector normal_force(const Skier& skier)
{
  Vector force;
  const double g = settings::g;
  double m = skier.get_mass();
  double slope = skier.get_current_slope();
  double aspect = skier.get_current_aspect();

  //normal versor
  force.x = sin(slope) * cos(aspect);
  force.y = sin(slope) * sin(aspect);
  force.z = cos(slope);

  assert(abs(force.inclination_angle() - M_PI/2.0 + slope) < EPS);

  force *= m*g*cos(slope);

  return force;
}

//F_S
Vector fall_line_force(const Skier& skier)
{
  Vector force;
  const double g = settings::g;
  double m = skier.get_mass();

  force = m*g* Vector(0,0,-1) + normal_force(skier);

  assert(abs(force.inclination_angle() + skier.get_current_slope()) < EPS);
  assert(abs(force.angle_on_xyplane() - skier.get_current_aspect()) < EPS);

  return force;
}

//F_{lat}
Vector lat_force(const Skier& skier)
{
  DownhillForce fp;
  if (!skier.turning())
  {
    return fall_line_force(skier) - fp.apply_without_skidding(skier);
  }
  else
  {
    CentripetalForce fc;
    return fall_line_force(skier) - fp.apply(skier) - fc.apply(skier);
  }
}

//F_{eff}
Vector effective_force(const Skier& skier)
{
  return lat_force(skier) - normal_force(skier);
}
