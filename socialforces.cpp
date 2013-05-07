/*
 * File:   socialforces.cpp
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

Vector DestinationForce::apply(const Skier& skier)
{
  Vector force = skier.get_waypoint() - skier.get_position();
  force.normalize();
  return settings::waypoint_force_strength * force;
}

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
  #ifdef DEBUG
  settings::left() << skier.get_position().x << " " << skier.get_position().y << " (" << distance.x <<","<<distance.y<<","<<distance.z<<") "<<distance.norm()<< " " << skier.get_direction().angle_on_xyplane()/settings::degree_to_radians << " " << (-1*distance).angle_on_xyplane()/settings::degree_to_radians << " " << endl;
  settings::left() << intersection.x << " " << intersection.y <<endl;
  #endif
//(skier.get_direction()/skier.get_direction().norm()) * (distance/distance.norm()) << " " << skier.get_direction().norm()*distance.norm() << " " << endl;
  distance *= visible(skier.get_direction(),-1*distance);
  return distance;
}

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
  #ifdef DEBUG
  settings::right() << skier.get_position().x << " " << skier.get_position().y << " (" << distance.x <<","<<distance.y<<","<<distance.z<<") "<<distance.norm()<< " " << endl;
  settings::right() << intersection.x << " " << intersection.y <<endl;
  #endif
  return distance;
}

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
      //cout << "skier repulsion"<< endl;
      //cout << r_ab;
      force += f_ab * visible(skier.get_direction(),-1*f_ab);
    }
  }
  #ifdef DEBUG
  settings::rep() << skier.get_position().x << " " << skier.get_position().y << " (" << force.x <<","<<force.y<<","<<force.z<<") "<<force.norm()<< " " << endl;
  #endif
  return force;
}
