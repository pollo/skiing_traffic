/*
 * File:   downhill_force.cpp
 *
 * Defines the downhill force acting on a skier
 *
 */

#include "physicalforces.h"
#include "parameters.h"
#include <cmath>
#include <cassert>

using namespace std;

#define EPS 0.000000001

Vector DownhillForce::apply(const Skier& skier)
{
  //cout << "downhill\n";
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

Vector CentripetalForce::apply(const Skier& skier)
{
  //cout << "centripeta\n";
  if (!skier.turning())
  {
    return Vector(0,0,0);
  }
  else
  {
    DownhillForce fp;
    const double rsc = settings::sidecut_radius;
    double m = skier.get_mass();
    double r = (skier.get_velocity()).norm();
    Vector f_lat = fall_line_force(skier) - fp.apply(skier);
    f_lat.normalize();
    ////cout << f_lat;
    double direction = (skier.fall_line_crossed() ? -1 : 1);
    ////cout << direction <<endl;
    return m / rsc * r*r * f_lat * direction;
  }
}

Vector AirDragForce::apply(const Skier& skier)
{
  //cout << "airdrag\n";
  const double cd = settings::air_drag_coefficient;
  const double A = settings::frontal_area;
  const double p = settings::air_density;
  double r = (skier.get_velocity()).norm();
  return -0.5 * cd * A * p * r*r * skier.get_direction();
}

Vector KineticFrictionForce::apply(const Skier& skier)
{
  //cout << "kinetic\n";
  const double u = settings::kinetic_friction_coefficient;
  double f_eff = effective_force(skier).norm();
  return -u * f_eff * skier.get_direction();
}

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

Vector fall_line_force(const Skier& skier)
{
  Vector force;
  const double g = settings::g;
  double m = skier.get_mass();

  force = m*g* Vector(0,0,-1) + normal_force(skier);

  //cout << force.inclination_angle() << " " << skier.get_current_slope() << endl;
  assert(abs(force.inclination_angle() + skier.get_current_slope()) < EPS);

  return force;
}

/*
Vector partial_lat_force(const Skier& skier)
{
  DownhillForce fp;

  return fall_line_force(skier) - fp.apply(skier);
  }*/

Vector lat_force(const Skier& skier)
{
  DownhillForce fp;
  if (!skier.turning())
  {
    return fall_line_force(skier) - fp.apply(skier);
  }
  else
  {
    CentripetalForce fc;
    return fall_line_force(skier) - fp.apply(skier) - fc.apply(skier);
  }
}

Vector effective_force(const Skier& skier)
{
  return lat_force(skier) - normal_force(skier);
}
