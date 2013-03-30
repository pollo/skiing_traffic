/*
 * File:   parameters.h
 *
 * Declares the parameters for the simulation
 *
 */


#ifndef PARAMETERS_H
#define PARAMETERS_H

struct settings {
  static const double g;
  static const double average_mass;
  static const int persons_hour;
  static const double degree_to_radians;
  static const double air_drag_coefficient;
  static const double frontal_area;
  static const double air_density;
  static const double sidecut_radius;
  static const double kinetic_friction_coefficient;
};

#endif /* PARAMETERS_H */
