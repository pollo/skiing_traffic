/*
 * File:   slope.h
 *
 * The class Slope represent the ski slope on which the simulation is run.
 * It exposes methods to:
 * - get elevation in a point of the slope
 * - get the slope value in a point
 * - get the aspect value in a point
 * - get the distance from the left edge of the slope from a point
 * - get the distance from the right edge of the slope from a point
 * - check if a point is inside the slope
 * - returns the set of physical forces that acts on the slope
 * - returns the set of social forces that acts on the slope
 */

#ifndef SLOPE_H
#define SLOPE_H

#include "gisbackend.h"
#include "skier.h"
#include "point.h"
#include "physicalforce.h"
#include "socialforce.h"
#include <set>

class Slope {
public:
  Slope(const GisBackend& gb);

  double get_elevation(const Point& p) const;
  double get_slope(const Point& p) const;
  double get_aspect(const Point& p) const;
  double distance_from_left(const Point& p) const;
  double distance_from_right(const Point& p) const;
  bool is_inside_slope(const Point& p) const;
  const std::set<PhysicalForce>& get_physical_forces() const;
  const std::set<SocialForce>& get_social_forces() const;
  const std::set<Skier>& get_skiers() const;

private:
  const GisBackend& gb;
  std::set<PhysicalForce> physical_forces;
  std::set<SocialForce> social_forces;
  std::set<Skier> skiers;
};

#endif /* SLOPE_H */
