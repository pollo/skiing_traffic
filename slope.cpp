/*
 * File:   slope.cpp
 *
 * Slope implementation
 *
 */

using namespace std;


#include "slope.h"

Slope::Slope(const GisBackend& gb) : gb(gb)
{
}

double Slope::get_elevation(const Point& p) const
{
  return gb.get_elevation(p.x, p.y);
}

double Slope::get_slope(const Point& p) const
{
  return gb.get_slope(p.x, p.y);
}

double Slope::get_aspect(const Point& p) const
{
  return gb.get_slope(p.x, p.y);
}

double Slope::distance_from_left(const Point& p) const
{
  return gb.distance_from_left(p.x, p.y);
}

double Slope::distance_from_right(const Point& p) const
{
  return gb.distance_from_right(p.x, p.y);
}

bool Slope::is_inside_slope(const Point& p) const
{
  return gb.is_inside_slope(p.x, p.y);
}

const set<PhysicalForce>& Slope::get_physical_forces() const
{
  return physical_forces;
}

const set<SocialForce>& Slope::get_social_forces() const
{
  return social_forces;
}

const set<Skier>& Slope::get_skiers() const
{
  return skiers;
}
