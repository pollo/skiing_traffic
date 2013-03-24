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
 * - get the set of physical forces that acts on the slope
 * - get the set of social forces that acts on the slope
 * - update the position of the skier on the slope
 *
 * Informations about the skiers on the slope is logged in the stream passed
 * as parameter to the constructor.
 */

#ifndef SLOPE_H
#define SLOPE_H

#include "gisbackend.h"
#include "skier.h"
#include "vector.h"
#include "physicalforce.h"
#include "socialforce.h"
#include <fstream>
#include <set>

class Slope {
public:
  Slope(const GisBackend& gb,
        std::ofstream &output_file);

  //returns the elevation for the point p, slope is taken into account
  double get_elevation(const Point& p) const;
  double get_slope(const Point& p) const;
  double get_aspect(const Point& p) const;
  double distance_from_left(const Point& p) const;
  double distance_from_right(const Point& p) const;
  bool is_inside_slope(const Point& p) const;
  //returns the slope along the line p1-p2 in the direction from p1 to p2
  double get_slope_from_p1_to_p2(const Point& p1, const Point& p2) const;
  void update(double dtime);
  const std::set<PhysicalForce*>& get_physical_forces() const;
  const std::set<SocialForce*>& get_social_forces() const;
  const std::set<Skier*>& get_skiers() const;

private:
  const GisBackend& gb;
  std::ofstream& output_file;
  std::set<PhysicalForce*> physical_forces;
  std::set<SocialForce*> social_forces;
  std::set<Skier*> skiers;
  //time elapsed since the last skier started
  double time_since_last_skier;
  int next_skier_id;

  //stores in the point c the center point of the cell in which the p Point is
  void get_cell_center(const Point& p,
                       Point* c) const;
  //start new skiers at the top of the slope
  void start_skiers(double dtime);
  void update_skiers(double dtime);
  void log_skiers_situation() const;
};

#endif /* SLOPE_H */
