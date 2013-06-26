/*
 *
 * Defines the social forces acting on a skier
 *
 */

#include "socialforces.h"
#include "parameters.h"
#include "slope.h"
#include <cmath>
#include <cassert>

#define EPS 0.00001

using namespace std;

int visible(const Vector& u, const Vector& v)
{
  if ((u/u.norm()) * (v/v.norm()) >= cos(settings::angle_view/2.0)
      || (u.norm()*v.norm() < EPS))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

//F_D
Vector DestinationForce::apply(const Skier& skier)
{
  Vector force = skier.get_waypoint() - skier.get_position();
  force.normalize();
  return settings::waypoint_force_strength * force;
}

//F_L
Vector LeftForce::apply(const Skier& skier)
{
  Vector intersection;
  Vector distance;
  double norm;
  double U = settings::edge_repulsion_strength;
  double Rl = settings::range_repulsion_left;

  skier.current_distance_from_left(&intersection);
  distance = skier.get_position() - intersection;
  norm = distance.norm();
  distance *= U/Rl * exp(-norm/Rl)/norm;
  distance *= visible(skier.get_direction(),-1*distance);
  return distance;
}

//F_R
Vector RightForce::apply(const Skier& skier)
{
  Vector intersection;
  Vector distance;
  double norm;
  double U = settings::edge_repulsion_strength;
  double Rr = settings::range_repulsion_right;

  skier.current_distance_from_right(&intersection);
  distance = skier.get_position() - intersection;
  norm = distance.norm();
  distance *= U/Rr * exp(-norm/Rr)/norm;
  distance *= visible(skier.get_direction(),-1*distance);
  return distance;
}

//F_S
Vector SkiersForce::apply(const Skier& skier)
{
  double dtime = settings::dtime;
  Vector force(0,0,0);
  for (set<Skier*>::iterator bskier=s.get_skiers().begin();
       bskier != s.get_skiers().end();
       bskier++)
  {
    if ((*bskier)->get_id() != skier.get_id())
    {
      Vector r_ab = skier.get_position() - (*bskier)->get_position();
      double r_ab_norm = r_ab.norm();
      double v_b = (*bskier)->get_velocity().norm();
      const Vector& e_b = (*bskier)->get_direction();
      Vector rab_vb_dtime = r_ab - v_b * dtime * e_b;
      double rab_vb_dtime_norm = rab_vb_dtime.norm();
      double s = 0.5 * sqrt(pow(r_ab_norm + rab_vb_dtime_norm, 2)
                            - pow(v_b * dtime, 2));
      double V = settings::skier_repulsion_strength;
      double Ra = settings::range_repulsion_skier;
      Vector f_ab = 0.25 * V/Ra * exp(-s/Ra) * 1/s *
                    (r_ab_norm + rab_vb_dtime_norm) *
                    (r_ab/r_ab_norm + rab_vb_dtime/rab_vb_dtime_norm);
      force += f_ab * visible(skier.get_direction(),-1*f_ab);
    }
  }
  return force;
}
