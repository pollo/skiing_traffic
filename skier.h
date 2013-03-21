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
  Skier(const Slope& slope,
        const Vector& position,
        const Vector& direction,
        const Vector& velocity,
        const Vector& acceleration);
  //gives default values to direction (alogn the fall line)
  //velocity (0) and acceleration (0)
  Skier(const Slope& slope,
        const Vector& position);
  double get_current_elevation() const;
  double get_current_slope() const;
  double get_current_aspect() const;
  //returns the inclination angle along the current trajectory
  double get_current_inclination_angle() const;

  void update(double dtime);

private:
  const Slope& slope;
  double mass;
  Vector position;
  Vector direction;
  Vector velocity;
  Vector acceleration;

  //updates the position of the skier using the Vector velocity
  //moves for dtime seconds
  void update_position(double dtime);
  void update_velocity(double dtime);
  void update_direction();
  void update_acceleration();
};

#endif /* SKIER_H */
