/*
 * File:   skier.h
 *
 * The class Skier
 */

#ifndef SKIER_H
#define SKIER_H

#include "vector.h"

//including slope.h will result in a circular dependency
class Slope;

class Skier {
public:
  Skier(int id,
        const Slope& slope,
        const Vector& position,
        const Vector& direction,
        const Vector& velocity,
        const Vector& acceleration);
  //gives default values to direction (alogn the fall line)
  //velocity (0) and acceleration (0)
  Skier(int id,
        const Slope& slope,
        const Vector& position);

  Vector get_position() const { return position; }
  Vector get_waypoint() const { return waypoint; }
  Vector get_direction() const { return direction; }
  Vector get_velocity() const { return velocity; }
  double get_mass() const { return mass; }
  int get_id() const { return id; }
  bool turning() const { return (turning_right || turning_left); }
  bool fall_line_crossed() const;

  double get_current_elevation() const;
  double get_current_slope() const;
  double get_current_aspect() const;
  //returns the inclination angle along the current trajectory (if the
  //trajectory is downhill the angle is negative
  double get_current_inclination_angle() const;
  //returns distance from left and intersection point
  double current_distance_from_left(Point *pl) const;
  //returns distance from right and intersection point
  double current_distance_from_right(Point *pl) const;
  bool is_inside_stop_area() const;

  void update(double dtime);

private:
  int id;
  const Slope& slope;
  Point position;
  Vector direction;
  Vector velocity;
  Vector acceleration;
  double mass;
  bool turning_right, turning_left;
  double time_since_last_waypoint;
  Point waypoint;

  //updates the position of the skier using the Vector velocity
  //moves for dtime seconds
  bool update_position(double dtime, double* time_to_reach_border);
  void update_velocity(double dtime, bool update_inclination);
  void update_direction();
  void update_acceleration();
  void reflect_edge(const Vector d);
  void decide_turn();
  void choose_waypoint(double dtime);
};

#endif /* SKIER_H */
