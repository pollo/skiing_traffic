/*
 * File:   socialforces.cpp
 *
 * Defines the social forces acting on a skier
 *
 */

#include "socialforces.h"
#include "parameters.h"
#include <cmath>
#include <cassert>

using namespace std;

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
  cout << "---sinistra---" << endl;
  cout << "dist " << norm << endl;
  distance *= U/Rl * exp(-norm/Rl)/norm;
  cout << distance;
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
  cout << "---destra---" << endl;
  cout << "dist " << norm << endl;
  distance *= U/Rr * exp(-norm/Rr)/norm;
  cout << distance;
  return distance;
}
