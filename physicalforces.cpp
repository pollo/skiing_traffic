/*
 * File:   physicalforces.cpp
 *
 * Defines the physical forces acting on a skier
 *
 */

#include "physicalforces.h"
#include "parameters.h"
#include <cmath>
#include <cassert>

using namespace std;

#define EPS 0.00000001

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
  //cout << "downhill " << skier.get_direction().inclination_angle() << " " << force.inclination_angle() << endl;
  //cout << force.inclination_angle() << " " << skier.get_current_inclination_angle() << endl;
  assert(abs(abs(force.inclination_angle()) - abs(skier.get_current_inclination_angle())) < EPS);
  return force;
}

Vector CentripetalForce::apply(const Skier& skier)
{
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
    //cout << fall_line_force(skier) << " " << fp.apply(skier) <<endl;
    //cout << "-------------" << endl;
    //cout << fall_line_force(skier).angle_on_xyplane() << " " << fp.apply(skier).angle_on_xyplane() <<endl;
    f_lat.normalize();
    double direction = (skier.fall_line_crossed() ? -1 : 1);
    //cout << "forza laterale\n";
    //cout << f_lat;
    //cout << "x_angle " << f_lat.angle_on_xyplane()/settings::degree_to_radians <<endl;
    return m / rsc * r*r * f_lat * direction;
  }
}

Vector AirDragForce::apply(const Skier& skier)
{
  const double cd = settings::air_drag_coefficient;
  const double A = settings::frontal_area;
  const double p = settings::air_density;
  double r = (skier.get_velocity()).norm();
  //cout << "airdrag\n";
  return -0.5 * cd * A * p * r*r * skier.get_direction();
}

Vector KineticFrictionForce::apply(const Skier& skier)
{
  const double u = settings::kinetic_friction_coefficient;
  double f_eff = effective_force(skier).norm();
  //cout << "kinetic\n";
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
  assert(abs(force.angle_on_xyplane() - skier.get_current_aspect()) < EPS);

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
